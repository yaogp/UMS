#ifndef __SMSSENDCLI_H__
	#define __SMSSENDCLI_H__
#include <occi.h>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <sys/timeb.h>
#include "database.h"
#include "scan_file.h"
//#include "log_file.h"


using namespace util;
using namespace std;

#define MAX_COMM_COUNTS		14000

struct ruleSendTime
{
	int timeRuleId;
	int timeOrder;
	char beginTime[6+1];
	char endTime[6+1];
};




 struct task_para_t{
  string tableName;
  string sendTableName;
  int parellelNum;
  int timeRuleId;
  int delayTime;
};
struct task_para_t task_para;


typedef struct _msgSend
{
	char msgSn[32+1];
	char sendCode[8+1];
	char templateId[8+1];
	char subscriptionId[9+1];
	char msisdn[64+1];
	char msgData[4000+1];
	char sendNum[8+1];
	char priority[3+1];
	char createTime[20+1];
	char scheduleTime[20+1];
	char expireTime[20+1];
	char cityId[2+1];
	char countyId[8+1];
	char staffId[20+1];
	char send_flag[1+1];
	char eff_time[20+1];
	char reserved1[24+1];
	char reserved2[24+1];
	char reserved3[24+1];	
	char status[3+1];/*0正常发送，1serviceType找不到匹配，2连接出错*/
	char resultCode[1+1];/*0代表失败, 1代表成功, 2请求频率过高, 9返回格式有误*/
	char resultStr[200+1];/*错误类型描述*/
	char report_id[200+1];

}msgSend;

char RresultStr[MAX_COMM_COUNTS][2000];


 struct file_conf{
 	char src_dir[128 + 1];
 	char src_name[128 + 1];
 	char src_full_name[256 + 1];
 	char dst_dir[128 + 1];
 	char dst_full_name[256 + 1];
 	char tmp_http_name[256 + 1];
	char tmp_sock_name[256 + 1];
 	char break_dir[128 + 1];
 	char break_name[128 + 1];
 	char break_full_name[256 + 1];
 	char bak_dir[128 + 1];
 	char bak_full_name[256 + 1];
 	char inv_dir[128 + 1];
 	char inv_full_name[256 + 1];
 	char err_dir[128 + 1];
 	char err_full_name[256 + 1];
 	char undo_full_name[256 + 1];
 	char pattern[128 + 1];
 	int limit_num;
	int commit_num;
	char log_path[256+1];
 };
 struct file_conf st_conf;

int	SeqThirdPart = 0; 
char send_Limit[10];
char send_TimeBeg[10];
char send_TimeEnd[10];
int sms_Limit;
int fail_times;
int online_gate;
int alarm_sleep;
scan_file<>* fscan;
scan_file<>* undo_fscan;

vector<string> v_rstr;
vector<string> bad_rstr;
vector<int> rule_dealer;
int redo_times;
ostringstream fmt;
char group_id[20];
msgSend msar[MAX_COMM_COUNTS];
msgSend redo_msar[MAX_COMM_COUNTS];
int sms_count;
util::log_file* lfile;

int bad_count;
 
int r_map[20000];
int undo_flag;
vector<int> v_tmplateId;

database *db = NULL;
static size_t process_data(void *data, size_t size, size_t nmemb, string &content);
#endif
