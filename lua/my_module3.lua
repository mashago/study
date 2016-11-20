
-- old logic
--[[
local modname = ...
local M = {}
_G[modname] = M
package.loaded[modname] = M
setfenv(1, M)
--]]

-- only one call, replace all before
-- package.seeall same as setmetatable(M, {__index=_G})
module(..., package.seeall)

function print_hello(n)
	print("my_module3 print_hello() " .. "n=" .. n)
end

