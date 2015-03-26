/*************************************************************************
	> File Name: ConnectClient.c
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月15日 星期日 10时34分37秒
 ************************************************************************/
/*
 *本文件功能是链接客户端
 *验证登录  注册
 *
 * */
#include<apue.h>
#include"server.h"


char *IP_ADDRESS = "127.0.0.1";
char * MYSQL_USER = "root";
char * DB_NAME = "USE_INFO";
char *MYSQL_PASSWD = "03131670";
int MySQL_PORT = 3306;
int IP_PORT = 8000;
int fds[100];
int size = 0;

typedef struct LoginAndResHead
{
	int flag;
	char name[30];
	char passwd[30];
}LoginAndResHead;

int initServer()
{
	int socketFd;
	struct sockaddr_in addr;

	socketFd = socket(AF_INET,SOCK_STREAM,0);
	if(socketFd < 0)
	{
		perror("获取SocketFd失败");
		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(IP_PORT);
	addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);;

	int ret = bind(socketFd,(struct sockaddr *)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("绑定端口号和地址失败");
		return -1;
	}

	ret = listen(socketFd,10);
	if(ret < 0)
	{
		perror("监听失败");
		return -2;
	}

	return socketFd;
}

void LoginFunc(int ClientFd,char name[30],char passwd[30])
{
	int ret;
	char buf[20];

	memset(buf,0,sizeof(buf));

	//在数据库中查找，如果查找成功，且密码正确，则登录成功,
	//....
	pid_t pid;
	printf("pid = [%d]\n",getpid());
	ret =  connectMysql(1,name,passwd);
	if(3 == ret)
		sprintf(buf,"LOGIN SUCCESS");
	else if(0 == ret)
	{
		sprintf(buf,"NO HAVE THE USER");
		printf("无此用户\n");
	}
	else if(1 == ret)
	{
		printf("用户密码错误\n");
		sprintf(buf,"PASSWD ERROR");
	}

	ret = send(ClientFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		perror("send success msg error");

		return;
	}

	//administrate function   登录成功后将执行文件管理任务
	AdminiFunc(ClientFd,name);

}
int  ResigerFunc(int ClientFd,char name[30],char passwd[30])
{
	//注册函数
	/*
	 *先在数据库中检查是否有此用户，如果有则给注册者发回提示信息
	 *
	 * */
	int ret;
	char buf[50];

	memset(buf,0,sizeof(buf));
	ret =  connectMysql(1,name,passwd);
	if(ret != 0)
	{
		printf("此用户名以存在\n");
		sprintf(buf,"THE USER NAME ORDER EXIST");
	}else 
	{
		printf("用户名合法\n");
		sprintf(buf,"CONTIUEN");
	}

	ret = send(ClientFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		perror("regiset return error");
		return 0;
	}

	ret = connectMysql(2,name,passwd);
	if(ret < 0)
	{
		perror("注册失败");
		return 0;
	}
	else if(10 == ret)
	{
		printf("注册成功\n");
	}
}
void *server_thread(void *p)
{
	int i = 0;
	int j = 0;
	int ret;
	int ClientFd = *(int *)p;
	int THID = pthread_self();
	char buf[80];
	LoginAndResHead head;		//记得free（）

	memset(buf,0,sizeof(buf));
	memset(head.name,0,sizeof(head.name));
	memset(head.passwd,0,sizeof(head.passwd));
	
	ret = recv(ClientFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		perror("接受数据失败");
		return;
	}

	printf("to here?\n");
	head.flag = atoi(buf);

	char * ptr;
    ptr = (char *)(buf + 1);
	ptr++;
	
	while(ptr[i] != ' ')
	{
		head.name[i] = ptr[i];
		i++;
	}

	ptr++;
	while(ptr[i] != '\0')
	{
		head.passwd[j] = ptr[i];
		i++;
		j++;

	//	printf("%c",head.passwd[j]);
	//	j++;
	//	i++;
	}

	if(1 == head.flag)			//用户登录
		LoginFunc(ClientFd,head.name,head.passwd);		
	else if(2 == head.flag)		//注册用户
		ResigerFunc(ClientFd,head.name,head.passwd);
	
}

void AcceptClient(va_list ap)
{
	int socketFd = va_arg(ap,int);

	printf("socetFd = [%d] pid = [%d] \n",socketFd,getpid());

	int clientFd;
	struct sockaddr_in sockaddr;

	//如果接受到的第一个是 1 表示登录
	//如果接受到的第一个是 2 表示注册
	
	while(1)
	{
	
		char buf[50];
		int len = sizeof(struct sockaddr_in);
		pthread_t tid;

		clientFd = accept(socketFd,(struct sockaddr *)&sockaddr,&len);
		if(clientFd < 0)
		{
			perror("接受链接失败");
			return;
		}

		fds[size++] = clientFd;
		printf("我是 %d 客户,我上线了\n",clientFd);

		//有客户接入，启动线程为此客户端服务
		pthread_create(&tid,0,server_thread,&clientFd);		//pthread_create(pthread_t *restrict tidp,
		//const thread_attr_t *restrict attr,void *(*start_trn)(void *),void *restrict arg);	arg是函数指针的
		//参数的首地址

	
	}
}

void  ConnectClient()
{
	int SocketFd = initServer();

	pid_t pid = CreateNewProc(AcceptClient,SocketFd);
}
