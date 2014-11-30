SRCS = \
	main.c leap.c

OBJS = $(subst .c,.o,$(SRCS))

TSTSRCS = \
	test.c

TSTOBJS = $(subst .c,.o,$(TSTSRCS))

BNCHSRCS = \
	bench.c leap.c

BNCHOBJS = $(subst .c,.o,$(BNCHSRCS))

CFLAGS = 
LIBS = -lsqlite3
TARGET = main

all : $(TARGET)

$(TARGET) : $(OBJS)
	gcc -o $@ $(OBJS) $(LIBS)

test: $(TSTOBJS)
	gcc -o $@ $(TSTOBJS) $(LIBS)

bench: $(BNCHOBJS)
	gcc -o $@ $(BNCHOBJS) $(LIBS)

.c.o :
	gcc -c $(CFLAGS) -I. $< -o $@

clean :
	rm -f *.o $(TARGET) test bench

purge:
	rm -f *.sqlite
