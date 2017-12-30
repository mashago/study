-- @see https://github.com/jinq0123/hotfix

package.path = "helper/?.lua;" .. package.path
local hotfix = require("hotfix")

function write_file(file_name, buffer)
    local f = assert(io.open(file_name, "w"))
    f:write(buffer)
    assert(f:close())
end  -- write_file()

function test1()

	local module_name = "my_test"
	local file_name = module_name .. ".lua"

	local buffer = [[
local M = {}

g_n = 0
g_func = function()
	print("old g_func g_n=", g_n)
end
local l_n = 0
M.m_n = 0
M.func = function()
	print("old g_n=", g_n, " l_n=", l_n, " M.m_n=", M.m_n)
	return 0
end

return M
]]

	write_file(file_name, buffer)

	print("old module")
	local my_test = require(module_name)
	g_func()
	local ret = my_test.func()
	print("ret=", ret)
	print()

	local buffer = [[
local M = {}

g_n = 1
g_func = function()
	print("new g_func g_n=", g_n)
end
local l_n = 1
M.m_n = 1 -- table value will not change
M.func = function()
	-- upvalue will replace by old value: l_n
	print("new g_n=", g_n, " l_n=", l_n, " M.m_n=", M.m_n)
	return 1
end
M.func2 = function()
	print("new function func2")
end

return M
]]

	write_file(file_name, buffer)

	-- will overwrite global value, global function and local function
	print("new module")
	hotfix.hotfix_module(module_name)
	g_func()
	local ret = my_test.func()
	print("ret=", ret)
	my_test.func2()

	os.remove(file_name)

	return 0
end

function test2()

	local module_name = "my_test"
	local file_name = module_name .. ".lua"

	local buffer = [[
local M = {}

local l_n = 0
M.m_n = 0

function M:new()
	local obj = {}
	M.__index = self
	setmetatable(obj, M)

	obj.o_n = 0

	return obj
end

function M:func()
	print("old l_n=", l_n," M.m_n=", M.m_n, " self.o_n=", self.o_n)
end

return M
]]

	write_file(file_name, buffer)

	print("old module")
	local my_test = require(module_name)
	local obj1 = my_test:new()
	obj1:func()
	print()

	local buffer = [[
local M = {}

local l_n = 1
M.m_n = 1

function M:new()
	local obj = {}
	M.__index = self
	setmetatable(obj, M)

	obj.o_n = 1
	obj.o_n2 = 2 -- new value, new obj is ok, but old obj will nil

	return obj
end

function M:func()
	print("new l_n=", l_n," M.m_n=", M.m_n, " self.o_n=", self.o_n, " self.o_n2=", self.o_n2)
end

function M:func2()
	print("new function func2")
end

return M
]]

	write_file(file_name, buffer)

	print("new module")
	hotfix.hotfix_module(module_name)
	obj1:func()
	obj1:func2()
	local obj2 = my_test:new()
	print()
	obj2:func()
	obj2:func2()

	os.remove(file_name)

	return 0
end

function test3()

	local module_name = "my_test"
	local file_name = module_name .. ".lua"

	local buffer = [[
local M = {}

function M.func()
	local n = 1
	local f = function()
		print("f n=", n)
	end
	f()
end

return M
]]

	write_file(file_name, buffer)

	print("old module")
	local my_test = require(module_name)
	my_test.func()
	print()

	local buffer = [[
local M = {}

function M.func()
	local n = 2
	local f = function()
		print("f n=", n)
	end
	f()
end

return M
]]

	write_file(file_name, buffer)

	print("new module")
	hotfix.hotfix_module(module_name)
	my_test.func()
	print()

	os.remove(file_name)

	return 0
end

function test4()

	local module_name = "my_test"
	local file_name = module_name .. ".lua"

	local buffer = [[
local M = {}

local function l_func()
	print("l_func old")
end

function g_func()
	print("g_func old")
end

function M.func()
	local n = 1
	local f = function()
		print("f n=", n)
	end
	f()
	l_func()
	g_func()
end

return M
]]

	write_file(file_name, buffer)

	print("old module")
	local my_test = require(module_name)
	my_test.func()
	print()

	local buffer = [[
local M = {}

local function l_func()
	print("l_func new")
end

function g_func()
	print("g_func new")
end

function M.func()
	local n = 2
	local f = function()
		print("f n=", n)
	end
	f()
	l_func()
	g_func()
end

return M
]]

	write_file(file_name, buffer)

	print("new module")
	local hotfix_helper = require("helper.hotfix_helper")
	hotfix_helper.init()
	hotfix_helper.check()
	my_test.func()
	print()

	os.remove(file_name)

	return 0
