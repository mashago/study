
-- 1. get module name, from require("modname") input
local modname = ...

-- 2. module table
local M = {}

-- 3. add table into requirer _G
_G[modname] = M

-- 4. set into requirer package.loaded, therefore no need to return module table in the end
package.loaded[modname] = M

-- 5. include part, define some global function into local
local print = print
-- or just use metatable to access _G
setmetatable(M, {__index = _G})

-- 6. set module environment is M
-- after this call, cannot access other module anymore
-- lua5.2 not support this function
setfenv(1, M)

-- print_hello() is set into a new environment M, therrfore will become M.print_hello()
function print_hello(n)
	n = n or 0
	print("my_module2 print_hello() " .. "n=" .. n)
end

