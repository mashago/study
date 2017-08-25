-- @see https://github.com/jinq0123/hotfix

local hotfix = require("hotfix")

function sleep(sec)
  local end_time = os.time() + sec
  while os.time() < end_time do end
end  -- sleep()

function write_file(file_name, buffer)
    local f = assert(io.open(file_name, "w"))
    f:write(buffer)
    assert(f:close())
end  -- write_file()

local buffer = [[
local M = {}

g_n = 0
local l_n = 0
M.m_n = 0
M.func = function()
	print("old", g_n, l_n, M.m_n)
	return 0
end

return M
]]

local test_file_name = "my_test"
local test_file_full_name = test_file_name .. ".lua"
write_file(test_file_full_name, buffer)

local my_test = require(test_file_name)
my_test.func()

local buffer = [[
local M = {}

g_n = 0
local l_n = 0
M.m_n = 0
M.func = function()
	print("new", g_n, l_n, M.m_n)
	return 0
end

return M
]]

-- local test_file_name = "my_test"
-- local test_file_full_name = test_file_name .. ".lua"
write_file(test_file_full_name, buffer)

--local my_test = require(test_file_name)
hotfix.hotfix_module(test_file_name)
my_test.func()
