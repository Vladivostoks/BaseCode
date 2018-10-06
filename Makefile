#******************************************************/
#-*-coding-utf-8-*-
#File:Makefile
#Date:2018-10-01
#Author:Ayden.Shu
#brief:Project build Makefile
#******************************************************/



all:demo



CFLAG +=
SRC +=

CC=
LD=
SRTIP=

OBJPATH ?= ./bin
SRCPATH = ./src

$(OBJPATH)/*.o:$(SRCPATH)/*.c
	gcc $(CFLAG) -c $^ -o $@

demo:$(SRC)
		


#clean
.PHONY:clean

clean:
	rm ./obj/*.o ./lib/*.a ./lib/*.so ./bin
	rmdir ./obj ./lib ./bin
