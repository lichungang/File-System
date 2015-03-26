/*************************************************************************
	> File Name: LogAndRes.c
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月15日 星期日 10时03分56秒
 ************************************************************************/

#include<apue.h>
#include"client.h"
int IP_PORT = 8000;
char *IP_ADDRESS = "127.0.0.1";

typedef struct LoginAndResHead
{
	int flag;
	char name[30];
	char * passwd;
}LoginAndResHead;

int SendFlagToServer(int ClientFd,LoginAndResHead *head)
{
	int ret;
	char buf[50] = {0};

	sprintf(buf,"%d",head->flag);
	strcat(buf," ");
	strcat(buf,head->name);
	strcat(buf," ");
	strcat(buf,head->passwd);
//	strcat(buf,);

	ret = send(ClientFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		perror("发送数据失败");

		return 0;
	}
	printf("发送的数据是 : %s\n",buf);

	memset(buf,sizeof(buf),0);
	ret = recv(ClientFd,buf,sizeof(buf),0);		//等待服务器发送回来是否登录成功的消息
	if(ret < 0)
	{
		perror("recv server error");
		return -1;
	}

	if(!(strcmp("LOGIN SUCCESS",buf) )|| !(strcmp("RESIGTER SUCCESS",buf)))
	{
		ret = 1;
		printf("%s\n",buf);
	}
	else if(!(strcmp("NO HAVE THE USER",buf)))
	{
		ret = -2;
		printf("无此用户,");
	}
	else if(!(strcmp("PASSWD ERROR",buf)))
	{
		printf("密码错误,");
		ret = -3;
	}
	//回收注册是服务器发回来的消息
	else if(!(strcmp("THE USER NAME ORDER EXIST",buf)))
	{
		printf("此用户名已经存在，");
		ret = -4;
	}
	else if(!(strcmp("CONTIUEN",buf)))
	{
		ret  = -5;
	}
	return ret;
}

int ConnectServer()
{

	int ret;
	int SocketFd;
	struct sockaddr_in addr;
	SocketFd = socket(AF_INET,SOCK_STREAM,0);
	if(SocketFd < 0)
	{
		perror("获取 socketFd 失败");
		return ;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(IP_PORT);
	addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

	ret = connect(SocketFd,(struct sockaddr *)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("链接服务器失败");
		return ;
	}

	return SocketFd;	
}
void Login()
{
	int ret;
	int i = 0;
	LoginAndResHead Login;
	Login.flag = 1;
	int ClientFd;
	char buf[30];

	while(i <= 10)
	{
		printf("请输入您的帐号 :");
		scanf("%s",Login.name);
		Login.passwd = getpass("请输入您的密码 :");

		ClientFd = ConnectServer();
		ret = SendFlagToServer(ClientFd,&Login);
		if(1 == ret)	//表示登录成功
			break;
		i++;
		printf("请重新登录\n");
	}

	if(i >= 10)	//login times after 10,login error
	{
		printf("sorray,you already has 10 error,exit");
		exit(0);
	}else 
	{
		//file system,updata,download
		UpAndDownloadFile(ClientFd);
	}
}

void Restiger()
{
	LoginAndResHead Resigter;
	Resigter.flag = 2;
	char * repasswd;
	int ClientFd;
	int ret;
	int PassLen = 0;

	while(1)
	{
		printf("请输入您的注册名 :");
		scanf("%s",Resigter.name);
		
		Resigter.passwd = getpass("请输入密码 :");

		printf("passwd = [%s]\n",Resigter.passwd);
		repasswd = getpass("请再次输入密码 :");

		printf("passwd = [%s]\n",Resigter.passwd);
		printf("repasswd = [%s]\n",repasswd);

		PassLen = strlen(repasswd);

		if(!(strcmp(repasswd,Resigter.passwd)) && PassLen != 0)
		{
			ClientFd = ConnectServer();
			ret = SendFlagToServer(ClientFd,&Resigter);
			if(-5 == ret)
			{
				printf("注册成功,请登录\n");
				sleep(2);
				break;
			}
			else if(-4 == ret)
				printf("此用户名以存在，请重新输入\n");
		}else
			printf("两次密码不一制，请重新输入\n");
			printf("密码为空，请重新输入\n");
	
		memset(repasswd,0,sizeof(repasswd));
		memset(Resigter.passwd,0,sizeof(Resigter.passwd));
		memset(Resigter.name,0,sizeof(Resigter.name));
	}

	printf("resigter success 注册成功\n");
	User_UpAndRegister_Interface();		//注册成功 回到登录界面
	LogAndRestiger();			//同上
	
}
void LogAndRestiger()
{
	int i;
	int j = 0;

	while(1)
	{
		printf("please input your need: ");
		scanf("%d",&i);

		if(1 == i)
		{
			Login();			//登录帐号函数
			printf("your input is %d\n",i);
			break;
		}
		else if(2 == i)
		{
			Restiger();			//注册帐号函数
			printf("your input is %d\n",i);
			break;
		}
		else if(3 == i)
			exit(0);
		else 
		{
			printf("your input error,please input agin\n");
		}

	}
}
