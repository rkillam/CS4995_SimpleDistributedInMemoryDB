CPP := g++
RM := rm -f

LDFLAGS := -lpthread -lzmqpp -lzmq -lconfig++ -Wl,--no-as-needed
CPP_FLAGS := -I. -O2 -std=c++11 -g

SOURCES := master.cpp slave.cpp router.cpp
TARGETS := $(SOURCES:%.cpp=%)

DEPENDENCIES := utils.cpp Message.cpp

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)

%: %.cpp $(DEPENDENCIES)
	$(CPP) $(CPP_FLAGS) -o $@ $^ $(LDFLAGS)
