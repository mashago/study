
default:	platform stu lua_interpreter

CC:=gcc
CFLAG:= -Wall -std=c99 -g

OS:=$(shell uname -s)
INC:=
LIB:=-llua -ldl -lm
#USER_MACROS:=-DLUA_COMPAT_ALL -D__LUA_5_2
USER_MACROS:=-DLUA_COMPAT_ALL 


platform:
	@echo Platform: $(OS)

clean: 
	rm -rf stu
	rm -rf lua_interpreter
	rm -rf *.dSYM

stu:	stu.c
	$(CC) $(CFLAG) -o $@ $< $(USER_MACROS) $(INC) $(LIB)

lua_interpreter:	lua_interpreter.c
	$(CC) $(CFLAG) -o $@ $< $(USER_MACROS) $(INC) $(LIB)
