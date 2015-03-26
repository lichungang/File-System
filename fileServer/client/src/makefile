BIN_DIR=../bin
INCLUDE_FLAG=-I ../include -I./
LIB_DIR=../lib

clientMain : clientMain.o InterFace.o LogAndRes.o ControlFile.o
	gcc -o $@ $^



%.o : %.c
	gcc -o $@ ${INCLUDE_FLAG} -c $^
