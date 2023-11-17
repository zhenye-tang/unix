CFLAGS += -pthread
LDFLAGS += -pthread

all:thread

thread: thread_0.o
	gcc $^ -o $@ 

clean:
	$(RM) -r *.o thread