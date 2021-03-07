EXENAME = wiki_algs
# fill in object files once we figure out the names of each
OBJS = Graph.o Vertex.o main.o

CXX = clang++
CXXFLAGS = $(CS225) -std=c++1y -stdlib=libc++ -c -g -O0 -WCL4 -Wextra -pedantic   
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

		# Custom Clang version enforcement Makefile rule:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu")
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running CS225 assignments. $(ccend))
endif
else
CLANG_VERSION_MSG = $(warning $(ccyellow) Looks like you are not on EWS. Be sure to test on EWS before the deadline. $(ccend))
endif

.PHONY: output_msgS


all : $(EXENAME)

output_msg: ; $(CLANG_VERSION_MSG)

$(EXENAME) : output_msg $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)


Graph.o : Graph.h Graph.cpp
		$(CXX) $(CXXFLAGS) Graph.cpp

Vertex.o : Vertex.h Vertex.cpp
		$(CXX) $(CXXFLAGS) Vertex.cpp

main.o : main.cpp Graph.h Vertex.h  
		$(CXX) $(CXXFLAGS) main.cpp

clean :
		-rm -f *.o $(EXENAME) test