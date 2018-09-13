#ifndef _DEFINE_H_
#define _DEFINE_H_

#define	HOST_IP_LEN	15
#define	FILE_NAME_LEN	128
#define	PORT_LEN	6
#define	TIME_LEN	32
#define MSG_LEN		1024
#define ERR_SOCK_LEN	256
#define LOG_NAME_LEN	16
#define LOG_PASS_LEN	16
#define SERVER_NAME_LEN	20
#define RES_LEN		8
#define USER_NAME_LEN	21
#define SP_NUMBER_LEN	21
#define MSG_CONTENT_LEN	1024
#define	MAX_THREADS	5
#define	CARD_ID_LEN	20
#define	ZIP_CODE_LEN	5
#define LOG_TYPE_LEN	1
#define SEND_BUFF_LEN	1024
#define CHARGE_NUMBER_LEN	21
#define USER_NUMBER_LEN		21
#define CORP_ID_LEN		5
#define SERVICE_TYPE_LEN	10
#define FEE_VALUE_LEN		6
#define GIVEN_VALUE_LEN		6
#define EXPIRE_TIME_LEN		16
#define SCHEDULE_TIME_LEN	16
#define RESULT_LEN		255
#define FLAG_LEN		2
#define SERIAL_LEN		12
#define MESSAGE_LEN		1024
/*msg id define*/
#define	SGIP_BIND		0x1
#define SGIP_BIND_RESP		0x80000001
#define	SGIP_UNBIND		0x2
#define SGIP_UNBIND_RESP	0x80000002
#define SGIP_SUBMIT		0x3
#define SGIP_SUBMIT_RESP	0x80000003
#define	SGIP_DELIVER		0x4
#define	SGIP_DELIVER_RESP	0x80000004
#define SGIP_REPORT		0x5
#define SGIP_REPORT_RESP	0x80000005

/*log type define*/
#define SPSMG	1
#define SMGSP	2

/*package length define*/
#define RESP_PACK_LEN	29
#define BIND_PACK_LEN  61
#define UNBIND_HEAD_LEN  20

/*comm status define*/
#define RUN_SUCCESS	"3"
#define FAIL_1		"1"
#define FAIL_2		"2"

#define SEND_SUCCESS	"0"

/*return package macro define*/
#define	REG_SUCCESS	"谢谢你的使用。如需要，请您重复操作。"
#define REG_FAIL_1	"对不起，您的手机不支持该业务!"
#define	REG_FAIL_2	"对不起，操作不成功，输入错误或您的手机不支持该业务。"



#define SP_NUMBER	"10011" /*上线时改成10011*/
#define USER_COUNT	1
#define CORP_ID		"21043" /*上线时改成21043*/
#define FEE_TYPE	1
#define FEE_VALUE	"0"
#define GIVEN_VALUE	"0"
#define AGENT_FLAG	0
#define MT_FLAG		2
#define PRIORITY_FLAG	0
#define	REPORT_FLAG	3
#define MSG_CODING	15
#define MSG_TYPE	0
#define TP_PID		0
#define TP_UDHI		0


/*seq1 define*/
#define SEQ_1		3021021043



/*log level flag*/
#define LOG_INFO	1
#define LOG_DEBUG	2
#define LOG_ERR		3
#define LOG_SYS		4

/*data type macro*/
#define INT_TYPE	0
#define OTH_TYPE	1

/*error handle marco*/
#define E_FORMAT	1
#define	E_MESSAGEID	2
#define	E_MSGLENGTH	3
#define	E_UNBIND	4

/*error macro define*/
#define	OK		0
#define E_NOLOG		1
#define E_DUPLOG	2
#define E_EXCCON	3
#define E_LOGTYPE	4
#define	E_PARMFORMAT	5
#define	E_INVALIENUM	6
#define	E_MSGID		7
#define E_LENGTH	8
#define	E_SEQENCE	9
#define	E_SYS		32
 
#endif

