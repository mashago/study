default: platform test test_set test_string test_hash

ACL_PATH=../acl
CFLAGS=\
-I. \
-I/usr/local/include \
-I$(ACL_PATH)/lib_acl_cpp/include \
-I$(ACL_PATH)/lib_acl/include \
-I$(ACL_PATH)/lib_protocol/include 


LFLAGS=\
-lpthread \
-L$(ACL_PATH)/lib_acl_cpp/lib -l_acl_cpp \
-L$(ACL_PATH)/lib_protocol/lib -l_protocol \
-L$(ACL_PATH)/lib_acl/lib -l_acl \
-L/usr/local/lib/

OS:=$(shell uname -sm)

ifeq ($(findstring Linux, $(OS)), Linux)
	CFLAGS += -DLINUX2
endif

ifeq ($(findstring Darwin, $(OS)), Darwin)
	CFLAGS += -DMACOSX
endif


platform:
	@echo Platform: $(OS)

clean:
	rm -rf test
	rm -rf test_set
	rm -rf test_string
	rm -rf test_hash

test: test.cpp
	g++ -Wall -std=c++0x $(CFLAGS) -o test test.cpp $(LFLAGS)
	
test_set: test_set.cpp
	g++ -Wall -std=c++0x $(CFLAGS) -o test_set test_set.cpp $(LFLAGS)

test_string: test_string.cpp
	g++ -Wall -std=c++0x $(CFLAGS) -o test_string test_string.cpp $(LFLAGS)

test_hash: test_hash.cpp
	g++ -Wall -std=c++0x $(CFLAGS) -o test_hash test_hash.cpp $(LFLAGS)
