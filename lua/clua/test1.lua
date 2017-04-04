
-- for test1
width = 200
height = 300

WRITE = {r = 255, g = 255, b = 255}
BLACK = {r = 0, g = 0, b = 0}
RED = {r = 255, g = 0, b = 0}

background = {r = 1, g = 2, b = 3}
background2 = WRITE

-- for test2
function func_t2(x, y)
	-- return (x^2 * math.sin(y)) / (1 - x)
	return x / y
end

-- for test3
function func_t3(x)
	-- mysin is a c function
	-- get c function, which already push into global
	if mysin == nil then
		return -1
	end
	return mysin(x)
end

-- for test4
function func_t4(x, y, op)

	-- cfunc4 is c function list register name
	if cfunc4 == nil then
		return -1
	end

	local result = 0
	local func
	if op == "add" then
		func = cfunc4.add -- get c function
	elseif op == "sub" then
		func = cfunc4.sub
	elseif op == "times" then
		func = cfunc4.times
	elseif op == "div" then
		func = cfunc4.div
	end
	if func ~= nil then
		result = func(x, y)
	end
	return result
end

-- for test5
test5array = {"masha", "peter", 12}

-- for test7
function func_t7(x)
	return c_str_upper(x) -- call c fucntion
end

-- for test9
function func_t9()
	cfunc9.set_ga()
end

-- for test10
function func_t10()
	local counter = c_new_counter() -- call closure factory, get a c closure
	local t = 0
	if counter == nil then
		return 0
	end

	return counter -- return a closure
end

function func_t11()
	local t1 = tuple.new(10, "masha", {}, 3) -- c lib tuple, call new function, create a c closure
	print(t1(1)) -- call c closure
	print(t1(2))
	print(t1())
	-- print(t1(-1))
	print(t1(5))
end

function func_t12(n)
	n = n or 10
	local t = myst.new()
	myst.set(t, n)
	local a = myst.get(t)
	print(a)

	-- will error
	-- myst.set(io.stdin, n)
end

