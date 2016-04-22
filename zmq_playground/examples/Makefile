CPP			    = g++
RM			    = rm -f
CPPFLAGS		= -I. -O2 -std=c++11 -g
LDFLAGS			= -lpthread -lzmqpp -lzmq -Wl,--no-as-needed
SOURCES			= $(wildcard simple*.cpp)
TARGETS			= $(SOURCES:%.cpp=%)

all:	${TARGETS}

clean:
	${RM} *.obj *~* ${TARGETS}

%: %.cpp
	${CPP} ${CPPFLAGS} -o $@ $< MyObj.cpp ${LDFLAGS}
