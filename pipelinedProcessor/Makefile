
#includes do system-c
INCLUDES=-I../../systemc-2.2.0/include/ -I./include

COMPONENT_NAME=pipelinedProcessorComponents

#libs do system-c
LDFLAGS=-L../../systemc-2.2.0/lib-linux -lsystemc
CPPFLAGS=-Wall -fPIC $(INCLUDES)

SRCS=$(shell find ./src -name '*.cpp')
OBJS=$(SRCS:.cpp=.o)

all: $(OBJS) executable

executable:
	@mkdir -p lib
	  $(CXX) -shared -o lib/lib$(COMPONENT_NAME).so $(OBJS) $(LDFLAGS) -lpthread -lm -ggdb

cleanobjs:
	rm -f $(OBJS)

clean: cleanobjs
	  rm -f lib/$(COMPONENT_NAME).so

distclean: clean
	  rm -f $(COMPONENT_NAME)

