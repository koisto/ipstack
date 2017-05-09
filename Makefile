
CC       = gcc
CFLAGS   = -std=c99 -Wall -I. 

LINKER   = gcc
LFLAGS   = -Wall -I. -lm

SRCDIR   = src
TSTDIR 	 = tst
UNITYDIR = ../Unity/src

SOURCES  := $(wildcard $(SRCDIR)/*.c)
TSTSRC   := $(wildcard $(TSTDIR)/*.c)
UNITYSRC := $(wildcard $(UNITYDIR)/*.c)

TSTBIN   := $(patsubst %,%.out,$(notdir $(basename $(TSTSRC))))

OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=%.o)


demo : $(OBJECTS)
	@echo "Linking $@"
	@$(LINKER) -o $@ $(LFLAGS) $(OBJECTS)

test : $(TSTBIN)
	@$(foreach tst,$(TSTBIN),./$(tst);)   

test%.out : test%.o %.o unity.o
	@echo "Linking $@"
	@$(LINKER) -o $@ $^

%.o : $(UNITYDIR)/%.c
	@echo "Compiling $<" 
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(UNITYDIR)	

%.o : $(TSTDIR)/%.c
	@echo "Compiling $<" 
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(SRCDIR) -I$(UNITYDIR)

%.o : $(SRCDIR)/%.c
	@echo "Compiling $<" 
	@$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: %.o

.PHONY: clean
clean:
	@rm -f *.o *.out demo

