#!/usr/local/bin/lua

function test1()
	print("type(nil)=" .. (type(nil)))

	print("type(true)=" .. type(true))

	local a = 10
	print("type(a)=" .. type(a))

	print("type(\"Hello World\")=" ..  type("Hello World"))

	local t = {}
	print("type(t)=" .. type(t))

	print("type(print)=" .. type(print))

	return 0
end


function test2()
	-- about true or false
	local flag = 0
	if flag then
		print(flag, "is true")
	end

	flag = ""
	if flag then
		print(flag, "is true")
	end

	flag = nil
	if not flag then
		print(flag, "is false")
	end

	flag = false
	if not flag then
		print(flag, "is false")
	end

	return 0
end


function test3()
-- {
	-- about string
	local s1 = "good morning"
	print("1 s1=" .. s1)
	s1 = [[
	haha
	bcbc
	123456 ]]
	print("2 s1=" .. s1)
	print "print hello without ()"

	print()

	g_string_b = "11" * 2
	print("1 g_string_b=" .. g_string_b)
	print("1 type(g_string_b)=" .. type(g_string_b))

	g_string_b = "11" * "2"
	print("2 g_string_b=" .. g_string_b)
	print("2 type(g_string_b)=" .. type(g_string_b))

	g_string_b = "11 * 2"
	print("3 g_string_b=" .. g_string_b)
	print("3 type(g_string_b)=" .. type(g_string_b))
	print("#g_string_b=" .. #g_string_b)

	print()

	g_string_c = "123Hello World"
	print("g_string_c=" .. g_string_c)

	print("string.upper(g_string_c)=" .. string.upper(g_string_c))
	print("string.lower(g_string_c)=" .. string.lower(g_string_c))
	-- string.gsub(input, find string, replace string, times)
	print("string.gsub(g_string_c, \"o\", \"O\")=" .. string.gsub(g_string_c, "o", "O"))
	print("string.gsub(g_string_c, \"o\", \"O\", 1)=" .. string.gsub(g_string_c, "o", "O", 1))
	print("string.find(g_string_c, \"123\", 1)=" .. string.find(g_string_c, "123", 1))
	print("string.find(g_string_c, \"456\", 1)=", string.find(g_string_c, "456", 1))
	print("string.reverse(g_string_c)=" .. string.reverse(g_string_c))

	g_string_fmt = "%d %s"
	print("string.format(g_string_fmt)=" .. string.format(g_string_fmt, 10, "hahaha"))

	print("string.char(97, 98, 99, 100)=" .. string.char(97, 98, 99, 100))

	-- string.byte(input, index = 1)
	print("string.byte(\"abc\")=" .. string.byte("abc"))
	print("string.byte(\"abc\", 2)=" .. string.byte("abc", 2))
	print("string.len(g_string_c)=" .. string.len(g_string_c))
	print("string.rep(g_string_c, 1)=" .. string.rep(g_string_c, 1))
	print("string.rep(g_string_c, 2)=" .. string.rep(g_string_c, 2))
	return 0
-- }
end

function test3()
	x = 1
	local x = 20
	local f = loadstring("x = x + 1; print(\"x=\" .. x)") -- loadstring only can access global var
	f()
	print("x=" .. x)

	local l = "1 + 2"
	local f2 = assert(loadstring("return " .. l))
	print("f2()=" .. f2())

	-- ... is a input
	local l2 = "local a = ...; return a + 1"
	local f3 = assert(loadstring(l2))
	print("f3(10)=" .. f3(10))

	print(loadstring("i i"))

	return 0
end

function test4()
	-- about error
	
	local flag = false

	--[[
	local r1 = assert(flag)
	print("r1=", r1)
	--]]

	--[[
	local r2 = assert(flag, "flag is false") -- print in assert
	print("r2=", r1)
	--]]
	
	--[=[
	error("create one error")
	print("after error")
	--]=]
	
	-- protect call
	local status, ret = pcall(function () error{code=1011} end)
	print("status=", status, " ret.code=", ret.code)

	status, ret = pcall(function () return "good" end)
	print("status=", status, " ret=", ret)

	print(debug.traceback())

	return 0
end

function test5()
	local t = {a=true, b=false, c=true}
	-- local t = {true, false, true}

	-- use a repeat as a continue
	for k, v in pairs(t) do
		repeat
			if not v then
				print(k .. " not fit")
				break -- early continue
			end
			-- my logic
			print(k .. " fit")
		until true
	end
	print()

	-- break can use in do ... end
	for k, v in pairs(t) do
		do	
			if not v then
				print(k .. " not fit")
				break -- early continue
			end
			-- my logic
			print(k .. " fit")
		end
	end

	return 0
end

function test6()

	-- about module and require

	-- after require, module table will save into package.loaded
	local m1 = require("my_module") -- "my_module init()"
	print("m1=", m1)

	-- use return table to call table function
	m1.print_hello(1) -- can access my_module.lua function

	-- when do require, will find if already in package.loaded
	m1 = require("my_module") -- will not print "my_module init()"
	print("m1=", m1)

	m1.print_hello(2) -- can access my_module.lua function
	my_module.print_hello(3) -- use my_module directly, because module in _G
	print("m1=", m1, " my_module=", my_module)

	print()

	if _VERSION == "Lua 5.1" then
		-- my_module2 use setfenv() inside, therefore lua5.2 cannot require it
		local m2 = require("my_module2")
		print("m2=", m2)
		m2.print_hello(1)
		my_module2.print_hello(2)
		print("m2=", m2, " my_module2=", my_module2)

		print()
	end

	local m3 = require("my_module3")
	print("m3=", m3)
	m3.print_hello(1)
	my_module3.print_hello(2)
	print("m3=", m3, " my_module3=", my_module3)

	print()

	-- about environment
	if _VERSION == "Lua 5.1" then
		-- only can run in lua5.1
		local gt = getfenv() -- return env, is _G
		print("gt=", gt, " _G=", _G)
		if gt == _G then
			print("gt == _G")
		else
			print("gt ~= _G")
		end

		do
			local _G = {} -- will not cover the real _G
			print("hello 1") -- still can use print
		end
		local oldEnv = getfenv()
		setfenv(1, {}) -- now, the real _G is empty
		-- print("hello 2") -- cannot get print anymore
		oldEnv.setfenv(1, oldEnv)
		print("hello 3") -- ok

		print()
		local f1 = function ()
			print("hello f before setfenv")
			local newEnv = {}
			setfenv(1, newEnv)
			-- print("hello f after setfenv") -- print not exists
		end
		f1()
		print("after f1()")
	end

	return 0
end

	
-- local w = debug.getinfo(2, "S").what
-- print("w=", w) -- w is "C"

function test7()
	
	local w = debug.getinfo(2, "S").what
	print("w=", w) -- w is "Lua"
	
	return 0
end


--
BaseClass = { x = 0, y = 0}
BaseClass.__index = BaseClass -- means BaseClass = {__index = BaseClass}, for object setmetatabe

function BaseClass:new(x, y)
	x = x or 0
	y = y or 0

	local self = {}
	setmetatable(self, BaseClass) -- means obj = {..., MT={__index = BaseClass}}

	self.x = x
	self.y = y

	return self
end

function BaseClass:info()
	print("x=", self.x, " y=", self.y)
end

function BaseClass:plus()
	self.x = self.x + 1
	self.y = self.y + 1
end

function BaseClass:step()
	return 5
end

function BaseClass:go()
	self.x = self.x + self.step()
end
--
--
-- an extend class
SecondClass = { z = 0 }
setmetatable(SecondClass, BaseClass) -- extend, means SecondClass = {..., MT=BaseClass={__index = BaseClass}}
SecondClass.__index = SecondClass -- means SecondClass = {..., __index = SecondClass, MT}

function SecondClass:new(x, y, z)
	x = x or 0
	y = y or 0
	z = z or 0

	local self = {}
	self = BaseClass:new(x, y)
	setmetatable(self, SecondClass) -- obj = {..., MT=SecondClass}

	self.z = z

	return self
end

function SecondClass:info()
	print("x=", self.x, " y=", self.y, " z=", self.z)
end

function SecondClass:plus()
	self.x = self.x + 1
	self.y = self.y + 1
	self.z = self.z + 1
end

function SecondClass:step() -- override
	return 10
end
--
-- another way to define a class
CatClass = {
	x = 0, y = 0,
	new = function (self, x, y)
		x = x or 0
		y = y or 0

		local proxy = {}
		-- setmetatable(proxy, CatClass)
		setmetatable(proxy, self)

		proxy.x = x
		proxy.y = y

		return proxy
	end,

	info = function (self)
		print("x=", self.x, " y=", self.y)
	end,

	plus = function (self)
		self.x = self.x + 1
		self.y = self.y + 1
	end,

	step = function (self)
		return 5
	end,

	go = function (self)
		self.x = self.x + self.step()
	end,
}
CatClass.__index = CatClass

-- an extend class
MadCatClass = {
	z = 0,

	new = function (self, x, y, z)
		x = x or 0
		y = y or 0
		z = z or 0

		local proxy = {}
		proxy = CatClass:new(x, y)
		setmetatable(proxy, self)

		proxy.z = z

		return proxy
	end,

	info = function (self)
		print("x=", self.x, " y=", self.y, " z=", self.z)
	end,

	plus = function (self)
		self.x = self.x + 1
		self.y = self.y + 1
		self.z = self.z + 1
	end,

	step = function (self) -- override
		return 10
	end,
}
setmetatable(MadCatClass, CatClass) -- extend
MadCatClass.__index = MadCatClass
--

function test8()
	print("BaseClass:")
	local b1 = BaseClass:new(4, 5)
	-- b1 way to get info():
	-- 1. b1 find itself "info", not exists
	-- 2. b1 find itself MT, exists, MT is BaseClass
	-- 3. BaseClass find "__index", exists, is BaseClass
	-- 4. BaseClass find "info", exists, call it
	b1:info()
	b1:plus()
	b1:info()
	local info_no = b1.info_no
	print()

	local b2 = BaseClass:new(6, 8)
	b2:info()
	b2:plus()
	b2:info()
	b1:info()
	print()

	print("SecondClass:")
	local s1 = SecondClass:new(1, 2, 3)
	-- s1 way to get info():
	-- 1. s1 find itself "info", not exists
	-- 2. s1 find itself MT, exists, MT is SecondClass
	-- 3. SecondClass find "__index", exists, is SecondClass
	-- 4. SecondClass find "info", exists, call it
	s1:info()
	s1:plus()
	s1:info()
	print()

	b1:info()
	s1:info()
	b1:go()

	-- s1 way to get go():
	-- 1. s1 find itself "go", not exists
	-- 2. s1 find itself MT, exists, MT is SecondClass
	-- 3. SecondClass find "__index", exists, is SecondClass
	-- 4. SecondClass find "go", not exists
	-- 5. SecondClass find MT, exists, MT is BaseClass
	-- 6. BaseClass find "__index", exists, is BaseClass
	-- 7. BaseClass find "go" exists, call it
	s1:go() -- call BaseClass::go()
	b1:info()
	s1:info()
	print()

	print("CatClass:")
	local c1 = CatClass:new(10, 12)
	c1:info()
	c1:go()
	c1:plus()
	c1:info()
	print()
	local c2 = CatClass:new(50, 50)
	c2:info()
	c2:go()
	c2:plus()
	c2:info()
	c1:info()
	print()

	print("MadCatClass:")
	local m1 = MadCatClass:new(10, 12, 14)
	m1:info()
	m1:go()
	m1:plus()
	m1:info()
	print()
	local m2 = MadCatClass:new(50, 50, 50)
	m2:info()
	m2:go()
	m2:plus()
	m2:info()
	m1:info()
	print()

	--[[
	-- stupid logic, dead loop
	local t = {}
	setmetatable(t, t)
	t.__index = t
	local a = t.a
	--]]

	return 0
end

function test9()
	local x = "x1"
	local my_table = {x=1} -- means key is "x", value is 1

	print("my_table[x]=", my_table[x]) -- == my_table["x1"]
	print("my_table.x=", my_table.x) -- == my_table["x"]
	-- print("my_table.\"x\"=", my_table."x") -- error
	print("my_table[\"x\"]=", my_table["x"])

	print()

	local i, j, k = 1, "1", "+1"
	my_table[i] = "iii"
	my_table[j] = "jjj"
	my_table[k] = "kkk"
	print("my_table[i]=", my_table[i])
	print("my_table[j]=", my_table[j])
	print("my_table[k]=", my_table[k])

	print()

	my_table = {"a1", "a2", [4]="a4"}
	print("my_table[1]=", my_table[1], " table.maxn(my_table)=", table.maxn(my_table))
	my_table[1000] = "a1000"
	print("my_table[1000]=", my_table[1000], " table.maxn(my_table)=", table.maxn(my_table))

	return 0
end

function test10_foo1()
end

function test10_foo2()
	return "a"
end

function test10_foo3()
	return "b", "c"
end

function test10_main(n)
	if n == 1 then
		return test10_foo1()
	elseif n == 2 then
		return test10_foo2()
	elseif n == 3 then
		return test10_foo3()
	elseif n == 4 then
		return test10_foo2(), test10_foo3()
	elseif n == 5 then
		return test10_foo3(), test10_foo3() -- only last return can return all its return value
	end
end

function test10()

	local r1, r2, r3, r4 = test10_main(1)
	print("r1=", r1, " r2=", r2, " r3=", r3, " r4=", r4)
	r1, r2, r3, r4 = test10_main(2)
	print("r1=", r1, " r2=", r2, " r3=", r3, " r4=", r4)
	r1, r2, r3, r4 = test10_main(3)
	print("r1=", r1, " r2=", r2, " r3=", r3, " r4=", r4)
	r1, r2, r3, r4 = test10_main(4)
	print("r1=", r1, " r2=", r2, " r3=", r3, " r4=", r4)
	r1, r2, r3, r4 = test10_main(5)
	print("r1=", r1, " r2=", r2, " r3=", r3, " r4=", r4)

	return 0
end

function test11()
	local v = nil
	if v then
		print(v, "is true")
	else
		print(v, "is false")
	end

	v = false
	if v then
		print(v, "is true")
	else
		print(v, "is false")
	end

	v = ""
	if v then
		print(v, "is true")
	else
		print(v, "is false")
	end

	v = 0
	if v then
		print(v, "is true")
	else
		print(v, "is false")
	end

	v = 1
	if v then
		print(v, "is true")
	else
		print(v, "is false")
	end


	return 0
end

function test12()
	local buffer = [[
	this is a long words.
	and \n will not work.]]
	print(buffer)

	print ""

	buffer = [=[
	this is another words.
	[[ use different block key word ]]
	]=]
	print(buffer)

	buffer = "masha"
	print("#buffer=", #buffer)
	return 0
end

function test13()
	
	local a, b = 1, 2
	print("a=", a, " b=", b)
	a, b = b, a
	print("a=", a, " b=", b)

	return 0
end

function test14()

	local count = 0
	while (count < 5) do 
		print("count=" .. count)
		count = count + 1
		if count == 3 then
			break -- use break to end loop
		end
	end

	print ""

	count = 0
	repeat
		print("count=" .. count)
		count = count + 1
	until count > 3

	print ""

	-- for x=start, end(incloud), step(option, default 1)
	for i = 0, 4, 1 do
		print("i=" .. i)
	end
	print("i=", i)

	print()

	for i = 3, 0, -1 do
		print("i=" .. i)
	end

	print()

	-- double for
	for i = 1, 3, 1 do
		for j = 1, 2, 1 do
			print("i=" .. i .. " j=" .. j)
		end
	end

	print()
	local my_table = {"aaa", "bbb", 123, [6]="x6"}
	print("#my_table=", #my_table, "table.maxn(my_table)=", table.maxn(my_table))
	-- ipairs() get array pair
	for k, v in ipairs(my_table) do
		print(k .. " : " .. v)
	end

	print()

	my_table.x5 = "v5"
	for k, v in pairs(my_table) do
		print(k .. " : " .. v)
	end
	
	return 0
end

function test15_add(...)
	local s = 0
	for _, v in ipairs({...}) do
		s = s + v
	end
	return s
end

function test15_select(...)
	print("select('#', ...)=", select('#', ...))
	for i=1, select('#', ...) do
		print(string.format("select(%d, ...)=", i), select(i, ...))
	end
end

function test15_unpack(...)
	print(unpack(...))
end

function test15()

	local s = test15_add(1, 2, 3, 4, 5)
	print("s=", s)
	print()

	test15_select(11, 12, 13, 14, 15)
	print()

	local t = {"a", "b", "hello"}
	print(unpack(t))
	print(type(unpack(t)))
	test15_unpack(t)

	return 0
end

function trim(s)
	-- ^ is begin from 
	-- %s is space. * is match repeated 0 or many times, prefer longer
	-- . is char. - is match repeated 0 or many times, prefer shorter
	-- $ is end with
	return (string.match(s, "^%s*(.-)%s*$"))
end

function test16()
	local buffer = ""

	buffer = ""
	buffer = trim(buffer)
	print("0 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "hello"
	buffer = trim(buffer)
	print("1 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = " hello"
	buffer = trim(buffer)
	print("2 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "hello "
	buffer = trim(buffer)
	print("3 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = " hello "
	buffer = trim(buffer)
	print("4 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "  hello  "
	buffer = trim(buffer)
	print("5 after trim: buffer=[" .. buffer .. "]")
	print()
	print()

	buffer = "hello world"
	buffer = trim(buffer)
	print("1 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "hello  world"
	buffer = trim(buffer)
	print("2 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = " hello world"
	buffer = trim(buffer)
	print("3 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "  hello world"
	buffer = trim(buffer)
	print("4 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "hello world "
	buffer = trim(buffer)
	print("5 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "hello world  "
	buffer = trim(buffer)
	print("6 after trim: buffer=[" .. buffer .. "]")
	print()

	buffer = "  hello  world  "
	buffer = trim(buffer)
	print("7 after trim: buffer=[" .. buffer .. "]")
	print()

	return 0
end


function test17()
-- [

	local buffer = ""

	-- replace from string
	-- %w is letters and integer
	buffer = "$player is best"
	buffer = string.gsub(buffer, "$%w+", "masha")
	print("1 buffer=[" .. buffer .. "]")

	buffer = "$player is best"
	buffer = string.gsub(buffer, "$(%w+)", "masha")
	print("2 buffer=[" .. buffer .. "]")

	buffer = "$player is best"
	buffer = string.gsub(buffer, "$(%w+)", "%1")
	print("3 buffer=[" .. buffer .. "]")

	local my_table = {player = "masha"}

	-- replace from table
	buffer = "player is best"
	buffer = string.gsub(buffer, "%w+", my_table)
	print("4 buffer=[" .. buffer .. "]")

	-- replace from table, search capture data in table
	buffer = "$player is best"
	buffer = string.gsub(buffer, "$(%w+)", my_table)
	print("5 buffer=[" .. buffer .. "]")

	-- replace from function
	buffer = "$player is best, $a is loser"
	buffer = string.gsub(buffer, "$(%w+)", function (n) return tostring(my_table[n]) end)
	print("6 buffer=[" .. buffer .. "]")

	-- capture empty
	print(string.match("abcde", "()b()c()"))
	print()

	-- \x ==> char(x)
	buffer = "%123abc&567def*"
	print(string.match(buffer, "[\97-\122]+"))
	for w in string.gmatch(buffer, "[\97-\122]+") do
		print("w=" .. w)
	end

	buffer = "$Rev: 3588 $"
	print(string.gsub(buffer, "[ ]*$[a-zA-Z:]*[ ]*", ""))
	print(string.match(buffer, "%d+"))

	return 0
end -- test17 end ]


function url_escape(s)
	s = string.gsub(s, "[&=%%%c]", function (c)
		return string.format("%%%02X", string.byte(c))
	end)
	s = string.gsub(s, " ", "+")
	return s
end

function url_encode(params)
	local b = {}
	for k, v in pairs(params) do
		b[#b+1] = (url_escape(k) .. "=" .. url_escape(v))
	end
	return table.concat(b, "&")
end

function url_unescape(s)
	s = string.gsub(s, "+", " ")
	s = string.gsub(s, "%%(%x%x)", function (h)
		return string.char(tonumber(h, 16))
	end)
	return s
end

function url_decode(s)
	local params = {}
	for k, v in string.gmatch(s, "([^&=]+)=([^&=]+)") do
		k = url_unescape(k)
		v = url_unescape(v)
		params[k] = v
	end
	return params
end

function test18()

	local params = { a = 10, b = "hello world", c = "&1%2ha ha" }
	print("params:")
	for k, v in pairs(params) do
		print("k=" .. k .. " v=" .. v)
	end
	print()

	local s = url_encode(params)
	print("s=" .. s)
	print()

	local t = url_decode(s)
	print("t:")
	for k, v in pairs(t) do
		print("k=" .. k .. " v=" .. v)
	end

	return 0
end

function test19_update_table(t)
	if type(t) ~= "table" then
		return t
	end

	print(t)

	t["a"] = 1
	t.b = 2
end

function test19()
	local t = { a = "hello" }
	print("before")
	print(t)
	for k, v in pairs(t) do
		print("k=" .. k .. " v=" .. v)
	end

	test19_update_table(t)
	print("after")
	print(t)
	for k, v in pairs(t) do
		print("k=" .. k .. " v=" .. v)
	end
	
	return 0
end

-- my printf(), like c
function printf(format, ...)
	return io.stdout:write(string.format(format, ...))
end

function test20()

	io.write("hello test20\n")

	local filename = ""

	do
		local input = io.input()
		filename = "test.lua"
		io.input(filename)
		local line_num = 0
		for line in io.lines() do
			line_num = line_num + 1
		end
		printf("line_num=%d\n", line_num)
		io.input(input)
		printf("\n")
	end
		

	filename = "non_existent_file.txt"
	local f, ret = io.open(filename, "r")
	if f == nil then
		-- io.stdout:write(ret, "\n\n")
		printf("%s\n", ret)
	end

	filename = "test.lua"
	f, ret = io.open(filename, "r")
	if f ~= nil then
		-- pos = f:seek(whence="cur", offset=0)
		local pos = f:seek()
		io.stdout:write("pos=", pos, "\n")
		local size = f:seek("end") -- from end
		io.stdout:write("size=", size, "\n")

		f:seek("set", 0)
		local line = f:read("*line")
		io.stdout:write(filename, " first line:\n")
		io.stdout:write(line, "\n")

		pos = f:seek()
		printf("after read a line pos=%d\n", pos)

		f:seek("set")
		local t = f:read("*all")
		printf("#t=%d\n", #t)

		f:close()
	end
	printf("\n")

	filename = "test.lua"
	f, ret = io.open(filename, "r")
	if f ~= nil then
		local BUFFER_SIZE = 2^10

		local total = 0
		while true do
			local lines, rest = f:read(BUFFER_SIZE, "*line")
			if not lines then break end
			if rest then lines = lines .. rest .. "\n" end
			total = total + #lines
			printf("#lines=%d\n", #lines)
		end
		printf("total=%d\n", total)

		f:close()
	end


	return 0
end

function test21()
	-- os library

	-- about time and date
	print("os.time()=", os.time())
	print("os.time{year=2016, month=10, day=30, hour=0}=", os.time{year=2016, month=10, day=30, hour=0})

	local now = os.time()
	local detail = os.date("*t", now)
	print("detail:")
	for k, v in pairs(detail) do
		print(k, v)
	end
	print()
	print(os.date("%Y-%m-%d %H:%M:%S %A"))
	print("os.clock()=", os.clock())
	print()

	-- about other
	print("os.getenv(\"HOME\")=", os.getenv("HOME"))
	print("os.execute(\"ls\")=", os.execute("ls"))

	return 0
end

function my_traceback()
	for level = 1, math.huge do
		local info = debug.getinfo(level, "Sl")
		if not info then break end
		if info.what == "C" then
			print(level, "C function")
		else
			printf("%d [%s]:%d\n", level, info.short_src, info.currentline)
		end
	end
end

function test22()
	-- about debug
	my_traceback()
	print()

	local a = 10
	local b = 20
	local x
	do 
		local c = a - b
	end

	local a = 1
	while true do
		local name, value = debug.getlocal(1, a)
		if not name then break end
		print(name, value)
		a = a + 1
	end

	return 0
end

function create_closure(n)
	local i = 10; -- upvalue
	return function()
		i = i+1;
		return n * i
	end
end

test23 = function()

	local c1 = create_closure(1);
	print(c1())
	print(c1())
	print();

	local c2 = create_closure(2);
	print(c2())
	print(c2())
	print();

	print(c1())

	return 0
end

function test24()
	local mylib = {}
	mylib.foo = function (x, y) return x + y end
	mylib.goo = function (x, y) return x - y end

	print(mylib.foo(5, 10))
	print(mylib.goo(5, 10))

	return 0
end

function test25()
	-- coroutine
	local co_a
	co_a = coroutine.create(
		function(n)
			print("inside status=" .. coroutine.status(co_a))
			local r = coroutine.yield(n + 1)
			print("r=" .. r)
			return n * r
		end
	)

	print("type(co_a)=" .. type(co_a))

	print("before resume status=" .. coroutine.status(co_a))

	local flag, ret = coroutine.resume(co_a, 10)
	print("after resume status=" .. coroutine.status(co_a))
	print("flag=", (flag and "true") or "false", "ret=", ret)

	flag, ret = coroutine.resume(co_a, 10)
	print("flag=", (flag and "true") or "false", "ret=", ret)
	print("final status=" .. coroutine.status(co_a))

	flag, ret = coroutine.resume(co_a, 10)
	print("flag=", (flag and "true") or "false", "ret=", ret)

	return 0
end

function test26()

	local send = function(n)
		coroutine.yield(n)
	end

	local product = function(n)
		while true do
			n = n * 2
			send(n)
		end
	end

	local productor = coroutine.create(product)

	function recv(n)
		local status, item = coroutine.resume(productor, n)
		return item
	end

	function consume()
		for i = 1, 5 do
			local item = recv(i)
			print("item=" .. item)
		end
	end

	consume()

	return 0
end

function test27()
	math.randomseed(os.time())
	for i=1, 20 do
		local n = math.random(100) -- 1 to 100
		print("n=", n)
	end

	local k = 0.1
	local r = math.floor(k * 100)
	print("r=", r)

	print()

	for i=1, 20 do
		local n = math.random(3) -- 1 to 3
		print("n=", n)
	end

	print("math.floor(0.3)=", math.floor(0.3))
	print("math.floor(1.3)=", math.floor(1.3))

	for i=1, 5 do
		local r = math.random() -- 0 to 1
		print("r=", r)
	end
	print()

	for i=1, 5 do
		local r = math.random(1, 5) -- 1 to 5
		print("r=", r)
	end
	print()

	for i=1, 10 do
		local r = math.random(100000000000000000, 600000000000000000)
		print("r=", "XMXP" .. string.format("%d", r))
	end
	print()

	for i=1, 5 do
		local r = math.random(2, 2) -- 1 to 1
		print("r=", r)
	end

	return 0
end

function test28()
	local t = {}
	local key = "x"
	if not t[key] then
		print("t[key] nil")
	else
		print("t[key] not nil")
	end
	return 0
end

function test29()

	local n = 2 ^ 31 - 1
	print("n=", n)

	n = 101
	local r = n / 100
	print("n=", n, " r=", r)

	n = 190
	r = math.floor(n / 100)
	print("n=", n, " r=", r)

	n = 190
	r = math.modf(n / 100)
	print("n=", n, " r=", r)
	
	return 0
end

function test30()

	local n = 10
	local ret

	ret = n < 0 and 0 or n
	print("n=", n, " ret=", ret)

	n = -1
	ret = n < 0 and 0 or n
	print("n=", n, " ret=", ret)
	
	return 0
end

function test31()
	local str = "{0, 100}, {101, 200}, {201, 300}"
	local t = load('return ' .. '{' .. str .. '}')()

	print(t[1][1])
	print(t[1][2])
	print(t[2][1])
	print(t[2][2])
	print(t[3][1])
	print(t[3][2])

	return 0
end

function test32()
	math.randomseed(os.time())
	local lv = 30
	local day = 300
	local r = 0.8 + math.random(0, 40) / 100
	print("r=" .. r)
	local time = lv / 5 * (0.5 + 10/(day+10)) * r
	print("time=" .. time)

	return 0
end

function test33()
	math.randomseed(os.time())
	local str = ""
	for i=1, 10 do
		local r = math.random(0, 1)
		str = str .. r .. " "
	end
	print("random(0,1)=" .. str)

	local str = ""
	for i=1, 10 do
		local r = math.random(1, 1)
		str = str .. r .. " "
	end
	print("random(1,1)=" .. str)

	return 0
end

function get_extension_point(from_x, from_y, to_x, to_y, distance)
	local side_x_m = 1
	if from_x > to_x then
		side_x_m = -1
	end
	local side_y_m = 1
	if from_y > to_y then
		side_y_m = -1
	end

	local offset_x = math.abs(to_x - from_x)
	local offset_y = math.abs(to_y - from_y)
	if offset_x == 0 and offset_y == 0 then
		return to_x + distance, to_y
	end

	if offset_x == 0 then
		return to_x, to_y + distance * side_y_m
	end

	if offset_y == 0 then
		return to_x + distance * side_x_m, to_y
	end

	local m = offset_y / offset_x

	local from_to_distance = math.pow(offset_x ^ 2 + offset_y ^ 2, 0.5)
	local from_target_distance = from_to_distance + distance

	local from_target_x = math.pow(from_target_distance ^ 2 / (1 + m ^ 2), 0.5)
	local from_target_y = from_target_x * m

	return from_x + from_target_x * side_x_m, from_y + from_target_y * side_y_m
end

function get_two_point_distance(from_x, from_y, to_x, to_y)
	local offset_x = math.abs(to_x - from_x)
	local offset_y = math.abs(to_y - from_y)
	if offset_x == 0 and offset_y == 0 then
		return 0
	end

	if offset_x == 0 then
		return offset_y
	end

	if offset_y == 0 then
		return offset_x
	end

	return math.pow(offset_x ^ 2 + offset_y ^ 2, 0.5)
end

function test34()

	math.randomseed(os.time())
	for i=1, 10 do
		local from_x, from_y = math.random(0, 50) , math.random(0, 50)
		local to_x, to_y = math.random(0, 50) , math.random(0, 50)
		local distance = math.random(0, 50)
		local target_x, target_y = get_extension_point(from_x, from_y, to_x, to_y, distance)
		-- print("target_x=", target_x, " target_y=", target_y)
		local from_to_distance = get_two_point_distance(from_x, from_y, to_x, to_y)
		local from_target_distance = get_two_point_distance(from_x, from_y, target_x, target_y)
		-- print("from_to_distance=", from_to_distance, " from_target_distance=", from_target_distance)
		local real_distance = from_target_distance - from_to_distance
		if math.abs(real_distance - distance) > 0.1 then
			print("from_x=", from_x, " from_y=", from_y)
			print("to_x=", to_x, " to_y=", to_y)
			print("distance=", distance)
			print("real_distance=", real_distance, " distance=", distance)
			break
		end

		print()
		local step_size = 10
		local step_x = (target_x - to_x) / step_size
		local step_y = (target_y - to_y) / step_size
		for k=1, step_size do
			local new_x = to_x + step_x * k
			local new_y = to_y + step_y * k
			print("new_x=", new_x, " new_y=", new_y)
		end


	end
	
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
,	test8
,	test9
,	test10
,	test11
,	test12
,	test13
,	test14
,	test15
,	test16
,	test17
,	test18
,	test19
,	test20
,	test21
,	test22
,	test23
,	test24
,	test25
,	test26
,	test27
,	test28
,	test29
,	test30
,	test31
,	test32
,	test33
,	test34
,	test35
}

function do_main()
	print("_VERSION=", _VERSION)
	print()

	local maxcase = #test_list
	local testcase = maxcase
	if #arg > 0 then -- arg is the params from lua execute program
		if arg[1] == "all" then
			print "test:run all case"
			for i, v in ipairs(test_list) do
				print(string.format("test:RUN case[%d]", i))
				local ret = v() or -1
				if ret ~= 0 then
					print(string.format("test:case[%d] %d", i, ret))
					return
				end
				print ""
			end
			print "all case done"
			return
		end

		testcase = tonumber(arg[1]) or maxcase
		if testcase <= 0 or testcase > maxcase then
			testcase = maxcase
		end
	end

	print(string.format("test:RUN case[%d]", testcase))
	local ret = test_list[testcase]() or -1
	if ret ~= 0 then
		print(string.format("test:case[%d] %d", testcase, ret))
	end

end

do_main()

