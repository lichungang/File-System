/*************************************************************************
	> File Name: clientMain.c
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月13日 星期五 10时12分58秒
 ************************************************************************/

#include<apue.h>
#include"client.h"
/*
 *1 提供用户注册与登录界面
 *2 提供文件管理界面
 * */


int main(void)
{
	//注册登录界面
	User_UpAndRegister_Interface();
	
	LogAndRestiger();
}
