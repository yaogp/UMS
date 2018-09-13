#include <iostream>
#include <curl/curl.h>
#include<ctime>
#include<unistd.h>
#include "database.h"
#include "common.h"
#include "smgsendcli.h"
#include<iconv.h>
#include "socksend.h"
#include <sstream>

using namespace std;
using namespace util;

static bool b_ctrl_c = false;
void ctrl_c(int sig)
{
	cout<<"*****************enter ctrl_c"<<endl;
	b_ctrl_c = true;
}

void DB_connect(){
	while(true){
	  string user_name,passwd,connect_string;
	  common::read_db_info(user_name, passwd, connect_string);			
		try{
			db =new database(user_name, passwd, connect_string);
		}catch (exception& ex){
			cout << ex.what() << std::endl;
			if(db)
			  delete db;
			cout<<"error DB_connect!!!! sleep 5 reconnect***"<<endl;
			sleep(5);	
			continue;		
		}
		break;
  }
}

void DB_disconnect(){
  if(db)
		delete db;
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
bool checkSendTime(string strTime,vector<ruleSendTime> rsTimeVct)
{
	bool flag = false;
	string time =strTime.substr(8);
#ifdef DEBUG		
//	cout << " current time: " << time << std::endl;
#endif
	vector<ruleSendTime>::const_iterator iter;
	//between beginTime and endTime
	for (iter = rsTimeVct.begin(); iter != rsTimeVct.end(); ++iter) {
		if(strcmp(time.c_str(),iter->beginTime)*strcmp(time.c_str(),iter->endTime)<=0)
		{
			flag =true;
			break;
		}
	}
	return flag;
}




void ivr_control_send()
{
	cout<<"*********usleep iunit"<<endl;
	int iunit = st_conf.limit_num;
	iunit =(int)(1000000/iunit);/*每条短信的微秒数*/
	cout<<"*********usleep iunit"<<iunit<<endl;
	usleep(iunit);/*1秒=1000000微秒*/
}


void update_send_log (msgSend *sms) //smsCounts
	{

		 cout<<"enter update_send_log"	<<endl;
	
	  
			Statement* istmt = NULL;
			Statement* dstmt = NULL;
			datetime dt(time(0));
			string tTime;
			dt.iso_string(tTime);

			
		 
			string logTableName = "msg_send_log_"+tTime.substr(0,6);

			try
			{

				string sql_istmt="INSERT INTO "+ logTableName +"(msg_sn, send_code, template_id, subscription_id, msisdn, msg_data,\ 
				send_num, priority, create_time, schedule_time, expire_time, city_id, county_id, staff_id,\
				send_status, send_time, fail_reason, message, reserved1, reserved2, reserved3, send_flag, report_id)\
				VALUES(:1, :2, :3, :4, :5, :6, :7,:8, to_date(:9,'yyyymmddhh24miss'), to_date(:10,'yyyymmddhh24miss'), to_date(:11,'yyyymmddhh24miss'), :12, :13, :14, :15, sysdate, :16, :17, :18, :19, :20, :21, :22)";

				istmt = db->create_statement(sql_istmt);

			//cout<<"sql_istmt="<<sql_istmt<<endl;

					istmt->setString(1,sms->msgSn);
										//cout <<"sms->msgSn"<<sms->msgSn<<endl;
										istmt->setString(2,sms->sendCode);
										//cout <<"sms->sendCode"<<sms->sendCode<<endl;
										istmt->setString(3,sms->templateId);
										//cout <<"sms->templateId"<<sms->templateId<<endl;
										istmt->setString(4,sms->subscriptionId);
										//cout <<"sms->subscriptionId"<<sms->subscriptionId<<endl;
										istmt->setString(5,sms->msisdn);
										//cout <<"sms->msisdn"<<sms->msisdn<<endl;
										istmt->setString(6,sms->msgData);
										//cout <<"sms->msgData"<<sms->msgData<<endl;
										istmt->setString(7,sms->sendNum);
										//cout <<"sms->sendNum"<<sms->sendNum<<endl;
										istmt->setString(8,sms->priority);
										//cout <<"sms->priority"<<sms->priority<<endl;
										istmt->setString(9,sms->createTime);
										//cout <<"sms->createTime"<<sms->createTime<<endl;
										istmt->setString(10,sms->scheduleTime);
										//cout <<"sms->scheduleTime"<<sms->scheduleTime<<endl;
										istmt->setString(11,sms->expireTime);
								//cout <<"sms->expireTime"<<sms->expireTime<<endl;
										istmt->setString(12,sms->cityId);
										//cout <<"sms->cityId"<<sms->cityId<<endl;
										istmt->setString(13,sms->countyId);
										//cout <<"sms->countyId"<<sms->countyId<<endl;
										istmt->setString(14,sms->staffId);
										//cout <<"sms->staffId"<<sms->staffId<<endl;
										istmt->setString(15, sms->status);
										//cout <<"sms->status"<<sms->status<<endl;
										istmt->setString(16, sms->resultCode);
										//cout <<"sms->resultCode"<<sms->resultCode<<endl;
										istmt->setString(17,sms->resultStr);
										//cout <<"sms->resultStr"<<sms->resultStr<<endl;
										istmt->setString(18,sms->reserved1);
										//cout <<"sms->reserved1"<<sms->reserved1<<endl;
										istmt->setString(19,sms->reserved2);
										//cout <<"sms->reserved2"<<sms->reserved2<<endl;
										istmt->setString(20,sms->reserved3);
										//cout <<"sms->reserved3"<<sms->reserved3<<endl;
										istmt->setString(21,sms->send_flag);
										//cout <<"sms->send_flag"<<sms->send_flag<<endl;
										istmt->setString(22,sms->report_id);
										//cout <<"sms->report_id"<<sms->report_id<<endl;


				istmt->executeUpdate();

			cout << " insert into  msg send table" << "\n"<<std::flush;

			
			
			
			string sql_dstmt="DELETE FROM "+ task_para.tableName+" WHERE msg_sn = :1";

			dstmt = db->create_statement(sql_dstmt);

			dstmt->setString(1, sms->msgSn);
			dstmt->executeUpdate();
			cout << " delete msg send table" << "\n"<<std::flush;

			
				
				 
				db->terminate_statement(istmt);
				db->terminate_statement(dstmt);
				istmt = NULL;
				dstmt = NULL;
				db->commit();
			}catch (exception& ex) {
				throw bad_param(__FILE__, __LINE__, 611, "update_send_log");
				cout <<"error update_send_log "<< ex.what() << std::endl;

				db->rollback();	
				if(istmt)
					istmt = NULL;
				if(dstmt)
					dstmt = NULL;
					db->rollback();	

				throw bad_param(__FILE__, __LINE__, 611, "update_send_log");
				cout <<"error update_send_log "<< ex.what() << std::endl;
				DB_disconnect();
				DB_connect();
				exit(1); 
			}
			 
		 
	}

