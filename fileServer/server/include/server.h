/*************************************************************************
	> File Name: server.h
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月15日 星期日 10时35分17秒
 ************************************************************************/

#include<apue.h>
#include<mysql/mysql.h>

typedef void(*TASK)(va_list);

void ConnectClient();
pid_t CreateNewProc(TASK task,...);
void *server_thread(void *p);
int AdminiFunc(int ClientFd,char name[30]);
int ConductCommandLsSend(int ClientFd,char buf[50],char UserName[30]);
int  ConductCommandLs(int ClientFd,char buf[],char UserName[30]);
int ConductCommandDownload(int ClientFd,char para[30]);
int ConductCommandUpload(int ClientFd,char para[30]);
int ConductCommandRemove(int ClinetFd,char para[30]);
int ConnectMysql(char name[30],char passwd[30]);
MYSQL * connectMysqlFunc();
int connectMysql(int flags,char name[30],char passwd[30]);
