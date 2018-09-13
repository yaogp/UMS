//#include <occi.h>
//#include <map>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <errno.h>
//#include "database.h"
#include "common.h"
//#include "log_file.h"

#include "smgsendcli.h"







#include "def.h"


using namespace std;

struct parseTemplateIn
{
    int templateId;
    char msgData[4000+1];
    char serviceType[5];
    char brand[20];
};

struct parseTemplateOut
{
    char msgContent[1024];
    int	msgType;
    int mmsTemplateId;
    char emailSubject[128];
};
struct msgTemplate
{
    int templateId;
    char templateRemarks[513];
    char templateText[4001];
    int msgType;
    int msgLimit;
    int mmsTemplateId;
    int msgSplitType;
    char msgSplitStr[11];
    int msgSys;
    char emailSubject[128];
};
struct msgTemplateXss
{
	char operator_id[21];
    int  templateId;
	char templateText[4001];
    char template_desc[129];
	int save_flag;
	char effDate[20+1];
	char expDate[20+1];
    int msgType;
    int msgLimit;
	int msgSplitType;
	char msgSplitStr[11];
};
struct queryRest
{
	int subscriptionId;
};

struct queryResp
{
	int subscriptionId;
	int serviceType;
	int brandId;
	char regionId[1+1];
};

struct sendResp
{
	int successCount;
	int failCount;
};

struct ruleSend
{
	char sendCode[8+1];
	char sendRemark[512+1];
	char accessCode[32+1];
	char gatewayFlag[10+1];
	int priority;
	char effDate[20+1];
	char expDate[20+1];
	int busiTime;
	int gatewayTime;
	int dataControl;
};

struct msgVar
{
	int templateId;
	char varCode[32+1];
	int varOrder;
	int dataType;
	char dataFormat[32+1];
};
struct msgVarXss
{
	char operatorId[20+1];
	int templateId;
	int varOrder;
	char varCode[8+1];
};



struct smsInfo
{
	char msgSn[32+1];
	int templateId;
	int subscriptionId;
	char sendCode[8+1];
	char regionId[2+1];
	char accessCode[32+1];
	int id;
	char msisdn[20+1];
	char flag[10+1];
	char msg[512+1];
	char sts[1+1];
	char getDate[20+1];
	char para1[20+1];
	char para2[20+1];
	char para3[20+1];
	char sendDate[20+1];
	char recv[20+1];
	char msgId[20+1];
	int pri;
	int accId;
	char expireTime[20+1];
	int doneCode;
	char reserved1[24+1];
	char reserved2[24+1];
	char reserved3[24+1];
	int sendFlag;
	int dataControl;
	int busiTime;
	char cityId[2+1];
	char countyId[8+1];
	char staffId[20+1];
  int sendNum;
};

struct mmsInfo
{
	char msgSn[32+1];
	int templateId;
	int subscriptionId;
	char sendCode[8+1];
	char regionId[2+1];
	char accessCode[32+1];
	int id;
	int tmpId;
	char msisdn[20+1];
	char flag[10+1];
	char msg[512+1];
	char param[4000+1];
	char sts[1+1];
	char failReason[500+1];
	char sendDate[20+1];
	char getDate[20+1];
	int accId;
	int pri;
	char para1[20+1];
	char para2[20+1];
	char para3[20+1];
	char recv[20+1];
	int sendFlag;
	char reserved1[24+1];
	char reserved2[24+1];
	char reserved3[24+1];
};

struct emailInfo
{
	char msgSn[32+1];
	int subscriptionId;
	char regionId[2+1];
	char accessCode[32+1];
	int id;
	char msisdn[20+1];
	char receiver[128+1];
	char receiverEmail[64+1];
	char emailSubject[128+1];
	char emailData1[4000+1];
	char emailData2[4000+1];
	char emailData3[4000+1];
	char emailCode[20+1];
	int priority;
	char sendCode[8+1];
	int templateId;
	char createTime[20+1];
	char effTime[20+1];
	char expTime[20+1];
	char para1[24+1];
	char para2[24+1];
	char para3[24+1];
	int sendFlag;
	int dataControl;
};

struct ucsSubsMsgInfo
{
	int notNullFlag;
	int subscriptionId;
	char regionId[2+1];
	char sendCode[8+1];
	int serviceType;
	char serviceNum;
	int sendFlag;
	int isAdSend;
	int sendCycle;
	char sendSchema[20+1];
};