end

function test5()

	local module_name = "my_test"
	local file_name = module_name .. ".lua"

	local buffer = [[
local function l_func()
	print("l_func old")
end

function g_func()
	print("g_func old")
	l_func()

	local func = func_map[1]
	func()
end

function g_init()
	func_map = {}
	local func_node = function()
		print("old closure call l_func")
		l_func()
	end
	func_map[1] = func_node
end

]]

	write_file(file_name, buffer)

	print("old module")
	require(module_name)
	g_init()
	g_func()
	print()

	local buffer = [[
local function l_func()
	print("l_func new")
end

function g_func()
	print("g_func new")
	l_func()

	local func = func_map[1]
	func()
end

function g_init()
	func_map = {}
	local func_node = function()
		print("old closure call l_func")
		l_func()
	end
	func_map[1] = func_node
end

]]

	write_file(file_name, buffer)

	print("new module")
	local hotfix_helper = require("helper.hotfix_helper")
	hotfix_helper.init()
	hotfix_helper.check()
	g_func()
	print()

	os.remove(file_name)

	return 0
end

function test6()
	-- must set in hotfix_module_names

	-- write class
	do
	local module_name = "class"
	local file_name = module_name .. ".lua"

	local buffer = [[
local _class={} 
function class(super)
	local class_type={}
	class_type.ctor=false
	class_type.super=super

	class_type.new=function(...) 
		-- function to new a obj
		local obj={}
		do
			local create
			create = function(c,...)
				-- recursion do create super
				if c.super then
					create(c.super,...)
				end
				-- call constructor
				if c.ctor then
					c.ctor(obj,...)
				end
			end

			create(class_type,...)
		end

		-- set obj get metatable(get from base class)
		setmetatable(obj,{ __index=_class[class_type] })
		return obj
	end

 	-- set base class set metatable
	-- therefore base class set data or function, obj can get it
	local vtbl={}
	_class[class_type]=vtbl
	setmetatable(class_type,{__newindex=
		function(t,k,v)
			print("__newindex k=", k)
			vtbl[k]=v
		end
	})
 
 	-- set base class get metatable(get from super)
	-- if base class has super, base class can get data from super which not found in base class
	if super then
		setmetatable(vtbl,{__index=
			function(t,k)
				print("__index k=", k)
				local ret=_class[super][k]
				vtbl[k]=ret
				return ret
			end
		})
	end
 
	return class_type
end
]]
	write_file(file_name, buffer)

	print("require ", module_name)
	require(module_name)
	print()
	end

	-- write base class
	do
	local module_name = "baseclass"
	local file_name = module_name .. ".lua"

	local buffer = [[
BaseClass = class()
function BaseClass:ctor(x)
	print("BaseClass ctor")
	self.x = x
end

function BaseClass:func1()
	print("BaseClass func1", self.x)
end

function BaseClass:func2()
	print("BaseClass func2")
end

function BaseClass:func3()
	print("BaseClass func3")
end
return BaseClass
]]
	write_file(file_name, buffer)

	local func = assert(load(buffer))
	local ok, obj = assert(pcall(func))
	print("ok=", ok)
	print("type(obj)=", type(obj))

	print("require ", module_name)
	require(module_name)
	print()
	end

	-- write extend class
	do
	local module_name = "extendclass"
	local file_name = module_name .. ".lua"

	local buffer = [[
ExtendClass = class(BaseClass)
function ExtendClass:ctor(a, b)
	print("ExtendClass ctor")
	self.a = a
	self.b = b
end

function ExtendClass:print_ab()
	print("ExtendClass print_ab", self.a, self.b)
end

function ExtendClass:func2()
	print("ExtendClass func2")
end
return ExtendClass
]]
	write_file(file_name, buffer)

	print("require ", module_name)
	require(module_name)
	print()
	end

	local b1 = BaseClass.new(10)
	b1:func1()
	b1:func2()


	local e1 = ExtendClass.new(20, 30)
	e1:func1()
	e1:print_ab()
	e1:func2()


	-----------------------------------------

	-- write base class new
	do
	local module_name = "baseclass"
	local file_name = module_name .. ".lua"

	local buffer = [[
BaseClass = class()
function BaseClass:ctor(x)
	print("BaseClass ctor new")
	self.x = x
end

function BaseClass:func1()
	print("BaseClass func1 new", self.x)
end

function BaseClass:func2()
	print("BaseClass func2 new")
end

function BaseClass:func3()
	print("BaseClass func3 new")
end
return BaseClass
]]
	write_file(file_name, buffer)

	print("write ", module_name)
	print()
	end

	-- write extend class new
	do
	local module_name = "extendclass"
	local file_name = module_name .. ".lua"

	local buffer = [[
ExtendClass = class(BaseClass)
function ExtendClass:ctor(a, b)
	print("ExtendClass ctor new")
	self.a = a
	self.b = b
end

function ExtendClass:print_ab()
	print("ExtendClass print_ab new", self.a, self.b)
end

function ExtendClass:func2()
	print("ExtendClass func2 new")
end
return ExtendClass
]]
	write_file(file_name, buffer)
	print("write ", module_name)
	print()
	end
	

	print("new module")
	local hotfix_helper = require("helper.hotfix_helper")
	hotfix_helper.init()
	hotfix_helper.check()


	b1:func1()
	b1:func2()

	e1:func1()
	e1:print_ab()
	e1:func2()


	os.remove("class.lua")
	os.remove("baseclass.lua")
	os.remove("extendclass.lua")

	return 0
