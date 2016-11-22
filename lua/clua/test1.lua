
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
	return x + y
end

-- for test3
function func_t3(x)
	-- will call c function
	if mysin == nil then
		return -1
	end
	return mysin(x)
end

-- for test4
function func_t4(x, y, op)

	if cfunc4 == nil then
		return -1
	end

	local result = 0
	local func
	if op == "add" then
		func = cfunc4.add
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
	return c_str_upper(x); -- call c fucntion
end
