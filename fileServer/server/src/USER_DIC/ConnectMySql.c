/*************************************************************************
	> File Name: ConnectMySql.c
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月16日 星期一 21时03分52秒
 ************************************************************************/

#include<apue.h>
#include<mysql/mysql.h>

extern char *IP_ADDRESS;
extern char *MYSQL_USER;
extern char *DB_NAME;
extern char *MYSQL_PASSWD;
extern int MySQL_PORT;
extern int IP_PORT;

MYSQL * connectMysqlFunc()
{
	MYSQL *conn = mysql_init(NULL);
	MYSQL *retMySql;
	char *res[40];
	if(conn == NULL)
	{
		printf("初始化 MYSQL 数据库失败");
		return NULL;
	}

	retMySql = mysql_real_connect(conn,IP_ADDRESS,MYSQL_USER,MYSQL_PASSWD,DB_NAME,MySQL_PORT,NULL,0);
	if(retMySql == NULL)
	{
		perror("链接数据库失败");
		mysql_close(conn);
		return NULL;
	}

	return conn;
}

int connectMysql(int flags,char name[30],char passwd[30])
{
	char na[30];
	char pa[30];
	int ret;
	int ret1 = 0;
	int ret2 = 0;
	MYSQL *conn;
	char res[30];

	memset(na,0,sizeof(na));	
	memset(pa,0,sizeof(pa));	

	printf("开始链接数据库...\n");
	conn = connectMysqlFunc();
	if(conn == NULL)
	{
		perror("链接数据库失败");
		return -1;
	}

	printf("链接数据库成功\n");
	
	if(1 == flags)		//表示在数据库中查询
	{
		char sql[356];

		strcpy(sql,"SELECT user_name,passwd FROM user_base_info");
		printf("执行查询结果...");
		ret = mysql_real_query(conn,sql,strlen(sql));
		if(ret != 0)
		{
			perror("查询失败");
			mysql_close(conn);
			return -2;
		}	

		printf("执行查询结果成功");
			//将查询结果和 name passwd 对比
			//如果有相同的则返回 1 否则返回 0
	
		printf("检索结果开始...");
		MYSQL_RES *rst = mysql_use_result(conn);
		if(rst != NULL)
		{
			int fieldNum = mysql_field_count(conn);
			MYSQL_ROW row;
			while(row = mysql_fetch_row(rst))
			{
				int i;
				for(i = 0; i < 2; i++)
				{
					strcpy(na,row[0]);
					strcpy(pa,row[1]);
				}
			
				if(!(strcmp(na,name)))
				{
					ret1 = 1;
					if(!(strcmp(pa,passwd)))
					{
						ret2 = 2;
						break;
					}else
						break;
				}

				memset(na,0,sizeof(na));
				memset(pa,0,sizeof(pa));
				printf("\n");
			}
			mysql_free_result(rst);
		}
	}
	else if(2 == flags)		//表示向数据库中插入数据
	{
		char sql[128];

		printf("开始向数据库内插入用户...\n");

	/*	strcat(sql,"INSERT INTO user_base_info (user_name,passwd) VALUES (");
		strcat(sql,""");
		strcat(sql,name);
		strcat(sql,""");
		strcat(sql,",");
		strcat(sql,""");
		strcat(sql,passwd);
		strcat(sql,""");
		strcat(sql,")");
*/

		sprintf(sql,"INSERT INTO user_base_info (user_name,passwd) VALUES ('%s','%s')",name,passwd);
		ret = mysql_real_query(conn,sql,strlen(sql));
		if(ret != 0)
		{
			perror("插入失败");
			mysql_close(conn);
			return -2;
		}	

		printf("插入成功");
		ret1 = 5;
		ret2 = 5;
	}


	ret = ret1 + ret2;
	printf("检索成功\n");
	mysql_close(conn);
	return ret;
}
