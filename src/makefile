COMPILER = gcc
PROGRAM = diceroll
SRC = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRC))
LDFLAGS = -lGL -lglut

# OSX
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
LDFLAGS = -framework OpenGL -framework GLUT
endif

all : $(PROGRAM)
	mv $(PROGRAM) ../bin
	rm *.o

$(PROGRAM) : $(OBJS)
	$(COMPILER) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

%.o : %.c
	$(COMPILER) -c $<
