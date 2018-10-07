#******************************************************/
#-*-coding-utf-8-*-
#File:Makefile
#Date:2018-10-01
#Author:Ayden.Shu
#brief:Project build Makefile
#******************************************************/

#生成目标文件路径
PREFIX ?= .

CC=clang
CCP=clang++
LD=clang
SRTIP=strip

Q = @
TRACE_CC = @echo "	CC	" $<
TRACE_CCP = @echo "	CCP	" $<
TARCE_LD = @echo "	LD	" $@
TRACE_AR = @echo "	AR	" $@

OBJPATH += ./obj
SRCPATH += ./src
LIBPATH += 
BINPATH += $(PREFIX)/bin

SRCFIEL += $(wildcard $(SRCPATH)/*.cpp)
SRCFIEL += $(wildcard $(SRCPATH)/*.c)

OBJFILE += $(addprefix $(OBJPATH)/,$(patsubst %.c,%.o,$(notdir SRCFILE)))
LIBFILE += 

CFLAG += -Wall -O3
CFLAG += -I ./include

CPPFLAG += -Wall -O3
CPPFLAG += -I ./include

LDFLAG +=  ${addprfix -l,$(LIBFILE)}


all:director $(LIBFILE) demo

$(OBJPATH)/*.o:$(SRCPATH)/*.c
	$(TARCE_CC)
	$(Q)$(CC) $(CFLAG) -c $^ -o $@


$(OBJPATH)/*.o:$(SRCPATH)/*.cpp
	$(TARCE_CCP)
	$(Q)$(CCP) $(CPPFLAG) -c $^ -o $@


demo:$(OBJFILE)
	$(TRACE_LD)
	$(Q)$(LD) $(LDFILE) $^ -o $(PRFIX)/$(BINPATH)/$@
#	$(Q)$(STRIP) $@ -o $@.striped

#virtual aim
.PHONY:clean director

director:
	@mkdir -p $(OBJPATH) $(PREFIX)/$(LIBPATH) $(PREFIX)/$(BINPATH)

clean:
	rm ./$(OBJPATH)/*.o $(PREFIX)/$(LIBPATH)/*.a $(PREFIX)/$(LIBPATH)/*.so $(PREFIX)/$(BINPATH)/*
	rmdir ./$(OBJPATH) $(PREFIX)/$(LIBPATH) $(PREFIX)/$(BINPATH)
