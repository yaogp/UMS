#include "socksend.h"





//#define DEBUG


using namespace std;
using namespace util;

typedef basic_string<char>::size_type S_T; 
static const S_T npos = -1;



int getMaxmday(int y, int m)
{
    int const days[2][12] =
    {
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int idx;
    if (y % 400 == 0)
        idx = 1;
    else if (y % 100 == 0)
        idx = 0;
    else if (y % 4 == 0)
        idx = 1;
    else
        idx = 0;
    return days[idx][m];
}

string addSecond(string date,int second)
{
	datetime ddate(date);
	struct tm tdate;
	time_t tmp =ddate.duration();
    	localtime_r(&tmp, &tdate);
    	tdate.tm_sec = tdate.tm_sec+second;
	datetime rdate(mktime(&tdate));
	string fmt;
	rdate.iso_string(fmt);
	return fmt;
}










datetime calSendTime(datetime createTime,string sendSchema)
{
	struct tm tmcreate;
	time_t tmp =createTime.duration();
    	localtime_r(&tmp, &tmcreate);
	struct tm tmsend;
	tmsend.tm_year = 0;
	tmsend.tm_mon = 0;
	tmsend.tm_mday = 1;
	tmsend.tm_hour = 0;
	tmsend.tm_min = 0;
	tmsend.tm_sec = 0;
#ifdef DEBUG	
	cout<<"tmsend.tm_wday is "<<tmcreate.tm_wday <<endl<<std::flush;
#endif		
	vector<string> vformat=tokenize(sendSchema," ");
	if(vformat.size()!=5)
	{
		throw bad_param(__FILE__, __LINE__, 129, "error calSendTime"); 
	}
#ifdef DEBUG	
	cout<<"vformat[3] is "<<vformat[3] <<endl<<std::flush;
#endif		
	tmsend.tm_year = tmcreate.tm_year;
	int modifyFlag =0;
#ifdef DEBUG	
	cout<<"tmsend.tm_year is "<<tmsend.tm_year <<endl<<std::flush;
#endif		
	//deal month
	tmsend.tm_mon = tmcreate.tm_mon;
	if(vformat[3].compare("*")==0)
	{
#ifdef DEBUG		
		cout<<"month * " <<endl<<std::flush;
#endif			
	}
	else if(vformat[3].find("*/")==0)
	{
#ifdef DEBUG		
		cout<<"month */ " <<endl<<std::flush;
#endif			
		int div = atoi(vformat[3].substr(2).c_str());
		int mod = (tmsend.tm_mon+1)%div;
		int month = 0;
		if(mod != 0)
		{
			if(tmsend.tm_mon+div-mod>12)
			{
				tmsend.tm_year++;
				tmsend.tm_mon = div-1;
			}
			else
			{
				tmsend.tm_mon = tmsend.tm_mon+div-mod;
			}
			modifyFlag =1;
		}
	}
	else
	{
#ifdef DEBUG		
		cout<<"month enum " <<endl<<std::flush;
#endif			
		vector<string> vnum =tokenize(vformat[3],",");
#ifdef DEBUG		
		cout<<"vnum.size() is "<<vnum.size()<<endl<<std::flush;
#endif			
		int currYearFlag = 0;
		for(int i=0;i<vnum.size();i++)
		{
#ifdef DEBUG			
			cout<<"vnum[i] is "<<vnum[i] <<endl<<std::flush;
#endif				
			int pot = atoi(vnum[i].c_str());
#ifdef DEBUG			
			cout<<"pot is "<<pot <<endl<<std::flush;
#endif				
			if(pot == tmsend.tm_mon)
			{
				currYearFlag = 1;
				break;
			}
			if(pot > tmsend.tm_mon)
			{
				tmsend.tm_mon = pot;
#ifdef DEBUG				
				cout<<"111111111tmsend.tm_mon is "<<tmsend.tm_mon <<endl<<std::flush;
#endif					
				currYearFlag = 1;
				modifyFlag =1;
				break;
			}
		}
		
		if( currYearFlag == 0)
		{
			tmsend.tm_year++;
			tmsend.tm_mon = atoi(vnum[0].c_str())-1;
			modifyFlag =1;
		}
		vnum.clear();
		vector<string>().swap(vnum);
				
	}
#ifdef DEBUG	
	cout<<"tmsend.tm_year is "<<tmsend.tm_year <<endl<<std::flush;
#endif		
	
	//deal day
	if(modifyFlag==1)
	{
		tmsend.tm_mday = 1;
	}
	else
	{
		tmsend.tm_mday = tmcreate.tm_mday;
	}
	
	int maxDay = getMaxmday(tmsend.tm_year+1900, tmsend.tm_mon);
	if(vformat[2].compare("*")==0)
	{
#ifdef DEBUG		
		cout<<"day * " <<endl<<std::flush;
#endif			
	}
	
	else if(vformat[2].find("*/")==0)
	{
#ifdef DEBUG		
		cout<<"day */ " <<endl<<std::flush;
#endif			
		int div = atoi(vformat[3].substr(2).c_str());
		int mod = (tmsend.tm_mday)%div;
		if(mod != 0)
		{
			if(tmsend.tm_mday+div-mod>maxDay)
			{
				tmsend.tm_mon++;
				tmsend.tm_mday = div;
			}
			else
			{
				tmsend.tm_mday = tmsend.tm_mday+div-mod;
			}
			modifyFlag =1;
		}
	}
	else
	{
#ifdef DEBUG		
		cout<<"day enum " <<endl<<std::flush;
#endif			
		vector<string> vnum =tokenize(vformat[2],",");
#ifdef DEBUG		
		cout<<"vnum.size() is "<<vnum.size()<<endl<<std::flush;
#endif			
		int currMonthFlag = 0;
		for(int i=0;i<vnum.size();i++)
		{
#ifdef DEBUG			
			cout<<"vnum[i] is "<<vnum[i] <<endl<<std::flush;
#endif			
			int pot = atoi(vnum[i].c_str());
#ifdef DEBUG			
			cout<<"pot is "<<pot <<endl<<std::flush;
#endif			
			if(pot == tmsend.tm_mday)
			{
				currMonthFlag = 1;
				break;
			}
			if(pot > tmsend.tm_mday)
			{
				tmsend.tm_mday = pot;
#ifdef DEBUG				
				cout<<"111111111tmsend.tm_mday is "<<tmsend.tm_mday <<endl<<std::flush;
#endif					
				currMonthFlag = 1;
				modifyFlag =1;
				break;
			}
		}
		
		if( currMonthFlag == 0)
		{
			tmsend.tm_mon++;
			tmsend.tm_mday = atoi(vnum[0].c_str());
			modifyFlag =1;
		}
		
		vnum.clear();
		vector<string>().swap(vnum);
				
	}
	
	datetime tmpTime(mktime(&tmsend));
	tmp =tmpTime.duration();
    	localtime_r(&tmp, &tmsend);
#ifdef DEBUG    	
	cout<<"tmsend.tm_wday is "<<tmsend.tm_wday <<endl<<std::flush;
#endif		
	
	//deal week 
	if(vformat[4].compare("*")==0)
	{
#ifdef DEBUG		
		cout<<"week * " <<endl<<std::flush;
#endif			
	}
	else if(vformat[4].find("*/")==0)
	{
#ifdef DEBUG		
		cout<<"week */ " <<endl<<std::flush;
#endif			
		int div = atoi(vformat[4].substr(2).c_str());
		int mod = (tmsend.tm_wday)%div;
		if(mod != 0)
		{
			tmsend.tm_mday = tmsend.tm_mday+div-mod;
			modifyFlag =1;
		}
	}
	else
	{
#ifdef DEBUG		
		cout<<"week enum " <<endl<<std::flush;
#endif			
		vector<string> vnum =tokenize(vformat[4],",");
		cout<<"vnum.size() is "<<vnum.size()<<endl<<std::flush;
		int currWeekFlag = 0;
		for(int i=0;i<vnum.size();i++)
		{
#ifdef DEBUG			
			cout<<"vnum[i] is "<<vnum[i] <<endl<<std::flush;
#endif				
			int pot = atoi(vnum[i].c_str());
#ifdef DEBUG			
			cout<<"pot is "<<pot <<endl<<std::flush;
#endif				
			if(pot == tmsend.tm_mday)
			{
				currWeekFlag = 1;
				break;
			}
			if(pot > tmsend.tm_wday)
			{
				tmsend.tm_mday = tmsend.tm_mday+pot-tmsend.tm_wday;
#ifdef DEBUG				
				cout<<"111111111tmsend.tm_wday is "<<tmsend.tm_wday <<endl<<std::flush;
#endif					
				currWeekFlag = 1;
				modifyFlag =1;
				break;
			}
		}
		
		if( currWeekFlag == 0)
		{
			tmsend.tm_mday = tmsend.tm_mday - tmsend.tm_wday + 7 + atoi(vnum[0].c_str());
			modifyFlag =1;
		}
		vnum.clear();
		vector<string>().swap(vnum);		
	}
	
	if(modifyFlag==1)
	{
		tmsend.tm_hour = 0;
	}
	else
	{
		tmsend.tm_hour = tmcreate.tm_hour;
	}
	
	//deal hour
	if(vformat[1].compare("*")==0)
	{
#ifdef DEBUG		
		cout<<"hour * " <<endl<<std::flush;
#endif			
	}
	else if(vformat[1].find("*/")==0)
	{
#ifdef DEBUG		
		cout<<"hour */ " <<endl<<std::flush;
#endif			
		int div = atoi(vformat[1].substr(2).c_str());
		int mod = (tmsend.tm_mday)%div;
		if(mod != 0)
		{
			if(tmsend.tm_hour+div-mod>23)
			{
				tmsend.tm_mday++;
				tmsend.tm_hour = div;
			}
			else
			{
				tmsend.tm_hour = tmsend.tm_mday+div-mod;
			}
			modifyFlag =1;
		}
	}
	else
	{
		cout<<"hour enum " <<endl<<std::flush;
		vector<string> vnum =tokenize(vformat[1],",");
		cout<<"vnum.size() is "<<vnum.size()<<endl<<std::flush;
		int currDayFlag = 0;
		for(int i=0;i<vnum.size();i++)
		{
			
			cout<<"vnum[i] is "<<vnum[i] <<endl<<std::flush;
			int pot = atoi(vnum[i].c_str());
			cout<<"pot is "<<pot <<endl<<std::flush;
			if(pot == tmsend.tm_hour)
			{
				currDayFlag = 1;
				break;
			}
			if(pot > tmsend.tm_hour)
			{
				tmsend.tm_hour = pot;
				cout<<"111111111tmsend.tm_hour is "<<tmsend.tm_hour <<endl<<std::flush;
				currDayFlag = 1;
				modifyFlag =1;
				break;
			}
		}
		if( currDayFlag == 0)
		{
			tmsend.tm_mday++;
			tmsend.tm_hour = atoi(vnum[0].c_str());
			modifyFlag =1;
		}
		vnum.clear();
		vector<string>().swap(vnum);
				
	}
	if(modifyFlag==1)
	{
		tmsend.tm_min = 0;
	}
	else
	{
		tmsend.tm_min = tmcreate.tm_min;
	}
	cout<<"tmsend.tm_hour is "<<tmsend.tm_hour <<endl<<std::flush;
	cout<<"tmsend.tm_min is "<<tmsend.tm_min <<endl<<std::flush;
	//deal minute
	if(vformat[0].compare("*")==0)
	{
		cout<<"minute * " <<endl<<std::flush;
	}
	else if(vformat[0].find("*/")==0)
	{
		cout<<"minute */ " <<endl<<std::flush;
		int div = atoi(vformat[0].substr(2).c_str());
		int mod = (tmsend.tm_min)%div;
		if(mod != 0)
		{
			if(tmsend.tm_min+div-mod>59)
			{
				tmsend.tm_hour++;
				tmsend.tm_min = div;
			}
			else
			{
				tmsend.tm_min = tmsend.tm_min+div-mod;
			}
			modifyFlag =1;
		}
	}
	else
	{
		cout<<"hour enum " <<endl<<std::flush;
		vector<string> vnum =tokenize(vformat[0],",");
		cout<<"vnum.size() is "<<vnum.size()<<endl<<std::flush;
		int currHourFlag = 0;
		for(int i=0;i<vnum.size();i++)
		{
			
			cout<<"vnum[i] is "<<vnum[i] <<endl<<std::flush;
			int pot = atoi(vnum[i].c_str());
			cout<<"pot is "<<pot <<endl<<std::flush;
			if(pot == tmsend.tm_min)
			{
				currHourFlag = 1;
				break;
			}
			if(pot > tmsend.tm_min)
			{
				tmsend.tm_min = pot;
				cout<<"111111111tmsend.tm_min is "<<tmsend.tm_min <<endl<<std::flush;
				currHourFlag = 1;
				modifyFlag =1;
				break;
			}
		}
		
		if( currHourFlag == 0)
		{
			tmsend.tm_hour++;
			tmsend.tm_min = atoi(vnum[0].c_str());
			modifyFlag =1;
		}
		
		vnum.clear();
		vector<string>().swap(vnum);
				
	}
	if(modifyFlag==1)
	{
		tmsend.tm_sec = 0;
	}
	else
	{
		tmsend.tm_sec = tmcreate.tm_sec;
	}
    	tmsend.tm_isdst = 0;
#ifdef DEBUG    	
  cout<<"tmsend.tm_year is "<<tmsend.tm_year <<endl<<std::flush;
	cout<<"tmsend.tm_mon is "<<tmsend.tm_mon <<endl<<std::flush;
	cout<<"tmsend.tm_mday is "<<tmsend.tm_mday <<endl<<std::flush;
	cout<<"tmsend.tm_wday is "<<tmsend.tm_wday <<endl<<std::flush;
	cout<<"tmsend.tm_hour is "<<tmsend.tm_hour <<endl<<std::flush;
	cout<<"tmsend.tm_min is "<<tmsend.tm_min <<endl<<std::flush;
	cout<<"tmsend.tm_sec is "<<tmsend.tm_sec <<endl<<std::flush;
#endif		
	datetime sendTime(mktime(&tmsend));

	vformat.clear();
	vector<string>().swap(vformat);
	
	return sendTime;
}

string calStringSendTime(string createTime,string sendSchema)
{
	datetime paraTime(createTime);
	datetime rusultTime = calSendTime(paraTime,sendSchema);
	string tmp;
	rusultTime.iso_string(tmp);
	return tmp;
}

vector<string> tokenize(const string& src, string tok,bool trim, string null_subst)  
{  
	if( src.empty() || tok.empty() ) 
	{
		throw bad_param(__FILE__, __LINE__, 130, "tokenize: empty string"); 
	}
	
	vector<string> v;  
	S_T pre_index = 0, index = 0, len = 0;  
	while((index = src.find(tok, pre_index))!=npos)  
	{  
		if( (len = index-pre_index)!=0 )
		{  
			v.push_back(src.substr(pre_index, len));
		}  
		else if(trim==false)
		{  
			v.push_back(null_subst);  
		}
		pre_index = index+tok.length();  
	}  
	string endstr = src.substr(pre_index);  
	if( trim==false )
	{ 
		v.push_back( endstr.empty()?null_subst:endstr );
	}  
	else
	{ 
		if(!endstr.empty())
		{ 
			v.push_back(endstr);
		}
	}
	return v;
}  
  

struct parseTemplateOut parseTemplate(struct parseTemplateIn *ptIn,struct ucsSubsMsgInfo rusmi)
{
#ifdef DEBUG	
	std::cout << "enter parseTemplate " << "\n"<<std::flush;
#endif		
	struct parseTemplateOut ptOut;
	ptOut.msgType =0;

	return ptOut;
}



int socksms_send(msgSend *sms) //vector<struct smsInfo> usmsvct
{
	cout<<"enter sock_process"<<endl;
	//char ServerIp[15 + 1];
  //char ServerPort[6 + 1]; 
  char IniFileName[128 + 1];   
  char LogDir[128 + 1];
  char sms_begDate[10];/*开始发送短信时间*/
  char sms_endDate[10];/*结束发送短信时间*/
  
  memset(IniFileName,0x00,sizeof(IniFileName));
  memset(&sLogBody,0x00,sizeof(_sLogBody));
  //strcpy(sLogBody.logUser,"EC_TEST");
  //strcpy(sLogBody.logPass,"1234");
  //strcpy(ServerIp,"134.78.42.16");
  //strcpy(ServerPort,"8801");
  strcpy(IniFileName, "smscli.ini");
  InitProgram(IniFileName);
  //cout<<"sLogBody.logUser="<<sLogBody.logUser<<endl<<flush;
  //cout<<"sLogBody.logPass="<<sLogBody.logPass<<endl<<flush;
  //cout<<"ServerIp="<<ServerIp<<endl<<flush;
  //cout<<"ServerPort="<<ServerPort<<endl<<flush;
  fmt.str("");
  fmt<<"sLogBody.logUser="<<sLogBody.logUser<<endl<<"sLogBody.logPass="<<sLogBody.logPass<<endl
  <<"ServerIp="<<ServerIp<<endl<<"ServerPort="<<ServerPort;
 // cout <<"fmt ="<<fmt.str()<<endl;

  strcpy(LogDir,"./log");
  strcpy(sms_begDate,"07");
  strcpy(sms_endDate,"22");	
	int	logtimes = 0;
	int conntimes = 0;
	char	seq[35];
	char	tmp[128];
	int sock;
	struct sockaddr_in sin;
  memset(&sin, 0x00, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(ServerIp);
	sin.sin_port = htons(atoi(ServerPort));
		
	while(1){
		if((sock = socket(AF_INET,SOCK_STREAM,0)) == -1)
		{
			//lfile->write_log("enter sock continue");
			//cout<<"enter sock continue"<<endl;
			continue;
		}
		cout<<"sock="<<sock<<endl;
		if(online_gate == 1){
		  fail_times = 0;
		}		
		if(conntimes >= fail_times){
			close(sock);
      		InitProgram("smscli.ini");
		  if(online_gate != 1){
		  	the_ums_alarm_remind();
		  	conntimes = 0;
			fmt.str("");
			fmt<<"sleep ";
			fmt<<alarm_sleep; 
			//lfile->write_log(fmt.str());
		    	//cout<<"sleep "<<alarm_sleep<<endl;
		  	sleep(alarm_sleep);
		    continue;
		  }
		  
		  return -1;
		}
		if (connect(sock,(const struct sockaddr*)(&sin),sizeof(sin)) < 0)
		{
			conntimes++;
			cout<<"enter connect continue"<<endl;
		  continue;
		}
		fmt.str("");
		fmt<<"finish connect";
		cout<<fmt.str()<<endl;
		cout<<"finish connect"<<endl;
		cout<<"fail_times="<<fail_times<<endl;
		
		if(logtimes >= fail_times){
			close(sock);
      InitProgram("smscli.ini");
		  if(online_gate != 1){
		    the_ums_alarm_remind();
		    logtimes = 0;
		    cout<<"sleep "<<alarm_sleep<<endl;
		  	sleep(alarm_sleep);
		    continue;
		  }
		  //the_send_sms(msgSend *sms);
		  return -2;
		}
		if (logSmsServer(sock) < 0)
		{
			logtimes++;
			if(logtimes >= 3)
			{
				fmt.str("");
				fmt<<"login continue";
				cout <<fmt.str()<<endl;
				sleep(5);
				continue;
			}else
				continue;
		}
	  fmt.str("");
		fmt<<"login in";
		cout <<fmt.str()<<endl;
		break;
  }
  
	
	
	int sms_Limit = 25;
	int send_count=0;
	bool not_sleeped = true;
	timeb one_beg;
  	timeb one_end;
  	memset(report_id,0x00, sizeof(report_id));

		if(not_sleeped){
		  ftime(&one_beg);	
	    //cout<<"begin time now : "<< one_beg.time<<"--"<<one_beg.millitm <<endl<<flush;
	    not_sleeped = false;	
		}
		
		send_count += atoi(sms->sendNum);
	  	fmt.str("");
		fmt<<"send_count= "<<send_count;

	  cout <<fmt.str()<<endl;
		if(send_count > sms_Limit){
		  ftime(&one_end);	
		  //cout<<">= sms_Limit time now: "<< one_end.time<<"--"<<one_end.millitm <<endl<<flush;	
		  //cout<<"time passed : "<<(one_end.time - one_beg.time)*1000 + one_end.millitm - one_beg.millitm <<endl<<flush;			
      int micro_send=(one_end.time - one_beg.time)*1000 + (one_end.millitm - one_beg.millitm);
      fmt.str("");
			fmt<<"micro_send= "<<micro_send;
			cout <<fmt.str()<<endl;
      //cout<<"micro_send="<<micro_send<<endl<<flush;
			if(micro_send < 1000){
				//cout<<"usleep "<<(1000-micro_send)*1000<<endl<<flush;
				fmt.str("");
				fmt<<"usleep "<<(1000-micro_send)*1000;
				cout <<fmt.str()<<endl;
				
				usleep((1000-micro_send)*1000);
				
				//ftime(&one_end);
				//cout<<"after usleep time now: "<< one_end.time<<"--"<<one_end.millitm <<endl<<flush;	
			}
			not_sleeped = true;
			send_count = atoi(sms->sendNum);
			//cout<<"send num = "<<send_count<<endl;
		}else {
		  //cout<<"send num = "<<send_count<<endl;
		}
		
		int	reval=0;
	  	strcpy(sResult,"");
		strcpy(sFlag,"2");
		reval = SubmitBusi(sock,sms);	
		
		struct smsgNum LmsgNum;
	  	LmsgNum.reval = reval;
	  
	  	LmsgNum.sFlag = atoi(sFlag);
		umsgNum.push_back(LmsgNum);
	
  	disConServer(sock);
	close(sock);
	return 0;
}

int logSmsServer(int fd)
{
#ifdef DEBUG	
	cout<<"enter logSmsServer"<<endl;
#endif	
	int	reval;
	int	CurPos = 0;
	char	SendBuff[1024 + 1];//def.h
	char	tmp[128];

	memset(SendBuff,0x00,sizeof(SendBuff));
	memset(&sMsgHead,0x00,sizeof(_sMsgHead));
	memset(&sResBody,0x00,sizeof(_sResBody));

	sMsgHead.MsgLength = htonl(BIND_PACK_LEN); //61
	sMsgHead.MsgComId = htonl(SGIP_BIND); //0x1
	MakeSeq(&sMsgHead.Seq1,&sMsgHead.Seq2,&sMsgHead.Seq3,0);
	//*seq1 = htonl(3021021043); //3021021043
	//*seq2 = htonl(atoi(localtime( &time(NULL))));
	//*seq3 = htonl(atoi(usmsvct[i].serial));	
	sLogBody.logType = (char)SPSMG; //1
	
	CurPos = 0;
	memcpy(SendBuff + CurPos,&(sMsgHead.MsgLength),sizeof(sMsgHead.MsgLength));
	CurPos += sizeof(sMsgHead.MsgLength);
	memcpy(SendBuff + CurPos,&(sMsgHead.MsgComId),sizeof(sMsgHead.MsgComId));
	CurPos += sizeof(sMsgHead.MsgComId);
	memcpy(SendBuff + CurPos,&(sMsgHead.Seq1),sizeof(sMsgHead.Seq1));
	CurPos += sizeof(sMsgHead.Seq1);
	memcpy(SendBuff + CurPos,&(sMsgHead.Seq2),sizeof(sMsgHead.Seq2));
	CurPos += sizeof(sMsgHead.Seq2);
	memcpy(SendBuff + CurPos,&(sMsgHead.Seq3),sizeof(sMsgHead.Seq3));
	CurPos += sizeof(sMsgHead.Seq3);
	memcpy(SendBuff + CurPos,&(sLogBody.logType),sizeof(sLogBody.logType));
	CurPos += sizeof(sLogBody.logType);
	memcpy(SendBuff + CurPos,sLogBody.logUser,LOG_NAME_LEN);
	CurPos += LOG_NAME_LEN;
	memcpy(SendBuff + CurPos,sLogBody.logPass,LOG_PASS_LEN);
	CurPos += LOG_PASS_LEN;
	memcpy(SendBuff + CurPos,sLogBody.reserve,RES_LEN);
	SendBuff[ntohl(sMsgHead.MsgLength)] = '\0';
	reval = send_data(fd,SendBuff,ntohl(sMsgHead.MsgLength),0);
	fmt.str("");
	fmt<<"login send_data";
	
	
	cout<<fmt.str() <<endl;
//	cout<< "ygp test 2 "<<SendBuff <<endl;
	if(reval == -1)
	{
		return -1;
	}	

	reval = recv_PackHead(fd,&sMsgHead);
	if(reval == -1)
	{
		return -1;
	}
	else if(sMsgHead.MsgLength > 40)
	{
		return -1;
	}
	else if (sMsgHead.MsgComId != SGIP_BIND_RESP)
	{
		sprintf(tmp,"log return msglen = [%u]",sMsgHead.MsgLength);
		sprintf(tmp,"log return commandid = [%x]",sMsgHead.MsgComId);
		if(sMsgHead.MsgComId == SGIP_UNBIND)
			ExceptionHandle(fd,SGIP_UNBIND_RESP,E_UNBIND);
		else
			ExceptionHandle(fd,SGIP_BIND_RESP,E_MESSAGEID);
		return -1;
	}
	
	reval = recv_PackBody(fd,&sResBody);
	if(reval == -1)
	{
		ExceptionHandle(fd,SGIP_BIND_RESP,E_FORMAT);
		return -1;
	}
	if (sResBody.result != 0)
	{
		sprintf(tmp,"log return result = [%d]",sResBody.result);
		sprintf(tmp,"log return reserve = [%s]",sResBody.reserve);
		return -1;
	}	
	return 0;
}

int disConServer(int fd)
{	
	cout<<"enter disConServer"<<endl;
	int	reval;	
	int	CurPos = 0;	
	char	SendBuff[1024 + 1];//def.h	
	char	tmp[128];  
	static unsigned int i_static_sequences=100000;	
	memset(SendBuff,0x00,sizeof(SendBuff));	
	memset(&sMsgHead,0x00,sizeof(_sMsgHead));		
	sMsgHead.MsgLength = htonl(UNBIND_HEAD_LEN); //20
	sMsgHead.MsgComId = htonl(SGIP_UNBIND); //0x2	
	sMsgHead.Seq1 = htonl(SEQ_1); //3021021043		
	char TmpTime[TIME_LEN + 1]; //32	
	struct tm *sTime = NULL;	
	time_t now;	
	now = time(NULL);	
	sTime = localtime(&now);	
	sprintf(TmpTime,"%d%02d%02d%02d%02d",sTime->tm_mon+1,sTime->tm_mday,sTime->tm_hour,sTime->tm_min,sTime->tm_sec);	
	sMsgHead.Seq2 = htonl(atoi(TmpTime));		
	i_static_sequences++;	
	if(i_static_sequences>=999999)		
	i_static_sequences=100000;		
	sMsgHead.Seq3 = htonl(i_static_sequences);		
	CurPos = 0;	memcpy(SendBuff + CurPos,&(sMsgHead.MsgLength),sizeof(sMsgHead.MsgLength));	
	CurPos += sizeof(sMsgHead.MsgLength);	memcpy(SendBuff + CurPos,&(sMsgHead.MsgComId),sizeof(sMsgHead.MsgComId));	
	CurPos += sizeof(sMsgHead.MsgComId);	memcpy(SendBuff + CurPos,&(sMsgHead.Seq1),sizeof(sMsgHead.Seq1));	
	CurPos += sizeof(sMsgHead.Seq1);	memcpy(SendBuff + CurPos,&(sMsgHead.Seq2),sizeof(sMsgHead.Seq2));	
	CurPos += sizeof(sMsgHead.Seq2);	memcpy(SendBuff + CurPos,&(sMsgHead.Seq3),sizeof(sMsgHead.Seq3));	
	CurPos += sizeof(sMsgHead.Seq3);	SendBuff[ntohl(sMsgHead.MsgLength)] = '\0';	
	reval = send_data(fd,SendBuff,ntohl(sMsgHead.MsgLength),0);	
	fmt.str("");	
	fmt<<"login unbin send data";	
	cout<<fmt.str()<<endl;	
	if(reval == -1)	
	{	
		cout<<"unbind send return -1"<<endl;
	  return -1;	
	}	  
	//cout<<"unbind send:"<<endl;  
	//cout<<"sMsgHead.Seq1="<<sMsgHead.Seq1<<endl;  
	//cout<<"sMsgHead.Seq2="<<sMsgHead.Seq2<<endl;  
	//cout<<"sMsgHead.Seq3="<<sMsgHead.Seq3<<endl;	  
	memset(&sMsgHead,0x00,sizeof(_sMsgHead));	
	reval = recv_PackHead(fd,&sMsgHead);  
	//cout<<"unbind responent:"<<endl;  
	//cout<<"unbind recv packhead reval="<<reval<<endl;
	//cout<<"sMsgHead.Seq1="<<sMsgHead.Seq1<<endl;  
	//cout<<"sMsgHead.Seq2="<<sMsgHead.Seq2<<endl;  
	//cout<<"sMsgHead.Seq3="<<sMsgHead.Seq3<<endl;	
	//cout<<"sMsgHead.MsgLength="<<sMsgHead.MsgLength<<endl;
  //cout<<"sMsgHead.MsgComId="<<sMsgHead.MsgComId<<endl;
	if(reval == -1)	
	{		
	  return -1;	
	}	

	return 0;
}

int  SubmitBusi(int sock, msgSend *sms)
{
#ifdef DEBUG	
	//cout<<"enter SubmitBusi"<<endl<<std::flush;
	//cout<<"SubmitBusi order="<<order<<std::flush;
	//cout<<"SubmitBusi msg="<<usmsvct[order].msgSn<<std::flush;
#endif	
	int	reval;
	int	CurPos = 0;
	char	SendBuff[SEND_BUFF_LEN + 1];
	char	TmpSend[SEND_BUFF_LEN + 1];
	char	tmp[1024];

	memset(SendBuff,0x00,sizeof(SendBuff));
	memset(&sMsgHead,0x00,sizeof(_sMsgHead));
	memset(&sSubmitBody,0x00,sizeof(_sSubmitBody));
	memset(&sResBody,0x00,sizeof(_sResBody));

	sMsgHead.MsgComId = htonl(SGIP_SUBMIT); //0x3
	MakeSeq(&sMsgHead.Seq1,&sMsgHead.Seq2,&sMsgHead.Seq3,atoi(sms->msgSn));
	//*seq1 = htonl(3021021043); //3021021043
	//*seq2 = htonl(atoi(localtime( &time(NULL))));
	//*seq3 = htonl(atoi(usmsvct[i].serial));	

	char	buff[512];
	sprintf(buff,"sMsgHead.Seq3 = %u\n",sMsgHead.Seq3);
	
	FullSubmitBody(sms);
	CurPos = 0;
	memcpy(TmpSend + CurPos,&(sMsgHead.MsgComId),sizeof(sMsgHead.MsgComId));
	CurPos += sizeof(sMsgHead.MsgComId);
	memcpy(TmpSend + CurPos,&(sMsgHead.Seq1),sizeof(sMsgHead.Seq1));
	CurPos += sizeof(sMsgHead.Seq1);
	memcpy(TmpSend + CurPos,&(sMsgHead.Seq2),sizeof(sMsgHead.Seq2));
	CurPos += sizeof(sMsgHead.Seq2);
	memcpy(TmpSend + CurPos,&(sMsgHead.Seq3),sizeof(sMsgHead.Seq3));
	CurPos += sizeof(sMsgHead.Seq3);
	memcpy(TmpSend + CurPos,sSubmitBody.spNumber,SP_NUMBER_LEN);
	CurPos += SP_NUMBER_LEN;
	memcpy(TmpSend + CurPos,sSubmitBody.chargeNumber,CHARGE_NUMBER_LEN);
	CurPos += CHARGE_NUMBER_LEN;
	memcpy(TmpSend + CurPos,&sSubmitBody.userCount,sizeof(sSubmitBody.userCount));
	CurPos += sizeof(sSubmitBody.userCount);
	memcpy(TmpSend + CurPos,sSubmitBody.userNumber,USER_NUMBER_LEN);
	CurPos += USER_NUMBER_LEN;
	memcpy(TmpSend + CurPos,sSubmitBody.corpId,CORP_ID_LEN);
	CurPos += CORP_ID_LEN;
	memcpy(TmpSend + CurPos,sSubmitBody.serviceType,SERVICE_TYPE_LEN);
	CurPos += SERVICE_TYPE_LEN;
	memcpy(TmpSend + CurPos,&sSubmitBody.feeType,sizeof(sSubmitBody.feeType));
	CurPos += sizeof(sSubmitBody.feeType);
	memcpy(TmpSend + CurPos,sSubmitBody.feeValue,FEE_VALUE_LEN);
	CurPos += FEE_VALUE_LEN;
	memcpy(TmpSend + CurPos,sSubmitBody.givenValue,GIVEN_VALUE_LEN);
	CurPos += GIVEN_VALUE_LEN;
	memcpy(TmpSend + CurPos,&sSubmitBody.agentFlag,sizeof(sSubmitBody.agentFlag));
	CurPos += sizeof(sSubmitBody.agentFlag);
	memcpy(TmpSend + CurPos,&sSubmitBody.mtFlag,sizeof(sSubmitBody.mtFlag));
	CurPos += sizeof(sSubmitBody.mtFlag);
	memcpy(TmpSend + CurPos,&sSubmitBody.priority,sizeof(sSubmitBody.priority));
	CurPos += sizeof(sSubmitBody.priority);
	memcpy(TmpSend + CurPos,sSubmitBody.expireTime,EXPIRE_TIME_LEN);
	CurPos += EXPIRE_TIME_LEN;
	memcpy(TmpSend + CurPos,sSubmitBody.scheduleTime,SCHEDULE_TIME_LEN);
	CurPos += SCHEDULE_TIME_LEN;
	memcpy(TmpSend + CurPos,&sSubmitBody.reportFlag,sizeof(sSubmitBody.reportFlag));
	CurPos += sizeof(sSubmitBody.reportFlag);
	memcpy(TmpSend + CurPos,&sSubmitBody.tpPid,sizeof(sSubmitBody.tpPid));
	CurPos += sizeof(sSubmitBody.tpPid);
	memcpy(TmpSend + CurPos,&sSubmitBody.tpUdhi,sizeof(sSubmitBody.tpUdhi));
	CurPos += sizeof(sSubmitBody.tpUdhi);
	memcpy(TmpSend + CurPos,&sSubmitBody.msgCoding,sizeof(sSubmitBody.msgCoding));
	CurPos += sizeof(sSubmitBody.msgCoding);
	memcpy(TmpSend + CurPos,&sSubmitBody.msgType,sizeof(sSubmitBody.msgType));
	CurPos += sizeof(sSubmitBody.msgType);
	memcpy(TmpSend + CurPos,&sSubmitBody.msgLength,sizeof(sSubmitBody.msgLength));
	CurPos += sizeof(sSubmitBody.msgLength);
	memcpy(TmpSend + CurPos,sSubmitBody.msgContent,ntohl(sSubmitBody.msgLength));
	//WriteLog(sSubmitBody.msgContent,LOG_ERR);
	
	CurPos += ntohl(sSubmitBody.msgLength);
	memcpy(TmpSend + CurPos,sSubmitBody.reserve,RES_LEN);
	CurPos += RES_LEN;
	sMsgHead.MsgLength = htonl(CurPos + sizeof(sMsgHead.MsgLength));
	
	/*make SendBuff*/
	memcpy(SendBuff,&sMsgHead.MsgLength,sizeof(sMsgHead.MsgLength));
	memcpy(SendBuff + sizeof(sMsgHead.MsgLength),TmpSend,CurPos);
	SendBuff[CurPos + sizeof(sMsgHead.MsgLength)] = '\0';

	//cout <<"ygp test SendBuff ="<<SendBuff <<endl;
	
	reval = send_data(sock,SendBuff,ntohl(sMsgHead.MsgLength),0);

  	fmt.str("");
 	fmt<<"发送 submit send_data message_id="<<sms->msgSn;
 
 	cout << fmt.str() <<endl;
	if(reval == -1){
		//WriteLog("send Submit Info error",LOG_ERR);
		fmt.str("");
		fmt<<"error send_data";
		cout << fmt.str() <<endl;
		return -9;
	}	

	reval = recv_PackHead(sock,&sMsgHead);
  
	if(reval == -1){
		ExceptionHandle(sock,SGIP_SUBMIT_RESP,E_FORMAT);
		strcpy(sms->status,"3");
		fmt.str("");
		fmt<<"error recv_PackHead";
		cout << fmt.str() <<endl;		
		return -9;
	}else if (sMsgHead.MsgComId != SGIP_SUBMIT_RESP){
		sprintf(tmp,"Submit Info return commandid = [%x]",sMsgHead.MsgComId);
		
		if(sMsgHead.MsgComId == SGIP_UNBIND)
			ExceptionHandle(sock,SGIP_UNBIND_RESP,E_UNBIND);
		else
			ExceptionHandle(sock,SGIP_SUBMIT_RESP,E_MESSAGEID);
		strcpy(sms->status,"1");	
		return -1;
	}
	sprintf(sms->report_id,"%u",sMsgHead.Seq3);	

	reval = recv_PackBody(sock,&sResBody);	
	if(reval == -1)
	{
		ExceptionHandle(sock,SGIP_SUBMIT_RESP,E_FORMAT);
		strcpy(sms->status,"3");
		fmt.str("");
		fmt<<"error recv_PackBody";
		cout << fmt.str() <<endl;			
		return -9;
	}
	else
	{
		sprintf(tmp,"Submit Info return result = [%d]",sResBody.result);
		if (sResBody.result == 0)
		{
			
			strcpy(sms->status,"6");
			strcpy(sms->resultStr,"sucess send");
		}
		else 
		{	
			 
			strcpy(sms->status,"5");
			 
			sprintf(sms->resultStr,"发送失败,失败错误码为:%d",sResBody.result);
			return -9;
		}
	}
	return 0;
}

void FullSubmitBody( msgSend *sms)
{
#ifdef DEBUG	
	//cout<<"enter FullSubmitBody"<<endl<<std::flush;
	//cout<<"FullSubmitBody i="<<i<<endl<<std::flush;
	//cout<<"FullSubmitBody msg="<<usmsvct[i].msgSn<<endl<<std::flush;
#endif		
	char tmpDev[USER_NUMBER_LEN + 1];//21
	char buff[512];
	
	sprintf(tmpDev,"86%s",sms->msisdn);
	cout<<"msisdn="<<tmpDev<<endl;
	/*
	if(strcmp(usmsvct[i].flag,SUCCESS) == 0)
		strcpy(sSubmitBody.msgContent,REG_SUCCESS);
	else if(strcmp(usmsvct[i].flag,FAIL_2) == 0)
		strcpy(sSubmitBody.msgContent,REG_FAIL_2);
	else if(strcmp(usmsvct[i].flag,FAIL_1) == 0)
		strcpy(sSubmitBody.msgContent,REG_FAIL_1);
	else
		strcpy(sSubmitBody.msgContent,usmsvct[i].message);	*/
	strcpy(sSubmitBody.msgContent,sms->msgData);
			
	sSubmitBody.msgLength = htonl(strlen(sSubmitBody.msgContent));
	//memcpy(sSubmitBody.spNumber,SP_NUMBER,SP_NUMBER_LEN);
	memcpy(sSubmitBody.spNumber, sms->reserved1, SP_NUMBER_LEN);
	sSubmitBody.userCount = (char)USER_COUNT;
	memcpy(sSubmitBody.userNumber,tmpDev,USER_NUMBER_LEN);
	memcpy(sSubmitBody.corpId,CORP_ID,CORP_ID_LEN);
	//memcpy(sSubmitBody.serviceType,usmsvct[i].service_type,SERVICE_TYPE_LEN);
	memcpy(sSubmitBody.serviceType,"10011",SERVICE_TYPE_LEN);
	//memcpy(sSubmitBody.feeValue,usmsvct[i].charge,FEE_VALUE_LEN);
	memcpy(sSubmitBody.feeValue,"0",FEE_VALUE_LEN);
	memcpy(sSubmitBody.chargeNumber,tmpDev,USER_NUMBER_LEN);

	//sSubmitBody.feeType = usmsvct[i].fee_type - 0x30 ;
	sSubmitBody.feeType = '1' - 0x30 ;
	//sSubmitBody.mtFlag = usmsvct[i].MOMTflag - 0x30 ;
	sSubmitBody.mtFlag = '2' - 0x30 ;
	//sSubmitBody.reportFlag = usmsvct[i].report_flag - 0x30;
	sSubmitBody.reportFlag = '1' - 0x30;

	memcpy(sSubmitBody.givenValue,GIVEN_VALUE,GIVEN_VALUE_LEN);
	sSubmitBody.agentFlag = (char)AGENT_FLAG;
	sSubmitBody.priority = (char)PRIORITY_FLAG;
	sSubmitBody.tpPid = (char)TP_PID;
	sSubmitBody.tpUdhi = (char)TP_UDHI;
	sSubmitBody.msgCoding = (char)MSG_CODING;
	sSubmitBody.msgType = (char)MSG_TYPE;
}

void MakeSeq(unsigned int* seq1, unsigned int* seq2,unsigned int* seq3,int i)
{
	char TmpTime[TIME_LEN + 1]; //32
	struct tm *sTime = NULL;
	time_t now;

	*seq1 = htonl(SEQ_1); //3021021043
	now = time(NULL);
	sTime = localtime(&now);
	sprintf(TmpTime,"%d%02d%02d%02d%02d",sTime->tm_mon+1,sTime->tm_mday,sTime->tm_hour,sTime->tm_min,sTime->tm_sec);
	*seq2 = htonl(atoi(TmpTime));
	
	SeqThirdPart = i;
	
	*seq3 = htonl(SeqThirdPart);	
	return;		
}

int recv_PackHead(int sock, _sMsgHead* sMsgHead)
{
	int	reval ;
	char tmp[128];
	/*recv msg total length*/
	reval = recv_data(sock,&(sMsgHead->MsgLength),sizeof(int),0,INT_TYPE);
	if (reval == -1)
	{
		return -1;
	}
	
	/*recv msg command id*/
	reval = recv_data(sock,&(sMsgHead->MsgComId),sizeof(int),0,INT_TYPE);
	if (reval == -1)
	{
		//WriteLog("recv MsgComId error!", LOG_ERR);
		return -1;
	}
	
	/*recv seq1*/
	reval = recv_data(sock,&(sMsgHead->Seq1),sizeof(int),0,INT_TYPE);
	if (reval == -1)
	{
		//WriteLog("recv Seq1 error!", LOG_ERR);
		return -1;
	}
	
	/*recv msg command id*/
	reval = recv_data(sock,&(sMsgHead->Seq2),sizeof(int),0,INT_TYPE);
	if (reval == -1)
	{
		//WriteLog("recv Seq2 error!", LOG_ERR);
		return -1;
	}
	
	/*recv msg command id*/
	reval = recv_data(sock,&(sMsgHead->Seq3),sizeof(int),0,INT_TYPE);
	if (reval == -1)
	{
		//WriteLog("recv Seq3 error!", LOG_ERR);
		return -1;
	}
	return 0;
}


int recv_PackBody(int sock, _sResBody* sResBody)
{
	int	reval;
	
	/*recv Resp Result*/
	reval = recv_data(sock,&(sResBody->result),sizeof(char),0,OTH_TYPE);
	if (reval == -1){
		return -1;
	}	
	/*recv Resp Reserv*/
	reval = recv_data(sock,sResBody->reserve,RES_LEN,0,OTH_TYPE);
	if (reval == -1)
		return -1;
	
	return 0;	
}

int send_data(int s,const char* msg,size_t len,int flags)
{
	int	sendlen = 0;
	int	wantsendlen = 0;
	int	finishlen = 0;
	int	tmpValue = 0;
	char	tmpContent[MSG_CONTENT_LEN + 1];
	char	tmpSend[MSG_CONTENT_LEN + 1];
	char	tmp[128];
	
	memset(tmpSend,0x00,MSG_CONTENT_LEN + 1);
	memcpy(tmpSend,msg,len);
	/*tmpSend[len] = '\0';*/
	memset(tmpContent,0x00,MSG_CONTENT_LEN + 1);
	
	memcpy(tmpContent,msg,len);
	tmpContent[len] = '\0';
	wantsendlen = len;
	for(;;){
		sendlen = send(s,tmpSend,wantsendlen,flags);

		if(sendlen == -1){
			//WriteLog(strerror(errno),LOG_ERR);
			return -1;
		}else if(sendlen != wantsendlen){
			finishlen += sendlen;
			memset(tmpSend,0x00,MSG_CONTENT_LEN + 1);
			memcpy(tmpSend, tmpContent + finishlen,len - finishlen);
			tmpSend[len - finishlen] = '\0';
			wantsendlen = len - finishlen;
			continue;
		}
		sprintf(tmp,"send buff length = [%d]",sendlen);
		break;	
	}
	
	return 0;
}

int recv_data(int s,void* buf,size_t len,int flags,int recvtype)
{
	int	reclen = 0;
	int	wantreclen = 0;
	int	finishlen = 0;
	int	tmpValue = 0;
	char	tmpContent[MSG_CONTENT_LEN + 1];
	char	tmpRecv[MSG_CONTENT_LEN + 1];
	char	tmp[128];
	
	tmpContent[0] = '\0';
	wantreclen = len;
	if(recvtype == INT_TYPE)
	{
		sprintf(tmp,"recv wantreclen length:%d", wantreclen);
		/*WriteLog(tmp, LOG_ERR);*/
		reclen = recv(s,&tmpValue,wantreclen,flags);
		sprintf(tmp,"recv length:%d", reclen);
		/*WriteLog(tmp, LOG_ERR);*/
		
		if (reclen == -1 || reclen != wantreclen) 
		{
			//WriteLog(strerror(errno),LOG_ERR);
			return -1;
		}
		*((unsigned int*)buf) = ntohl(tmpValue);
	}else if(recvtype == OTH_TYPE){
		for(;;){
			reclen = recv(s,tmpRecv,wantreclen,flags);
			if(reclen == -1 || reclen == 0){
				//WriteLog(strerror(errno),LOG_ERR);
				return -1;
			}else if(reclen != wantreclen){
				memcpy(tmpContent + finishlen,tmpRecv,reclen);
				finishlen += reclen;
				tmpContent[finishlen] = '\0';
				wantreclen = len - finishlen;
				continue;
			}
			break;	
		}
		if(len == 1){
			*((int*)buf) = (int)tmpRecv[0];
		}	
		else{
			if(strlen(tmpContent) == 0){
				tmpRecv[len] = '\0';
				strcpy((char*)buf,tmpRecv);
			}	
			else
				strcpy((char*)buf,tmpContent);
		}		
	}
	
	return 0;	
}


int InitProgram(char* filename)
{
	char	TmpOth[128];
	FILE*	fp = NULL;
	
	fp = fopen(filename,"r");
	if (fp == NULL){
		return -1;
	}
	
	if(GetValue(fp,"loguser",TmpOth) == -1){
		return -1;
	}
	strcpy(sLogBody.logUser,TmpOth);

	if(GetValue(fp,"logpass",TmpOth) == -1){
		return -1;
	}
	strcpy(sLogBody.logPass,TmpOth);

	if(GetValue(fp,"serverip",TmpOth) == -1){
		return -1;
	}
	strcpy(ServerIp,TmpOth);	

	if(GetValue(fp,"serverport",TmpOth) == -1){
		return -1;
	}
	strcpy(ServerPort,TmpOth);	
	
	if(GetValue(fp,"failtimes",TmpOth) == -1){ 
		return -1;
	}
	fail_times=atoi(TmpOth);
	if(GetValue(fp,"onlinegate",TmpOth) == -1){ 
		return -1;
	}
	online_gate=atoi(TmpOth);	
	if(GetValue(fp,"alarmsleep",TmpOth) == -1){ 
		return -1;
	}
	alarm_sleep=atoi(TmpOth);	
  /*
	if(GetValue(fp,"logdir",TmpOth) == -1){
		return -1;
	}
	strcpy(LogDir,TmpOth);	

	if(GetValue(fp,"smsbeg",TmpOth) == -1){
		return -1;
	}
	strcpy(sms_begDate,TmpOth);		

	if(GetValue(fp,"smsend",TmpOth) == -1){
		return -1;
	}
	strcpy(sms_endDate,TmpOth);	
	*/
	
	fclose(fp);
	
	return 0;
}

int GetValue(FILE* fp , char* field, char* value)
{
	char	tmp[256];
	char	fname[20];
	char	ffield[20];
	char 	fvalue[96];
	char*	ptmp = NULL;
	
	strcpy(ffield,field);
	fvalue[0] = '\0';
	rewind(fp);
	while(!feof(fp))
	{
		fgets(tmp,127,fp);
		ptmp = strstr(tmp,"=");
		memcpy(fname,tmp,ptmp - tmp);
		fname[ptmp - tmp] = '\0';
		AllTrim(fname);
		ToUpper(fname);
		ToUpper(ffield);
		if(strcmp(fname,ffield) == 0){
			strcpy(fvalue,ptmp + 1);
			break; 
		}	
	}
	if (strlen(fvalue) == 0)
		return -1;
	else{
		AllTrim(fvalue);
		strcpy(value,fvalue);	
	}
	return 0;	
}

void ToUpper(char* value)
{
	int	i;
	int	flag = 0;
	int	len = strlen(value);
	char	tmp[20];
	
	for(i = 0; i < len; i++){
		if(isupper(*(value + i)) == 0){
			flag = 1;
			tmp[i] = (char)((int)(*(value + i)) - 32);
		}
	}
	if(flag == 1){
		tmp[i] = '\0';
		strcpy(value,tmp);
	}	
}

void AllTrim(char* content)
{
	int  cpBeg = 0,cpEnd = 0;
	char *pTmp = NULL;
	char Tmp[512];
	
	if(content == NULL) 
		return;
	cpEnd = strlen(content);
	for(pTmp = content;*pTmp&&isspace(*pTmp);pTmp++)
		cpBeg++;
	for(pTmp = content +  strlen(content) - 1;*pTmp&&isspace(*pTmp);pTmp--)
		cpEnd--;
	memcpy(Tmp,content+cpBeg,cpEnd - cpBeg);
	Tmp[cpEnd - cpBeg] = '\0';
	strcpy(content,Tmp);
}

void the_ums_alarm_remind(){
	  cout<<"enter the_ums_alarm_remind"<<endl;
	  string sAlarmPara;
	  vector<string> v_alarm_para;
	  string user_name,passwd,connect_string;
		common::read_db_info(user_name, passwd, connect_string);
    database *db_r = NULL;
    Statement* stmt = NULL;
    ResultSet* rset = NULL;
		try {
			db_r =new database(user_name, passwd, connect_string);
			try
			{
				string sql_para="select distinct msisdn from ums_alarm_para where need_flag=0 \ 
				and send_mode in (0,1)";				
				cout<<"sql_para="<<sql_para<<endl;
        stmt = db_r->create_statement(sql_para);
        rset = stmt->executeQuery();
        while(rset->next()){
        	sAlarmPara = rset->getString(1);
          v_alarm_para.push_back(sAlarmPara);	
        }
        stmt->closeResultSet(rset);
        db_r->terminate_statement(stmt);
        stmt = NULL;
        
        string sql_interface="INSERT INTO sms_interface(smsid,user_nbr,smsinfo)\
        VALUES(sms_send_seq.nextval,:1,'UMS连接华为网关异常告警')";
        stmt = db_r->create_statement(sql_interface);
        cout<<"sql_interface="<<sql_interface<<endl;
        for(int i=0;i<v_alarm_para.size();i++){
        	cout<<"v_alarm_para["<<i<<"]"<<v_alarm_para[i]<<endl;
          stmt->setString(1, v_alarm_para[i]);
          stmt->executeUpdate();
        }
        db_r->commit();
				db_r->terminate_statement(stmt);
				stmt = NULL;
				        
				string sql_log="insert into ums_alarm_log(seq_id,msg_data,sms_status,\
				mail_status)values(alarm_seq_sn.nextval,'UMS连接华为网关异常告警',1,0)";
				cout<<"sql_log="<<sql_log<<endl;
        stmt = db_r->create_statement(sql_log);
				stmt->executeUpdate();
				db_r->commit();
				db_r->terminate_statement(stmt);
				stmt = NULL;
			}
			catch (exception& ex) {
				cout << ex.what() << std::endl;
				fmt.str("");
				fmt << "ex.what() is " << ex.what();
				
				cout <<"the_ums_alarm_remind 1: "<<fmt.str()<<endl;
				if(stmt)
					db_r->terminate_statement(stmt);
				throw bad_param(__FILE__, __LINE__, 125, ex.what());
			}
			delete db_r;
		} catch (exception& ex) {
			delete db_r;			
			fmt.str("");
			fmt << "ex.what() is " << ex.what();
			cout <<"the_ums_alarm_remind 2: "<<fmt.str()<<endl;
			cout << ex.what() << std::endl;			
			throw bad_param(__FILE__, __LINE__, 126, ex.what());
		}	

}

void ExceptionHandle(int sock,unsigned int rmsg_id,int error_no)
{
	int	reval;
	int	msgLen = 0;
	char	SendBuff[SEND_BUFF_LEN + 1];
	
	memset(SendBuff,0,sizeof(SendBuff));
	memset(&sResBody,0,sizeof(_sResBody));
	switch(error_no)
	{
		case E_FORMAT:
		{
			sMsgHead.MsgLength = htonl(RESP_PACK_LEN);
			sMsgHead.MsgComId = htonl(rmsg_id);
			sMsgHead.Seq1 = htonl(0);
			sMsgHead.Seq2 = htonl(0);
			sMsgHead.Seq3 = htonl(0);
			sResBody.result = (char)E_PARMFORMAT;
			break;
		}	
		
		case E_MESSAGEID:
		{
			sMsgHead.MsgLength = htonl(RESP_PACK_LEN);
			sMsgHead.MsgComId = htonl(rmsg_id);
			sMsgHead.Seq1 = htonl(sMsgHead.Seq1);
			sMsgHead.Seq2 = htonl(sMsgHead.Seq2);
			sMsgHead.Seq3 = htonl(sMsgHead.Seq3);
			sResBody.result = (char)E_MSGID;
			break;
		}
		
		case E_MSGLENGTH:
		{
			sMsgHead.MsgLength = htonl(RESP_PACK_LEN);
			sMsgHead.MsgComId = htonl(rmsg_id);
			sMsgHead.Seq1 = htonl(sMsgHead.Seq1);
			sMsgHead.Seq2 = htonl(sMsgHead.Seq2);
			sMsgHead.Seq3 = htonl(sMsgHead.Seq3);
			sResBody.result = (char)E_LENGTH;
			break;
		}
		
		case E_UNBIND:
		{
			sMsgHead.MsgLength = htonl(RESP_PACK_LEN - 9);
			sMsgHead.MsgComId = htonl(rmsg_id);
			sMsgHead.Seq1 = htonl(sMsgHead.Seq1);
			sMsgHead.Seq2 = htonl(sMsgHead.Seq2);
			sMsgHead.Seq3 = htonl(sMsgHead.Seq3);
			break;
		}	
		
		default:
		{
			sMsgHead.MsgLength = htonl(RESP_PACK_LEN);
			sMsgHead.MsgComId = htonl(rmsg_id);
			sMsgHead.Seq1 = htonl(sMsgHead.Seq1);
			sMsgHead.Seq2 = htonl(sMsgHead.Seq2);
			sMsgHead.Seq3 = htonl(sMsgHead.Seq3);
			sResBody.result = (char)E_SYS;		
		}		
	}
	
	if (error_no ==  E_UNBIND)
	{
		memcpy(SendBuff,&(sMsgHead.MsgLength),sizeof(sMsgHead.MsgLength));
		msgLen += sizeof(sMsgHead.MsgLength);
		memcpy(SendBuff + msgLen,&(sMsgHead.MsgComId),sizeof(sMsgHead.MsgComId));
		msgLen += sizeof(sMsgHead.MsgComId);
		memcpy(SendBuff + msgLen,&(sMsgHead.Seq1),sizeof(sMsgHead.Seq1));
		msgLen += sizeof(sMsgHead.Seq1);
		memcpy(SendBuff + msgLen,&(sMsgHead.Seq2),sizeof(sMsgHead.Seq2));
		msgLen += sizeof(sMsgHead.Seq2);
		memcpy(SendBuff + msgLen ,&(sMsgHead.Seq3),sizeof(sMsgHead.Seq3));
		msgLen += sizeof(sMsgHead.Seq3);
	}
	else
	{
		memcpy(SendBuff,&(sMsgHead.MsgLength),sizeof(sMsgHead.MsgLength));
		msgLen += sizeof(sMsgHead.MsgLength);
		memcpy(SendBuff + msgLen,&(sMsgHead.MsgComId),sizeof(sMsgHead.MsgComId));
		msgLen += sizeof(sMsgHead.MsgComId);
		memcpy(SendBuff + msgLen,&(sMsgHead.Seq1),sizeof(sMsgHead.Seq1));
		msgLen += sizeof(sMsgHead.Seq1);
		memcpy(SendBuff + msgLen,&(sMsgHead.Seq2),sizeof(sMsgHead.Seq2));
		msgLen += sizeof(sMsgHead.Seq2);
		memcpy(SendBuff + msgLen,&(sMsgHead.Seq3),sizeof(sMsgHead.Seq3));
		msgLen += sizeof(sMsgHead.Seq3);
		memcpy(SendBuff + msgLen,&(sResBody.result),sizeof(sResBody.result));
		msgLen += sizeof(sResBody.result);
		memcpy(SendBuff + msgLen ,sResBody.reserve,RES_LEN);
		msgLen += RES_LEN;
	}
	reval = send_data(sock,SendBuff,ntohl(sMsgHead.MsgLength),0);
	fmt.str("");
	fmt<<"exceptionhandle send_data";
	cout<<fmt.str()<<endl;
	if(reval == -1)
	{
		return ;
	}	
}