struct umsCodeNum
{
	int seq_id;
	int code_id;
  int systme_id;
  char pace_flag[3+1];
  char pace_day[200+1];
  char start_time[6+1];
  char end_time[6+1];
  int sms_sum;
  int send_num;
};


char	sResult[128];
char 	sFlag[2];

//_sComm	sComm[MAX_COMM_COUNTS];	//100

typedef struct{
	char	logType;
	char	logUser[LOG_NAME_LEN + 1];
	char	logPass[LOG_PASS_LEN + 1];
	char	reserve[RES_LEN + 1];
}_sLogBody;
_sLogBody sLogBody;

typedef struct{
	char	spNumber[SP_NUMBER_LEN + 1];
	char	chargeNumber[CHARGE_NUMBER_LEN + 1];
	char	userCount;
	char	userNumber[USER_NUMBER_LEN + 1];
	char	corpId[CORP_ID_LEN + 1];
	char	serviceType[SERVICE_TYPE_LEN + 1];
	char	feeType;
	char	feeValue[FEE_VALUE_LEN + 1];
	char	givenValue[GIVEN_VALUE_LEN + 1];
	char    agentFlag;
	char	mtFlag;
	char	priority;
	char	expireTime[EXPIRE_TIME_LEN + 1];
	char	scheduleTime[SCHEDULE_TIME_LEN +1];
	char	reportFlag;
	char	tpPid;
	char	tpUdhi;
	char	msgCoding;
	char	msgType;
	int		msgLength;
	char	msgContent[MSG_CONTENT_LEN + 1];
	char	reserve[RES_LEN + 1];
}_sSubmitBody;

typedef struct{
	unsigned int	MsgLength;
	unsigned int	MsgComId;
	unsigned int	Seq1;
	unsigned int	Seq2;
	unsigned int	Seq3;
}_sMsgHead;
_sMsgHead sMsgHead;

typedef struct{
	int	result;
	char	reserve[RES_LEN + 1];
} _sResBody;
_sResBody sResBody;

struct sMsgSn{
 int reval;
 char msg_sn[32+1];
};

struct smsgNum{
 int reval;
 int num;
 int sFlag;
};





	
	struct parseTemplateOut parseTemplate(struct parseTemplateIn *ptIn,struct ucsSubsMsgInfo rusmi);
	struct parseTemplateOut parseTemplateXss(struct parseTemplateIn *ptIn);
	struct ruleSend* getCurrRuleSend(char* sendCode);
	struct msgTemplate* getCurrMsgTemplate(int templateId);
	struct msgTemplateXss* getCurrMsgTemplateXss(int templateId);
	struct ucsSubsMsgInfo query_ucs_subs_msg_info(struct ucsSubsMsgInfo usmi);
	datetime calSendTime(datetime createTime,string sendSchema);
	string calStringSendTime(string createTime,string sendSchema);
	vector<string> tokenize(const string& src, string tok,bool trim=false, string null_subst=""); 


	
	
	

	map<string, string> msg_ad;
	vector<struct ruleSend> rsvct;
	vector<struct msgTemplate> mtvct;
	vector<struct msgTemplateXss> mtxvct;
	vector<struct umsCodeNum> mucnct;
  map<string, string> conn_info;
  vector<struct smsInfo> usmsvct;	
  vector<struct smsInfo> smsvct;
  _sSubmitBody	sSubmitBody;

	int socksms_send(msgSend *sms);
	int logSmsServer(int fd);
	int disConServer(int fd);
	int SubmitBusi(int sock,msgSend *sms);
	void FullSubmitBody(msgSend *sms);
	void MakeSeq(unsigned int* seq1, unsigned int* seq2,unsigned int* seq3,int i);
	int recv_PackHead(int sock, _sMsgHead* sMsgHead);
	int recv_PackBody(int sock, _sResBody* sResBody);
	int send_data(int s,const char* msg,size_t len,int flags);
	int recv_data(int s,void* buf,size_t len,int flags,int recvtype);
	
	int InitProgram(char* filename);
	int GetValue(FILE* fp , char* field, char* value);
	void ToUpper(char* value);
	void AllTrim(char* content);
	void ExceptionHandle(int sock,unsigned int rmsg_id,int error_no);

	void the_ums_alarm_remind();

	string tableName;
	string sendTableName;
	string logTableName;
	int send_num;
	string user_name;
	string passwd;
	string connect_string;
	vector<struct sMsgSn> uMsgsn;
	vector<struct smsgNum> umsgNum;
	char ServerIp[15 + 1];
  char ServerPort[6 + 1];
  char report_id[20000][256];
	//int	sock;

