CC = gcc
CFLAGS = -Wall -Wextra -g -pthread -D_DEFAULT_SOURCE \
         -Wno-unused-variable \
         -Wno-unused-but-set-variable \
         -Wno-unused-parameter \
         -Wno-return-type
LDFLAGS = -pthread -lrt

TARGET = campus_compute

OBJS = master.o \
       jobs.o \
       logger.o \
       ipc.o \
       banker_algo.o \
       worker_failure_detection.o \
       resource_cleaner.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

master.o: master.c
	$(CC) $(CFLAGS) -c master.c -o master.o

%.o: %.c
	$(CC) $(CFLAGS) -Dmain=$*_main -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) campus.log temp_worker.log

.PHONY: all run clean