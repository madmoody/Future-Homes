# Makefile for the project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Source files and output
SRCS = AM312PIRIntroder.c GateAutoActuator.c GroveGassensor(MQ9).c LihtFanAc.c LiveMonitoring.c SmartDoor.c firandsmokedetect.c
OBJS = $(SRCS:.c=.o)
EXECUTABLE = my_program

# Targets and dependencies
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the generated files
clean:
	rm -f $(OBJS) $(EXECUTABLE)
