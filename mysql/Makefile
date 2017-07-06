all: platform libmysql_operator.a mysql_test mysql_test2 test_lock

OS:=$(shell uname -s)

platform:
	@echo Platform: $(OS)

CFLAG:=-Wall -std=c++11 

INC:=/usr/local/include/mysql /usr/include/mysql
LIB_PATH:=.
LIB:=mysql_operator mysqlclient

ifeq "$(OS)" "Linux"
LIB_PATH+=/usr/lib64/mysql/ .
endif

ifeq "$(OS)" "Darwin"
LIB_PATH+=/usr/local/mysql/lib .
endif

clean:
	rm -rf mysql_operator.o
	rm -rf libmysql_operator.a
	rm -rf mysql_test
	rm -rf mysql_test.dSYM
	rm -rf mysql_test2
	rm -rf mysql_test2.dSYM
	rm -rf test_lock
	rm -rf test_lock.dSYM

mysql_operator.o: mysql_operator.cpp mysql_operator.h
	g++ $(CFLAG) $(addprefix -I, $(INC)) -c $< -o $@

libmysql_operator.a: mysql_operator.o
	ar -crus $@ $<
	
mysql_test: mysql_test.cpp 
	g++ $(CFLAG) -o $@ $(addprefix -I, $(INC)) $< $(addprefix -L, $(LIB_PATH)) $(addprefix -l, $(LIB))
	
mysql_test2: mysql_test2.cpp libmysql_operator.a
	g++ $(CFLAG) -o $@  $(addprefix -I, $(INC)) $< $(addprefix -L, $(LIB_PATH)) $(addprefix -l, $(LIB)) 
	
test_lock: test_lock.cpp libmysql_operator.a
	g++ $(CFLAG) -o $@  $(addprefix -I, $(INC)) $< $(addprefix -L, $(LIB_PATH)) $(addprefix -l, $(LIB)) 