void httpsms_send(msgSend *sms)
{
	string message;
	CURL *curl;
   	CURLcode res;
	int resCount=0;
	curl = curl_easy_init();
	if (curl)
    	{	
		char postfields[4000];
		struct curl_slist *http_header = NULL;
		http_header = curl_slist_append(http_header, "Accept: *");
		http_header = curl_slist_append(http_header, "Content-Type:application/x-www-form-urlencoded;charset=GBK");  

		curl_easy_setopt(curl, CURLOPT_POSTQUOTE, http_header);

		string content;content.clear();
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &process_data);
		sprintf(postfields,"&SpCode=240546&LoginName=admin1&Password=admin1!&MessageContent=%s&UserNumber=%s&SerialNumber=%s&ScheduleTime=%s&ExtendAccessNum=&f=1", sms->msgData,sms->msisdn,sms->msgSn,sms->scheduleTime);
		//cout <<"postfields="<<postfields<<endl;					
		//lfile->write_log(postfields);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
    		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, &postfields); // 指定post内容
    		curl_easy_setopt(curl, CURLOPT_URL, "http://134.78.4.50:8899/sms/Api/Send.do");   // 指定url

        	res = curl_easy_perform(curl);

			if (res != CURLE_OK)
			{
				cout <<"res ="<<res <<endl; 				
				//lfile->write_log("res : " + res);
				//exit(1);
				strcpy(sms->status,"99");
               			curl_slist_free_all(http_header);
               			curl_easy_cleanup(curl);

			}
			
			std::size_t found = content.find("error_code"); 
			if (found!=std::string::npos)  
			{
				found += 13;
				int j=0;
				//cout <<"found =" <<found<<endl;
				//lfile->write_log("found : " + found);
				while (content[found]!='"' && j <3){
					sms->status[j] =content[found];
					j++;
					found ++;
				}
			}	
			//cout <<"status" <<sms->status<<endl;
			fmt.str("");
			fmt <<"status : ";
			fmt <<sms->status;
			//lfile->write_log(fmt.str());
			curl_slist_free_all(http_header);
       		 	curl_easy_cleanup(curl);
			//cout <<"content"<<content<<endl;
			//lfile->write_log("content : " + content);
			
   			 	}
			 else 
			cout<<"init curl  fail "<<endl;
			//lfile->write_log("httpsms_send : init curl  fail");

}





