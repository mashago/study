#!/usr/local/bin/lua

require "util"

function test1()

	local f = loadfile("for_test_run.lua") -- loadfile() return compiled code as a lambda function 
	print(f)
	io.stdout:write("printHello:")
	print(printHello) -- the function in target lua still not define
	log()

	local ret = f() -- run target lua, as init, after this call, functions in target lua is defined
	print(ret)
	log()
	io.stdout:write("printHello:")
	print(printHello) -- function is defined
	log()

	f() -- run target lua again
	log()

	io.stdout:write("printHello:")
	print(printHello) -- function is defined
	printHello()

	return 0
end

function test2()

	-- dofile = loadfile + run lua
	local ret = dofile("for_test_run.lua")
	print(ret)
	io.stdout:write("printHello:")
	print(printHello)
	log()

	ret = dofile("for_test_run.lua")
	print(ret)
	io.stdout:write("printHello:")
	print(printHello)
	log()

	print(printHello) -- run target lua function
	printHello()


	return 0
end

function test3()

	g_i = 10
	local l_i = 20

	local f = loadstring("g_i = g_i + 1") -- loadstring use global var
	local g = function () l_i = l_i + 1 end
	f()
	f()
	print(g_i)
	log()

	g()
	g()
	print(l_i)
	log()

	do
		local words = "1 + 2 + 3 + 4"
		local func = assert(loadstring("return " .. words)) -- add return to become a block
		log("func()=%d", func())
		log()
	end

	do
		-- local words = "1 + 2 + 3 + 4" -- error, not a block
		local words = "return 1 + 2 + 3 + 4"
		local func = assert(loadstring(words))
		log("func()=%d", func())
		log()
	end

	do
		local words = "local x = ...; return x * 2" -- use va_arg
		local func = assert(loadstring(words))
		log("func()=%d", func(20))
		log()
	end

	do
		local str_table = '{k1="v1", k2="v2"}'
		local t = load('return ' .. str_table)()
		for k, v in pairs(t) do
			print(k, v)
		end
	end

	do
		local str_function = "return " .. "function (lv) return 5*lv+5 end"
		local func = load(str_function)()
		print("ret=", func(3))
	end

	return 0
end

function test4()

	while false
	do
		local n = 1
		if n ~= 2 then
			error("n ~= 2")
		end
	end

	while false
	do
		local n = 1
		assert(n == 2, "n ~= 2")
	end

	do
		local func1 = function()
			error("func1 raise a error")
		end

		-- protect call
		local status, msg = pcall(func1)
		if status then
			log("run func1() ok")
		else
			log(msg)
		end
		log()
	end

	do
		local func2 = function()
			-- error can accept everything
			error({msg = "func2 raise a error"})
		end

		-- protect call
		local status, ret = pcall(func2)
		if status then
			log("run func2() ok")
		else
			log(ret.msg)
		end
		log()
	end

	do
		local funcx = function()
			error("funcx raise a error")
		end

		local func3 = function()
			funcx()
		end

		-- execute protect call
		local status, ret = xpcall(func3, debug.traceback)
		if status then
			log("run func3() ok")
		else
			log(ret)
		end
		log()
	end

	return 0
end

function test5()
	print(debug.getinfo(1).currentline)
	print(debug.getinfo(0).name)

	return 0
end

function write_file(file_name, buffer)
    local f = assert(io.open(file_name, "w"))
    f:write(buffer)
    assert(f:close())
end  -- write_file()

function test6()
	local buffer1 = [[
local l_var_a = 123
local function l_func_a()
	print("call l_func_a")
end

g_var_a = 456
function g_func_a()
	print("call g_func_a")
end

function g_func_b()
	return l_var_a
end

local l_var_c = 111
function g_func_c()
	return l_var_c
end
]]
	local f1 = load(buffer1)

	local file_name = "for_test_module_tmp.lua"
	local buffer2 = [[
local l_var_a = 314
local function l_func_a()
	print("call l_func_a")
end

g_var_a = 789
function g_func_a()
	print("call g_func_a")
end

function g_func_b()
	return l_var_a
end

local l_var_c = 222
]]
	write_file(file_name, buffer2)

	local f2 = loadfile(file_name)

	print("type(f1)=", type(f1))
	print("type(f2)=", type(f2))

	local f1_ret = f1()
	print("type(f1_ret)=", type(f1_ret))
	print("l_var_a=", l_var_a)
	print("g_var_a=", g_var_a)
	print("g_func_b()=", g_func_b())
	print("g_func_c()=", g_func_c())
	for i = 1, math.huge do
		local name, value = debug.getupvalue(g_func_b, i)
		if not name then 
			break 
		end
		print("name=", name, " value=", value)
	end

	print()

	local f2_ret = f2()
	print("type(f1_ret)=", type(f1_ret))
	print("l_var_a=", l_var_a)
	print("g_var_a=", g_var_a)
	print("g_func_b()=", g_func_b())
	print("g_func_c()=", g_func_c())

	os.remove(file_name)

	return 0
end

function test7()
	print("_G=", _G)
	print("_ENV=", _ENV)
	_G.gt = 10
	print("gt=", gt)
    print("_ENV.gt=", _ENV.gt)
    print()

	local func = function()
		local print = _G.print
		local _ENV = {}
		print("gt=", gt)
        print("_G=", _G)
	end
	func()

	return 0
end

function test_notyet()
	return 0
end

test_list =
{
	test1
,	test2
,	test3
,	test4
,	test5
,	test6
,	test7
}

function do_main()
	print("hello " .. arg[0])
	print("_VERSION=" .. _VERSION)

	local maxcase = #test_list
	local testcase = maxcase

	if #arg > 0 then -- arg is the params from lua execute program
		-- run all case
		if arg[1] == "all" then
			print "run all case"
			for i, v in ipairs(test_list) do
				print(string.format("run case[%d]", i))
				local ret = v() or -1
				if ret ~= 0 then
					print(string.format("case[%d] ret=%d", i, ret))
					return
				end
				print ""
			end
			print "all case done"
			return
		end

		-- name one case
		testcase = tonumber(arg[1]) or maxcase
		if testcase <= 0 or testcase > maxcase then
			testcase = maxcase
		end
	end

	print(string.format("run case[%d]", testcase))
	local ret = test_list[testcase]() or -1
	if ret ~= 0 then
		print(string.format("case[%d] ret=%d", testcase, ret))
	end

end

do_main()

