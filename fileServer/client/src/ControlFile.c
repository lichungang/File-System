/*************************************************************************
	> File Name: ControlFile.c
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: 2015年03月15日 星期日 20时02分17秒
 ************************************************************************/
/*control up and down load
 *
 *commend "ls" is list now file or dictroy
 *commend "cd + dictory" is remove now dictroy
 *commend "cd ../" is remove to last dictroy
 *commend "download + filename" is download the file to client now dirctory 
 *commend "upload + filename" is uploading the file to server now dirctroy 
 *Commend "remove + filename" is delect the file
 *commend "exit" is exit the system;
 * */
#include<apue.h>

int SendCommand(int ClientFd,char buf[50])
{
	int ret;

	ret = send(ClientFd,buf,strlen(buf),0);
	if(ret < 0)
	{
		perror("send command error");
		return -1;
	}
	return ret;
}

int  RecvRemoveResult(int ClientFd)
{
	int ret;
	char  buf[30];
	
	memset(buf,0,sizeof(buf));
	ret = recv(ClientFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		perror("in RecvRemoveResult function,recv error");
		return -1;
	}

	if(!strcmp(buf,"remove success"))
	{
		printf(">>删除成功\n");
		return 1;
	}
	else if(!strcmp(buf,"Not Have The File")) 
	{
		printf(">>服务器上没有此文件\n");
		return -2;
	}

	return ret;
}

int SendUploadFileData(int ClientFd,char ptr[20],char filename[30])
{
	int ret;
	int fd;
	char str[30];
	char buf[512];

	memset(buf,0,sizeof(buf));
	memset(str,0,sizeof(str));

	sprintf(str,"./DOWNLOADFILE/LocalFile/%s",ptr);
	fd = open(str,O_RDWR,0700 );
	if(fd < 0)
	{
		printf(">>无此文件\n");
	//	perror("open file error");
		return -1;
	}
	else
	{

		SendCommand(ClientFd,filename);
		
		char msg[40];
		memset(msg,0,sizeof(msg));
		ret = recv(ClientFd,msg,sizeof(msg),0);
		if(ret < 0)
		{
			perror("send error in SendUploadFileData function");
			return -3;
		}

		if(!strcmp(msg,"File Exsit"))
		{
			printf(">>此文件服务器上已存在\n");
			return -4;
		}
		else
		{
			printf(">>上传中...\n");
			ret = read(fd,buf,sizeof(buf));
			if(ret < 0)
			{
				perror("read data error");
				return -2;
			}	

			while(ret > 0)
			{
				ret = send(ClientFd,buf,sizeof(buf),0);
				if(ret < 0)
				{
					perror("send data error");
					return -5;
				}	

				memset(buf,0,sizeof(buf));
				ret = read(fd,buf,sizeof(buf));
				if(ret < 0)
				{
					printf("read finish");
				}
			}
		}
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"send out");
	ret = send(ClientFd,buf,sizeof(buf),0);

	sleep(1);
	memset(buf,0,sizeof(buf));
	ret = recv(ClientFd,buf,sizeof(buf),0);
	if(!strcmp(buf,"upload finish"))
	{
		printf(">>上传成功\n");
	}

	close(fd);
	return 1;
}

int RecvDictroy(int ClientFd)
{
	int ret;
	char buf[50];

	memset(buf,0,sizeof(buf));
	ret = recv(ClientFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		perror("recv dictroy error");
		return -1;
	}
	printf(">> %s \n",buf);

	return ret;
}