void smg_send(){
 	lfile->write_log("enter smg_send");

	
	char getResult[4096];
	for(int i=0;i<sms_count;i++){

		lfile->write_log(" send begin ");


		if(atoi(msar[i].send_flag) != 0)continue;
			
		std::vector<int>::iterator it;
		long num_head=(msar[i].msisdn[0]-'0')*100+(msar[i].msisdn[1]-'0')*10+(msar[i].msisdn[2]-'0');
		
		cout <<"num_head"<<num_head<<endl;
		it= find( rule_dealer.begin(), rule_dealer.end(), num_head);

//		 cout <<"it =" <<*it<<endl;
		if(it != rule_dealer.end())
			{
			if (socksms_send(&msar[i]) <0)
				{				
				lfile->write_log("sock error");
				//cout <<"sock error"<<endl;
				exit(1);
				}
			}				 	
	     else
	     	{
	
				lfile->write_log("httpsms_send");	
		 	  	httpsms_send(&msar[i]);
			 
	     	}
		  update_send_log(&msar[i]);
				
		}
}






void get_send_task(){

  while(true){
  	Statement* stmt = NULL;
		try
		{
			string sql_task="SELECT msg_table_name, send_table_name, parellel_num, time_rule_id, delay_time\ 
			FROM code_ums_send_task WHERE group_id = :1";
			cout<<"sql_task="<<sql_task<<endl;
      		stmt = db->create_statement(sql_task);
			stmt->setString(1, group_id);
			ResultSet *rset = stmt->executeQuery();
			if (!rset->next())
				throw bad_param(__FILE__, __LINE__, 126, "code_ums_send_task");
			task_para.tableName = rset->getString(1);
			task_para.sendTableName = rset->getString(2);
			task_para.parellelNum = rset->getInt(3);
			task_para.timeRuleId = rset->getInt(4);
			task_para.delayTime = rset->getInt(5);
			
			stmt->closeResultSet(rset);	
			db->terminate_statement(stmt);
			stmt = NULL;
			cout <<"task_para.sendTableName ="<<task_para.sendTableName <<endl;
		}catch (exception& ex) {
			if(stmt)
				db->terminate_statement(stmt);
			throw bad_param(__FILE__, __LINE__, 611, "get_send_task");
			cout <<"error get_send_task "<< ex.what() << std::endl;
			DB_disconnect();
			DB_connect();
			continue;
		}
		break;
	}
	cout <<__FUNCTION__<<" is end "<<endl;
}

