#
# Makefile for CPP
#

# Compiler and compiler options:



CC		  = g++
CXX       = g++
CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
LDFLAGS	  =
LDLIBS    = -L. -lclientserver
PREFIX    = /h/d2/u/dt09lo7/Desktop/Cplusplus
SRCFILES  = $(wildcard *.cc)
OBJFILES  = $(SRCFILES:%.cc=%.o)
EXEFILES  = main libclientserver.a
DEBUG = false
# insert the following lines after the definitions of CXXFLAGS and LDFLAGS
ifeq ($(DEBUG), true)
CXXFLAGS += -ggdb
LDFLAGS += -ggdb
else
CXXFLAGS += -O2
endif


#g++ -c -ggdb -pipe -Wall -W -ansi -pedantic-errors -Wmissing-braces -Wparentheses -Wold-style-cast reciprocal.cc
#g++ -o reciprocal -ggdb reciprocal.o

#Compile & link EVERYTHING
.PHONY: all
all: $(EXEFILES)

include $(SRCFILES:.cc=.d)

#Installation

libclientserver.a: connection.o server.o
	ar rv libclientserver.a \
	connection.o server.o
	ranlib libclientserver.a

clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) *.d.*

cleaner: clean
	$(RM) libclientserver.a


# Linking:
main: in_memory_database.o article.o news_group.o on_disc_database.o
#	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

#TOTALLY BONKERS
%.d: %.cc
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Dependencies, the implicit rule .cc => .o is used
#ltest.o: ltest.cc list.h
#list.o: list.cc list.h
