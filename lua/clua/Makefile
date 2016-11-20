
default:	platform stu lua_interpreter

CFLAG:= -Wall -std=c99 -g

OS:=$(shell uname -s)
LIB:=-llua -ldl -lm

platform:
	@echo Platform: $(OS)

clean: 
	rm -rf stu
	rm -rf lua_interpreter
	rm -rf *.dSYM

stu:	stu.c
	gcc $(CFLAG) -o $@ $< $(LIB)

lua_interpreter:	lua_interpreter.c
	gcc $(CFLAG) -o $@ $< $(LIB)
