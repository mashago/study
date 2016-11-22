# here for c++0x
#default: platform str_queue tp worker stack_tp tv queue_tp stu st smart_pointer frnd2tmp member_tp container stream variadic_tp function_tp class_test config_test type_tp macro_test stress order_queue_tp time_func string_tool my_string acct error_means rtti read_file sensitive_word_filter.o libsensitive_word_filter.a  sensitive_word_filter_test animal 

# here for c++11
# default: platform str_queue tp worker stack_tp tv queue_tp stu st smart_pointer frnd2tmp member_tp container stream variadic_tp function_tp class_test config_test type_tp macro_test stress order_queue_tp time_func string_tool my_string acct error_means rtti rvalue lambda wrapper 
default: platform str_queue tp worker stack_tp tv queue_tp stu st smart_pointer frnd2tmp member_tp container stream variadic_tp function_tp class_test config_test type_tp macro_test stress order_queue_tp time_func string_tool my_string acct error_means rtti 

OS:=$(shell uname -s)

# CFLAG:=-Wall -std=c++11 -DCPP11
CFLAG:=-Wall -std=c++0x

platform:
	@echo Platform: $(OS)

clean:
	rm -rf stu
	rm -rf stu3
	rm -rf st
	rm -rf str_queue
	rm -rf my_string
	rm -rf tp
	rm -rf acct
	rm -rf worker
	rm -rf stack_tp
	rm -rf tv
	rm -rf queue_tp
	rm -rf error_means
	rm -rf rtti
	rm -rf smart_pointer
	rm -rf frnd2tmp
	rm -rf member_tp
	rm -rf container
	rm -rf stream
	rm -rf rvalue
	rm -rf lambda
	rm -rf wrapper
	rm -rf variadic_tp
	rm -rf function_tp
	rm -rf class_test
	rm -rf config_test
	rm -rf type_tp
	rm -rf macro_test
	rm -rf stress
	rm -rf order_queue_tp
	rm -rf time_func
	rm -rf string_tool
	rm -rf read_file
	rm -rf sensitive_word_filter.o
	rm -rf libsensitive_word_filter.a
	rm -rf sensitive_word_filter_test
	rm -rf animal
	rm -rf *.dSYM

stu:	stu.cpp stu.h stu2.cpp stock.h stock.cpp counter.h counter.cpp tc_a.h tc_a.cpp
	g++ -g3 $(CFLAG) -o stu stu.cpp stu2.cpp stock.cpp counter.cpp tc_a.cpp 

stu3:	stu3.cpp stu2.cpp
	g++ $(CFLAG) -o stu3 stu2.cpp stu3.cpp

st:	sizeof_test.cpp
	g++ $(CFLAG) -o st sizeof_test.cpp

str_queue:	str_queue.cpp str_queue.h
	g++ $(CFLAG) -o str_queue str_queue.cpp -lpthread

my_string:	my_string.cpp my_string.h
	g++ $(CFLAG) -o my_string my_string.cpp

tp:	tennis_player.cpp tennis_player.h
	g++ $(CFLAG) -o tp tennis_player.cpp

acct:	acctabc.cpp acctabc.h
	g++ $(CFLAG) -o acct acctabc.cpp

worker:	worker.cpp worker.h
	g++ $(CFLAG) -o worker worker.cpp

stack_tp:	stack_tp.cpp stack_tp.h
	g++ $(CFLAG) -o stack_tp stack_tp.cpp

tv:	tv.cpp tv.h
	g++ $(CFLAG) -o tv tv.cpp

queue_tp:	queue_tp.cpp queue_tp.h
	g++ $(CFLAG) -o queue_tp queue_tp.cpp

error_means:	error_means.cpp 
	g++ $(CFLAG) -o error_means error_means.cpp

rtti:	rtti.cpp 
	g++ $(CFLAG) -o rtti rtti.cpp

smart_pointer:	smart_pointer.cpp 
	g++ $(CFLAG) -o smart_pointer smart_pointer.cpp

frnd2tmp:	frnd2tmp.cpp 
	g++ $(CFLAG) -o frnd2tmp frnd2tmp.cpp

member_tp:	member_tp.cpp 
	g++ $(CFLAG) -o member_tp member_tp.cpp

container:	container.cpp 
	g++ $(CFLAG) -o container container.cpp

stream:	stream.cpp 
	g++ $(CFLAG) -o stream stream.cpp

rvalue:	rvalue_reference.cpp 
	g++ $(CFLAG) -o rvalue rvalue_reference.cpp

lambda:	lambda.cpp 
	g++ $(CFLAG) -o lambda lambda.cpp

wrapper:	wrapper.cpp 
	g++ $(CFLAG) -o wrapper wrapper.cpp

variadic_tp:	variadic_tp.cpp 
	g++ $(CFLAG) -o variadic_tp variadic_tp.cpp

function_tp:	function_tp.cpp 
	g++ $(CFLAG) -o function_tp function_tp.cpp

class_test:	class_test.h class_test.cpp class_test_run.cpp
	g++ $(CFLAG) -o class_test class_test_run.cpp class_test.cpp

config_test: config_test.cpp 
	g++ $(CFLAG) -o config_test config_test.cpp

type_tp: type_tp.cpp 
	g++ $(CFLAG) -o type_tp type_tp.cpp

macro_test: macro_test.cpp 
	g++ $(CFLAG) -g -D_MY_DEF_D -o macro_test macro_test.cpp

stress: stress.cpp 
	g++ -g $(CFLAG) -o stress stress.cpp

order_queue_tp: order_queue_tp.cpp 
	g++ -g $(CFLAG) -o order_queue_tp order_queue_tp.cpp

time_func: time_func.cpp 
	g++ -g $(CFLAG) -o time_func time_func.cpp

string_tool: string_tool.cpp 
	g++ -g $(CFLAG) -o string_tool string_tool.cpp

read_file: read_file.cpp 
	g++ -g $(CFLAG) -o read_file read_file.cpp

sensitive_word_filter.o: sensitive_word_filter.cpp sensitive_word_filter.h
	g++ -g $(CFLAG) -c -o $@ $<

libsensitive_word_filter.a: sensitive_word_filter.o 
	ar -crus $@ $<

sensitive_word_filter_test: sensitive_word_filter_test.cpp libsensitive_word_filter.a
	g++ -g $(CFLAG) -o $@ $^

animal: animal.cpp animal.h
	g++ -g $(CFLAG) -o $@ $<
