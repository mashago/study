
-- 1.define a global table, therefore can call "my_module.xxx" directly
my_module = {}

local x = 1

-- 2.add function into table
function my_module.init()
	print("my_module init()")
end

function my_module.print_hello(n)
	n = n or 0
	print("my_module print_hello() " .. "n=" .. n .. " x=" .. x)
	x = x + 10 -- x will become a upvalue?
end

my_module.init()

-- 3. return table
return my_module