end

function test7()

	local module_name = "my_test"
	local file_name = module_name .. ".lua"

	local buffer = [[
g_func_list = g_func_list or {} 
g_func_list = {} 
local function l_func()
	print("old l_func")
end

local class = 
{
	tmp_list = {}
}

function class.func_a()
	print("func_a old")
end

function class.func_b()
	print("func_b old")
	class.tmp_list[1]()
	class.tmp_list[2]()
	class.tmp_list[3]()
	print("------")
	g_func_list[1]()
	g_func_list[2]()
	g_func_list[3]()
end

function class.func_c()
	print("func_c old")
	-- l_func()
end

function class.init()
	local func_node = function()
		print("old closure call class.func_a %p", class.func_a)
		class.func_a()
	end
	local func_node2 = function()
		print("old closure call l_func %p", l_func)
		l_func()
	end
	local cfa = class.func_a
	local func_node3 = function()
		print("old closure call cfa %p", cfa)
		cfa()
	end
	class.tmp_list[1] = func_node
	class.tmp_list[2] = func_node2
	class.tmp_list[3] = func_node3
	g_func_list[1] = func_node
	g_func_list[2] = func_node2
	g_func_list[3] = func_node3
end

--[==[
function init()
	local func_node = function()
		print("old closure call class.func_a %p", class.func_a)
		class.func_a()
	end
	local func_node2 = function()
		print("old closure call l_func %p", l_func)
		l_func()
	end
	local cfa = class.func_a
	local func_node3 = function()
		print("old closure call cfa %p", cfa)
		cfa()
	end
	class.tmp_list[1] = func_node
	class.tmp_list[2] = func_node2
	class.tmp_list[3] = func_node3
	g_func_list[1] = func_node
	g_func_list[2] = func_node2
	g_func_list[3] = func_node3
end
--]==]

return class
]]

	write_file(file_name, buffer)

	print("old module")
	c = require(module_name)
	c.init()
	print()
	c.func_b()
	print()
	c.func_c()
	print()

	local buffer = [[
g_func_list = g_func_list or {}
-- g_func_list = {} -- g_func_list will be clear
local function l_func()
	print("new l_func")
end

local class = 
{
	tmp_list = {}
}
function class.func_a()
	print("func_a new")
end

function class.func_b()
	print("func_b new")
	class.tmp_list[1]()
	class.tmp_list[2]()
	class.tmp_list[3]()
	print("------")
	g_func_list[1]()
	g_func_list[2]()
	g_func_list[3]()
end

function class.func_c()
	print("func_c new")
	-- l_func()
end

return class
]]

	write_file(file_name, buffer)

	print("new module")
	local hotfix_helper = require("helper.hotfix_helper")
	hotfix_helper.init()
	hotfix_helper.check()
	c.func_b()
	print()
	c.func_c()
	print()

	os.remove(file_name)

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
