/*************************************************************************
	> File Name: Connect.c
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月15日 星期日 10时48分41秒
 ************************************************************************/

#include<apue.h>
#include"server.h"

void catch_CHLD(int num)
{
	while(waitpid(-1,WNOHANG,NULL) > 0)
		;
}

void Zombury()
{
	signal(SIGCHLD,catch_CHLD);
}

pid_t CreateNewProc(TASK task,...)
{
	va_list ap;
	pid_t pid;

	va_start(ap,task);

	pid = fork();
	if(0 == pid)
	{
		task(ap);
		exit(0);
	}

	return pid;
}