void get_send_conf(){

  while(true){
	  char param_name[32+1];
	  char param_value[512+1];
	  memset(param_name,0x00,sizeof(param_name));
	  memset(param_value,0x00,sizeof(param_value));
    	Statement* stmt = NULL;  	
		try
		{
			string sql_conf="SELECT param_name,param_value FROM code_ums_send_conf WHERE group_id = :1";
	    		stmt = db->create_statement(sql_conf);
			stmt->setString(1, group_id);
			ResultSet *rset = stmt->executeQuery();
			while(rset->next()){
				strcpy(param_name,rset->getString(1).c_str());
				strcpy(param_value,rset->getString(2).c_str());
			  AllTrim(param_name);
			  AllTrim(param_value);
			  int len = strlen(param_value);	      
	      if((strcmp(param_name,"src_dir") == 0)||(strcmp(param_name,"SRC_DIR") == 0))
	      {
	      	if(param_value[len-1] != '/'){
	      		param_value[len] = '/';
	      		param_value[len+1] = '\0';
	      	}
		      strcpy(st_conf.src_dir,param_value);
	      }
	      if((strcmp(param_name,"dst_dir") == 0)||(strcmp(param_name,"DST_DIR") == 0))
	      {
	      	if(param_value[len-1] != '/'){
	      		param_value[len] = '/';
	      		param_value[len+1] = '\0';
	      	}
		      strcpy(st_conf.dst_dir,param_value);
	      }
	      if((strcmp(param_name,"inv_dir") == 0)||(strcmp(param_name,"INV_DIR") == 0))
	      {
	      	if(param_value[len-1] != '/'){
	      		param_value[len] = '/';
	      		param_value[len+1] = '\0';
	      	}
		      strcpy(st_conf.inv_dir,param_value);
	      }
	      if((strcmp(param_name,"bak_dir") == 0)||(strcmp(param_name,"BAK_DIR") == 0))
	      {
	      	if(param_value[len-1] != '/'){
	      		param_value[len] = '/';
	      		param_value[len+1] = '\0';
	      	}
		      strcpy(st_conf.bak_dir,param_value);
	      }
	      if((strcmp(param_name,"err_dir") == 0)||(strcmp(param_name,"ERR_DIR") == 0))
	      {
	      	if(param_value[len-1] != '/'){
	      		param_value[len] = '/';
	      		param_value[len+1] = '\0';
	      	}
		      strcpy(st_conf.err_dir,param_value);
	      }   
		  if((strcmp(param_name,"log_path") == 0)||(strcmp(param_name,"LOG_PATH") == 0))
	      {
	      	if(param_value[len-1] != '/'){
	      		param_value[len] = '/';
	      		param_value[len+1] = '\0';
	      	}
		      strcpy(st_conf.log_path,param_value);
	      } 
	      if((strcmp(param_name,"pattern") == 0)||(strcmp(param_name,"PATTERN") == 0))
	      {
		      strcpy(st_conf.pattern,param_value);
	      }
	      if((strcmp(param_name,"limit_num") == 0)||(strcmp(param_name,"LIMIT_NUM") == 0))
	      {
		      st_conf.limit_num=atoi(param_value);
	      } 
		  if((strcmp(param_name,"commit_num") == 0)||(strcmp(param_name,"commit_num") == 0))
	      {
		      st_conf.commit_num=atoi(param_value);
	      } 
		  }
			stmt->closeResultSet(rset);	
			db->terminate_statement(stmt);
			stmt = NULL;
		}
		catch (exception& ex) {
			if(stmt)
				db->terminate_statement(stmt);
			throw bad_param(__FILE__, __LINE__, 691, "get_send_conf");
			cout <<"error get_send_conf "<< ex.what() << std::endl;
			DB_disconnect();
			DB_connect();
			continue;			
		}
		break;
	}
}

void load_dealer_code(){

	rule_dealer.clear();

	 Statement* stmt = NULL;

	 try
	 	{
			string sql_rule="select num_head from rule_dealer_code   where dealer_code =1 ";
			//cout<<"load_dealer_code sql is :" <<sql_rule <<endl;
			stmt = db->create_statement(sql_rule);
			ResultSet *rset = stmt->executeQuery();
			while (rset->next())
				{
				int num_head = rset->getInt(1);
				rule_dealer.push_back(num_head);

				}
				
	 			 stmt->closeResultSet(rset);
				db->terminate_statement(stmt);
				stmt = NULL;
			}catch (exception& ex) {
				cout <<"error get_send_time "<< ex.what() << std::endl;
				if(stmt)
					db->terminate_statement(stmt);
				DB_disconnect();
				DB_connect();
				exit(1);
				
			}

}
	

