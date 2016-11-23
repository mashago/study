default:	platform proto test test2

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

clean:
	rm -rf masha.pb.*
	rm -rf test
	
proto: masha.proto
	protoc --proto_path=. --cpp_out=. masha.proto

# must add -lprotobuf
test: test.cpp
	g++ --std=c++0x -o test test.cpp masha.pb.cc -lpthread -lprotobuf

test2: test2.cpp
	g++ --std=c++0x -o test2 test2.cpp masha.pb.cc -lpthread -lprotobuf
