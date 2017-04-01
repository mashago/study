
default: MongoOperator


OS:=$(shell uname -s)


# FLAG:=-DTTT -D__MONGO_REPLICA_SET 
FLAG:=-DTTT 

ifeq "$(OS)" "Linux"
INC:=-I /usr/local/mongodb/include/
# for centos
# LINK:=/usr/local/mongodb/lib/libmongoclient.a /usr/lib64/libboost_regex-mt.so 
# LIB:=-lboost_thread-mt -lboost_filesystem -lboost_program_options -lssl -lcrypto
# for defora
LINK:=/usr/local/mongodb/lib/libmongoclient.a 
LIB:=-L/usr/lib64 -lboost_thread -lboost_filesystem -lboost_program_options -lboost_regex -lssl -lcrypto -lpthread
endif

ifeq "$(OS)" "Darwin"
INC:=-I /usr/local/mongodb/include/ -I ../jsoncpp/c++11
LINK:=/usr/local/mongodb/lib/libmongoclient.a
LIB:=-L/usr/local/lib -lboost_thread-mt -lboost_filesystem -lboost_program_options -lboost_regex-mt -lboost_system-mt -lssl -lcrypto
endif


clean:
	rm -rf mon2
	rm -rf dbio
	rm -rf MongoOperator

mon2: mon2.cpp
	g++ --std=c++11 $(INC) -o mon2 mon2.cpp $(LINK) $(LIB)

dbio: dbio.cpp Base64.cpp
	g++ --std=c++11 $(INC) -o dbio dbio.cpp Base64.cpp libjson.a $(LINK) $(LIB)

MongoOperator: MongoOperator.cpp Base64.cpp
	g++ -g --std=c++11 $(FLAG) $(INC) -o MongoOperator MongoOperator.cpp Base64.cpp libjson.a $(LINK) $(LIB)
