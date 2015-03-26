BIN_DIR=../bin
INCLUDE_FLAG=-I ../include -I./
LIB_DIR=../lib
LIB_FLAG= -L/usr/lib/mysql -lmysqlclient

MainServer : MainServer.o ConnectClient.o Connect.o AdministrateFile.o ConnectMySql.o -lpthread
	gcc -Wall -ggdb -o $@ $^ ${LIB_FLAG}

%.o : %.c
	gcc -o $@ ${INCLUDE_FLAG} -c $^ -lpthread