int  RecvDownLoadFile(int ClientFd,char ptr[20])
{
	//recv file
	int fd;
	int ret = 1;
	char buf[512];
	char str[50];
	memset(buf,0,sizeof(buf));

	ret = recv(ClientFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		perror("error to recv file");
		return -2;
	}
	printf(">>受到数据是 %s\n",buf);

	if(!strcmp(buf,"No have the file"))
	{
		printf(">>无此文件\n");
		return -1;
	}
	else
	{
		sprintf(str,"./DOWNLOADFILE/DownloadFile/%s",ptr);
		fd = open(str,O_CREAT | O_RDWR | O_EXCL,0666 );
		if(fd < 0)
		{
			perror("open file error");
			return -1;
		}
//		memset(buf,0,sizeof(buf));
		while(ret > 0)
		{
			printf("recv data is [%s]\n",buf);
			ret = write(fd,buf,strlen(buf));
			if(ret < 0)
			{
				perror("write into file error");
				return -3;
			}
			memset(buf,0,sizeof(buf));
			ret = recv(ClientFd,buf,sizeof(buf),0);	//在这儿阻塞了
			if(ret < 0)
			{
				perror("error to recv file");
				return -2;
			}
			if(!strcmp(buf,"out"))
			{
				printf("受到数据是 %s,数据已收完\n",buf);
				break;
			}
		}
	}
	close(fd);
	printf(">>下载成功\n");
	return 1;
	
}

int UpAndDownloadFile(int ClientFd)
{
	int ret;
	int size;
	char ptr[20];
	char buf[30];
//	memset(ptr,0,sizeof(ptr));
	
	printf("------------going server file system------------\n");
	while(1)
	{
		size = 0;
		memset(buf,0,sizeof(buf));
		memset(ptr,0,sizeof(ptr));
	
		printf(">>");
		scanf("%s",buf);

		if(!strcmp(buf,"ls") ||!strcmp(buf,"cd ../"))
		{
			ret = SendCommand(ClientFd,buf);
			if(0 < ret)
			{
				ret = RecvDictroy(ClientFd);
			}
		}else if(!strcmp(buf,"download"))
		{
			int fd;
			char str[50];
			memset(str,0,sizeof(str));
				printf(">>Please input you want to download filename :");
				scanf("%s",ptr);
			
				//下载之前前判断当前目录下有没有此文件，如果有则不用下载
				size = strlen(ptr);
				if(0 == size)
					printf("您的输入为空\n");
			
				sprintf(str,"./DOWNLOADFILE/DownloadFile/%s",ptr);
				fd = open(str,O_RDONLY,0666);
				if(fd < 0)
				{
					memset(buf,0,sizeof(buf));
					strcat(buf,"download ");
					strcat(buf,ptr);
			
					ret = SendCommand(ClientFd,buf);
					if(ret < 0)
					{
						perror("SendDownLoadFileName error");
						return -1;		
					}
					ret = RecvDownLoadFile(ClientFd,ptr);
					if(ret < 0)
					{
						memset(buf,0,sizeof(buf));
						memset(ptr,0,sizeof(ptr));
						continue;
					}
				}
				else
				{
					printf("您要下载的文件已存在\n");
					continue;
				}
		}else if(!strcmp(buf,"upload"))
		{

			//上传文件
			printf(">>Please input you want upload filename :");
			scanf("%s",ptr);
			
			size = strlen(ptr);
			if(0 == size)
				printf(">>您的输入为空\n");
		
			memset(buf,0,sizeof(buf));
			strcat(buf,"upload ");
			strcat(buf,ptr);

		/*	ret = SendCommand(ClientFd,buf);
			if(ret < 0)
			{
				perror("send upload file name error");
				return -3;
			}
		*/
			ret = SendUploadFileData(ClientFd,ptr,buf);
			if(-1 == ret)
			{
				printf("此文件不存在\n");
				continue;
			//	perror("send upload data error");
			//	return -4;

			}
		}
		else if(!strcmp(buf,"remove"))
		{
			printf(">>Please input you want remove file or dictroy :");
			scanf("%s",ptr);

			size = strlen(ptr);
			if(0 == size)
				printf(">>您的输入为空\n");

			memset(buf,0,sizeof(buf));
			strcat(buf,"remove ");
			strcat(buf,ptr);

			ret = SendCommand(ClientFd,buf);
			if(ret < 0)
			{
				printf(">>删除命令失败\n");
		//		perror("send  remove command error");
				return -5;
			}

			ret = RecvRemoveResult(ClientFd);
			if(ret <  0)
			{
				perror(">>RecvRemoveResult error");
			}
		}
		else if(!strcmp(buf,"exit"))
		{
			SendCommand(ClientFd,buf);
			printf("WELCOME TO USEING NEXT TIME,BYE!\n");
			exit(0);
		}
		else
		{
			printf(">>无此命令\n");
		}
		
	}
}

