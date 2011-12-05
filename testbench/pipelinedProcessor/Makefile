
#includes do system-c
INCLUDES=-I../../../systemc-2.2.0//include/ -I../../pipelinedProcessor/include -I./

COMPONENT_NAME=pipelinedProcessorTB

#libs do system-c
LDFLAGS=-L../../../systemc-2.2.0/lib-linux -L../../pipelinedProcessor/lib -lpipelinedProcessorComponents -lsystemc
CPPFLAGS=-Wall -fPIC $(INCLUDES)

SRCS=$(shell find -name '*.cpp')
OBJS=$(SRCS:.cpp=.o)

all: $(OBJS) executable

executable:
	  $(CXX) -o $(COMPONENT_NAME) $(OBJS) $(LDFLAGS) -lpthread -lm -ggdb

cleanobjs:
	rm -f $(OBJS)

clean: cleanobjs
	  rm -f $(COMPONENT_NAME)

distclean: clean
	  rm -f $(COMPONENT_NAME)

