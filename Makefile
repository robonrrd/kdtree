CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11
LDFLAGS=-g 
LDLIBS=-lm

BUILD_SRCS=build_kdtree.cpp
BUILD_OBJS=$(subst .cpp,.o,$(BUILD_SRCS))

QUERY_SRCS=query_kdtree.cpp
QUERY_OBJS=$(subst .cpp,.o,$(QUERY_SRCS))

all: build_kdtree query_kdtree

build_kdtree: $(BUILD_OBJS)
	$(CXX) $(LDFLAGS) -o build_kdtree $(BUILD_OBJS) $(LDLIBS)
build_kdtree.o:	build_kdtree.cpp kdtree.h
	$(CXX) $(CPPFLAGS) -c build_kdtree.cpp

query_kdtree: $(QUERY_OBJS)
	$(CXX) $(LDFLAGS) -o query_kdtree $(QUERY_OBJS) $(LDLIBS) 
query_kdtree.o:	query_kdtree.cpp  kdtree.h
	$(CXX) $(CPPFLAGS) -c query_kdtree.cpp

clean:
	$(RM) $(BUILD_OBJS) $(QUERY_OBJS) $(TEST_OBJS) *~
