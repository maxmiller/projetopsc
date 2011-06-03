
INCLUDES=-I../systemc-2.2.0/include/

COMPONENT_NAME=rula
LDFLAGS=-L../systemc-2.2.0/lib-linux -lsystemc
CPPFLAGS=-Wall -fPIC $(INCLUDES)

SRCS=$(shell find . -name '*.cpp')
OBJS=$(SRCS:.cpp=.o)

all: $(OBJS) executable cleanobjs

executable:
	  $(CXX) -o $(COMPONENT_NAME) $(OBJS) $(LDFLAGS) -lpthread -lm

cleanobjs:
	rm -f $(OBJS)

clean: cleanobjs
	  rm -f $(COMPONENT_NAME)

distclean: clean
	  rm -f $(COMPONENT_NAME)

