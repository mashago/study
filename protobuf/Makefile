#default:	platform proto test test2 stu
default:	platform proto stu

OS:=$(shell uname -s)

CXX:=g++
CXXFLAG:=-Wall -std=c++11
LIB:=-lpthread -lprotobuf

platform:
	@echo Platform: $(OS)

clean:
	rm -rf masha.pb.*
	rm -rf test test2 stu
	
proto: masha.proto
	protoc --proto_path=. --cpp_out=. masha.proto

# must add -lprotobuf
test: test.cpp masha.pb.cc
	$(CXX) $(CXXFLAG) -o $@ $^ $(LIB)

test2: test2.cpp masha.pb.cc
	$(CXX) $(CXXFLAG) -o $@ $^ $(LIB)

stu: stu.cpp masha.pb.cc
	$(CXX) $(CXXFLAG) -o $@ $^ $(LIB)
