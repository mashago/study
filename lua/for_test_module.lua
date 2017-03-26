
-- 1.
local M = {}
mymodule = M

M.printHello = function()
	print("hello world")
end

print "for_test_module finish"

return mymodule

--[[
-- 2.
local modname ...  -- require module name
local M = {}
_G[modname] = M
-- if module has no return, will return package.loaded[modname]
package.loaded[modname] = M 
--]]

--[[
-- 3. set environment
local modname ...  -- require module name
local M = {}
_G[modname] = M
-- if module has no return, will return package.loaded[modname]
package.loaded[modname] = M 
setmetatable(M, {__index = _G}) -- for see global package
setfenv(1, M)

function printHello() ... end -- now printHello is in M
--]]

--[[
-- 4. set environment, include
local modname ...  -- require module name
local M = {}
_G[modname] = M
-- if module has no return, will return package.loaded[modname]
package.loaded[modname] = M 
local io = io -- include package
local math = math
setfenv(1, M)
--]]

--[[
-- 5. use module()
module(..., package.seeall)
--]]
