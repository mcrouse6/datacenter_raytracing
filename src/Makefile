PROJ = datacenter
EXECUTABLE = main
LIBS = -L/home/mcmc/Research/WI/bin -Wl,-rpath-link=/home/mcmc/Research/WI/bin -lwiapi
CC = g++
CFLAGS = -std=c++11
INCLUDES = -I/home/mcmc/Research/WI/include -I ../include
COMMON = common
TARGET = $(addprefix $(PROJ)/,$(EXECUTABLE))

.PHONY: default all clean 

default: $(TARGET)
all: default

#SRCS = $(foreach dir,$(PROJ),$(wildcard $(dir)/*.cpp)) $(wildcard $(COMMON)/*.cpp)
SRCS = $(wildcard $(PROJ)/*.cpp) $(wildcard $(COMMON)/*.cpp)
OBJECTS = $(patsubst %.cpp, %.o, ${SRCS})
HEADERS = $(wildcard *.hpp)

$(info $$TARGET is [${TARGET}])
$(info $$SRCS is [${SRCS}])
$(info $$OBJECTS is [${OBJECTS}])

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

.PRECIOUS: $(EXECUTABLE) $(OBJECTS) 

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(INCLUDES) $(LIBS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