int  get_sendsmg(int pnum){
	
		
		
		
		
		
	    Statement* stmt = NULL;

		memset(&msar,0x00,sizeof(msgSend));
		sms_count =0;
	std::ostringstream oss;
	



	//cout <<"get_sendsmg begin"<<endl;
	//cout <<"get_sendsmg commit_num ="<<st_conf.commit_num <<endl;
		
			try
			{					
			 
			oss <<"SELECT msg_sn, send_flag, msisdn, msg_data, access_code,priority,to_char(create_date,'yyyymmddhh24miss'), to_char(nvl("
    			<<"audit_date,eff_date),'yyyymmddhh24miss'),to_char(eff_date,'yyyymmddhh24miss'), to_char(exp_date,'yyyymmddhh24miss'),to_number(lengthb(msg_data)) FROM "
    			<< task_para.tableName 
    			<<" where rownum < :1 and exp_date >=sysdate and mod(to_number(msisdn), "
    			<< task_para.parellelNum 
   			<<") =" 
  			  	<< pnum; 
			
				string sql_rule=oss.str();
				lfile->write_log("get_sendsmg sql_rule=" + sql_rule);
				 
				stmt = db->create_statement(sql_rule);
				stmt->setInt(1, st_conf.commit_num  );
				
				ResultSet *rset = stmt->executeQuery();
				
		//cout <<"get_sendsmg sql exec"<<endl;
				while (rset->next())
				{
					strcpy(msar[sms_count].msgSn,rset->getString(1).c_str());
					strcpy(msar[sms_count].send_flag,rset->getString(2).c_str());
					strcpy(msar[sms_count].msisdn,rset->getString(3).c_str());
					strcpy(msar[sms_count].msgData,rset->getString(4).c_str());
					strcpy(msar[sms_count].reserved1,rset->getString(5).c_str());
					strcpy(msar[sms_count].priority,rset->getString(6).c_str());
					strcpy(msar[sms_count].createTime,rset->getString(7).c_str());
					strcpy(msar[sms_count].scheduleTime,rset->getString(8).c_str());
					strcpy(msar[sms_count].eff_time,rset->getString(9).c_str());
					strcpy(msar[sms_count].expireTime,rset->getString(10).c_str());
					int msg_length = rset->getInt(11);

					strcpy(msar[sms_count].sendCode,"0000");   
					strcpy(msar[sms_count].templateId,"900000");  
					strcpy(msar[sms_count].subscriptionId,"900000");  

					strcpy(msar[sms_count].cityId,"A");  
					strcpy(msar[sms_count].countyId,"A00"); 
					strcpy(msar[sms_count].staffId,"5211"); 
					strcpy(msar[sms_count].reserved3,task_para.tableName.c_str() );
					  if(msg_length > 290){
					  strcpy(msar[sms_count].sendNum , "5");
					}else if(msg_length > 250){
					  strcpy(msar[sms_count].sendNum , "4");
					}else if(msg_length > 205){
					   strcpy(msar[sms_count].sendNum , "3");
					}else if(msg_length > 120){
					   strcpy(msar[sms_count].sendNum , "2");
					}else{
					   strcpy(msar[sms_count].sendNum , "1");
					}	

					
					sms_count++;

				}
				stmt->closeResultSet(rset);
				db->terminate_statement(stmt);
				stmt = NULL;
			}catch (exception& ex) {
				oss << "error get_sendsmg " << ex.what() ; 
				lfile->write_log("error get_sendsmg " + oss.str() );
				if(stmt)
					db->terminate_statement(stmt);
				DB_disconnect();
				DB_connect();
				return -1;
				
			}


			 	
			lfile->write_log("sms_count = " + sms_count );
			return sms_count;
}
		


static size_t process_data(void *data, size_t size, size_t nmemb, string &content)
{
    long sizes = size * nmemb;
    string temp;
    temp = string((char*)data,sizes);
    content += temp;
    return sizes;
}


int main(int argc, char *argv[])
{	
	setlocale(LC_ALL,"Zh_CN.UTF-8");
	signal(SIGINT, ctrl_c);
	signal(SIGTERM, ctrl_c);
	if (argc != 2)
	{
		cout << "Usage:" <<"argv[0] ="<< argv[0] << " groupId\n" << std::endl;
		cout << "Usage:" <<"argv[1] ="<< argv[1] << " groupId\n" << std::endl;
		exit(1);
	}
	memset(group_id,0x00,sizeof(group_id));
  strcpy(group_id,argv[1]);
//  cout<<"group_id="<<group_id<<endl; 
  
  DB_connect();
  
  memset(&task_para,0x00,sizeof(task_para));
  get_send_task();
 // cout <<"ygp_1"<<endl; 
  get_send_conf();
//  cout <<"ygp_2"<<endl;
  load_dealer_code();
//  cout <<"ygp_3"<<endl;
  DB_disconnect(); 

  
  int pnum=task_para.parellelNum;
//  cout <<"ygp_4:task_para.parellelNum = "<<task_para.parellelNum<<endl;
  for (int no=0;no<pnum;no++) { 
// cout <<"ygp_5:no = "<<no <<endl;
  int pid=0;
  if((pid=fork()) == 0) {
  bool running = true;
	while(running){	

		DB_connect();

		char log_name[20]={};
		log_file::switch_mode mode;
		mode = util::log_file::mode_day;
		sprintf(log_name,"%s_%d",group_id,no);
		log_name[strlen(log_name)]='\0';
		lfile =new  log_file(mode,st_conf.log_path,log_name );
		lfile->write_log("do busi begin");
			if(get_sendsmg(no)>0)
			{
				smg_send();
			}

		DB_disconnect();	

		sleep(2);
		}
		
	}
  }
  return 0;
 
}

