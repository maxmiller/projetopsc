
#includes do system-c
INCLUDES=-I../../systemc-2.2.0/include/ \
				 -I../components/include

COMPONENT_NAME=processor

#libs do system-c
LDFLAGS=-L../../systemc-2.2.0/lib-linux -lsystemc\
				-L../components/lib -lcomponents
CPPFLAGS=-Wall -fPIC $(INCLUDES)

SRCS=$(shell find -name '*.cpp')
OBJS=$(SRCS:.cpp=.o)

all: $(OBJS) executable

executable:
	@mkdir -p bin
	  $(CXX) -o ./bin/$(COMPONENT_NAME) $(OBJS) $(LDFLAGS) -lpthread -lm

cleanobjs:
	rm -f $(OBJS)

clean: cleanobjs
	  rm -f $(COMPONENT_NAME)

distclean: clean
	  rm -f $(COMPONENT_NAME)

