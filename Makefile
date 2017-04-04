
TARGET   = ipstack

CC       = gcc
# compiling flags here
CFLAGS   = -std=c99 -Wall -I.

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -I. -lm

# change these to proper directories where each file should be
SRCDIR   = src
TSTDIR 	 = tst
UNITYDIR = Unity/src

SOURCES  := $(wildcard $(SRCDIR)/*.c)
TSTSRC   := $(wildcard $(TSTDIR)/*.c)
UNITYSRC := $(wildcard $(UNITYDIR)/*.c)

TSTRES   := $(patsubst %,%.txt,$(notdir $(basename $(TSTSRC))))

OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=%.o)

ipstack : $(OBJECTS)
	@$(LINKER) -o $@ $(LFLAGS) $(OBJECTS)

.PHONY: test
test : $(TSTRES)
	@echo "Tests complete. See results\n"

test%.txt : test%.out
	@-./$< > $@ 2>&1

test%.out : test%.o %.o unity.o
	@$(LINKER) $(DEPFLAGS) -o $@ $^

%.o : $(UNITYDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(UNITYDIR)	

%.o : $(TSTDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(SRCDIR) -I$(UNITYDIR)

%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: %.o
.PRECIOUS: test%.out

.PHONY: clean
clean:
	@rm -f *.o *.out *.txt $(TARGET) $(DEPDIR)/*.d

