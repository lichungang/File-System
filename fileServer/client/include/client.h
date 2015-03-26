/*************************************************************************
	> File Name: client.h
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月13日 星期五 10时22分33秒
 ************************************************************************/

#include<apue.h>

void User_UpAndRegister_Interface();
void LogAndRestiger();
int  ConnectServer();
int UpAndDownloadFile();
int SendCommand(int ClientFd,char buf[30]);
int RecvDictroy(int ClientFd);
int SendDownLoadFileName(int Clientfd,char ptr[20]);
int RecvDownLoadFile(int ClientFd,char ptr[20]);
int SendUploadFileData(int ClientFd,char ptr[20]);
int RecvRemoveResult(int ClientFd);
