-- @see https://github.com/jinq0123/hotfix

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
	-- hotfix.hotfix_module(module_name)
	local hotfix_helper = require("helper.hotfix_helper")
	my_test.func()
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
