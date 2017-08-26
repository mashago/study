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
	print("old g_func")
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
	print("new g_func")
end
local l_n = 1
M.m_n = 1
M.func = function()
	print("new g_n=", g_n, " l_n=", l_n, " M.m_n=", M.m_n)
	return 1
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

	return obj
end

function M:func()
	print("new l_n=", l_n," M.m_n=", M.m_n, " self.o_n=", self.o_n)
end

return M
]]

	write_file(file_name, buffer)

	print("new module")
	hotfix.hotfix_module(module_name)
	obj1:func()
	local obj2 = my_test:new()
	print()
	obj2:func()

	os.remove(file_name)

	return 0
end

test_list =
{
	test1
,	test2
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
