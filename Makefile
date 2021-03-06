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
#CC=/usr/local/opt/gcc/bin/gcc-8
#CCP=/usr/local/opt/gcc/bin/g++-8
LD=clang
SRTIP=strip

Q = @
TRACE_CC = @echo "	CC	" $<
TRACE_CCP = @echo "	CCP	" $<
TARCE_LD = @echo "	LD	" $@
TRACE_AR = @echo "	AR	" $@

OBJPATH += ./obj
SRCPATH += ./src
LIBPATH += $(PREFIX)/lib
BINPATH += $(PREFIX)
HEADPATH += ./include

SRCFILE += $(wildcard $(SRCPATH)/*.cpp)
SRCFILE += $(wildcard $(SRCPATH)/*.c)
HEADFILE += $(wildcard $(HEADPATH)/*)

OBJFILE += $(addprefix $(OBJPATH)/,$(filter %.o,$(patsubst %.c,%.o,$(notdir $(SRCFILE)))))
OBJFILE += $(addprefix $(OBJPATH)/,$(filter %.o,$(patsubst %.cpp,%.o,$(notdir $(SRCFILE)))))

LIBFILE += 
CFLAG += -Wall -O2
CFLAG += -I ./include

CPPFLAG += -Wall -O2 -std=c++11
CPPFLAG += -I ./include

#LDFLAG +=  ${addprfix -l,$(LIBFILE)}
LDFLAG += -lstdc++


$(info Start Building demo....)
all:director $(LIBFILE) demo

$(OBJPATH)/%.o:$(SRCPATH)/%.c $(HEADFILE)
	$(TRACE_CC)
	$(Q)$(CC) $(CFLAG) -c $(filter %.c,$^) -o $@


$(OBJPATH)/%.o:$(SRCPATH)/%.cpp $(HEADFILE)
	$(TRACE_CCP)
	$(Q)$(CCP) $(CPPFLAG) -c $(filter %.cpp,$^) -o $@


$(BINPATH)/demo:$(OBJFILE)
	@echo Source file include $(SRCFILE)
	$(TRACE_LD)
	$(Q)$(LD) $(LDFLAG) $^ -o $@
#	$(Q)$(STRIP) $@ -o $@.striped

#virtual aim
.PHONY:clean director

director:
	@mkdir -p $(OBJPATH) $(LIBPATH) $(BINPATH)

clean:
#-rm $(OBJPATH)/*.o $(LIBPATH)/*.a $(LIBPATH)/*.so $(BINPATH)/*
	-rm -rf $(OBJPATH) $(LIBPATH) $(BINPATH)/demo
	
