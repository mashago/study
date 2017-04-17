#!/usr/local/bin/lua

require "util"

function returnNull()
end

function returnOne()
	return 1
end

function returnTwo()
	return 21, 22
end

function returnThree()
	return 31, 32, 33
end


function test1()

	do
		local a, b, c = returnTwo()
		print(a)
		print(b)
		print(c)
		log()
	end

	do
		-- NOTE: only last function can return more then 1 ret
		local a, b, c = returnOne(), returnTwo()
		print(a)
		print(b)
		print(c)
		log()
	end

	do
		-- NOTE: only last function can return more than 1 value
		local a, b, c = returnTwo(), returnOne()
		print(a)
		print(b)
		print(c)
		log()
	end

	do
		local a, b, c = returnTwo(), returnTwo()
		print(a)
		print(b)
		print(c)
		log()
	end

	do
		-- force to return 1 value
		local a, b = (returnTwo())
		print(a)
		print(b)
		log()
	end

	do
		-- unpack array
		-- return from t[1] to t[#t]
		-- from lua5.2, use table.unpack(t)
		local t = {1, 2, 3}
		local a, b = table.unpack(t)
		print(a)
		print(b)
		log()
	end

	return 0
	
end

function vaFunc(...)
	do
		local a, b, c = ...
		print(a)
		print(b)
		print(c)

		local len = select('#', ...) -- get va arg len
		log("len=%d", len)
		for i=1, select('#', ...) do
			-- print(select(i, ...)) -- return i to len value
			local x = select(i, ...)
			print("x=" .. x)
		end

		-- for lua5.1, will create a arg table, which store va
		-- local n = arg[1]
		-- from lua5.2, arg table is delete, use { ... } to make a va table
		local t = { ... }
		print("#t=", #t)
		for i=1, #t do
			print("t[" .. i .. "]=", t[i])
		end

		log()
	end

	return ...
end

function test2()
	vaFunc()
	vaFunc(1, 2)
	vaFunc('a', 'b', 'c')

	return 0
end

function test3()
	print("type(print)", type(print))

	do
		local t = { p = print }
		t.p("hello world")
		print(t.p)
		log()

		print = math.pi
		t.p(print)
		print = t.p
		log()

		t.x = function (x) print(x) end
		t.x(10)
		log()
	end

	do
		local funcA = function (x, func) return func(x) end
		local a = funcA(10, function (x) return x * 10 end)
		log("a=%d", a)
		log()
	end

	do
		local a = 10
		local funcA = function () return a * 5 end
		local x = funcA()
		log("x=%d", x)
		log()
	end

	do
		local a = 10
		local funcA = function ()
			local n = 0 -- upvalue for return function
			return function ()
				-- closure, is a function and its upvalue
				n = n + 1
				return n
			end
		end

		local myFunc1 = funcA()
		log("myFunc1()=%d", myFunc1())
		log("myFunc1()=%d", myFunc1())
		log("myFunc1()=%d", myFunc1())

		local myFunc2 = funcA()
		log("myFunc2()=%d", myFunc2())
		log("myFunc1()=%d", myFunc1())

		log()
	end

	return 0
end

function test4()
	do
		local t = {}
		t.p = function (...) return print(...) end
		t.p("111")
	end

	do
		local t = {
			p = function (...) return print(...) end,
		}
		t.p("222")
	end
	do
		local t = {}
		function t.p(...) return print(...) end
		t.p("333")
	end

	do
		local function p(...)
			return print(...) 
		end
		p("hello p")
		log()
	end

	do
		local plusA
		plusA = function (n)
			n = n or 0
			if n <= 0 then return n end
			return n + plusA(n-1)
		end

		log("plusA(3)=%d", plusA(3))
		log("plusA(10)=%d", plusA(10))

		-- NOTE:
		-- will expend to:
		-- local plusB
		-- plusB = function (n) ...
		local function plusB(n)
			n = n or 0
			if n <= 0 then return n end
			return n + plusB(n-1)
		end

		log("plusB(3)=%d", plusB(3))
		log("plusB(10)=%d", plusB(10))

		--[[
		-- error case
		local plusC = function (n)
			n = n or 0
			if n <= 0 then return n end
			return n + plusC(n-1) -- plusC not define here
		end

		log("plusC(3)=%d", plusC(3))
		log("plusC(10)=%d", plusC(10))
		--]]

	end
		

	return 0
end

function run_func1()
	if func1 then
		func1()
	end
end

function run_func2()
	if func2 == nil then
		print "func2 == nil"
	end
end

function test5()

	-- func1 is global function
	function func1()
		print "hello func1"
	end
	func1()

	function func1()
		print "hello func1 overwrite"
	end
	func1()
	run_func1()

	print "------------------------------------"
	print()

	local func2 = function()
		print "hello func2"
	end
	func2()
	run_func2()

	print "------------------------------------"
	print()

	local func3 = function(x)
		if type(x) == "string" then
			-- value pass
			print("x1:", x)
			x = "change"
			print("x2:", x)
		elseif type(x) == "table" then
			-- reference pass
			print("x:", x)
			x.k1 = "v1"
		end
	end

	local t1 = {}
	print("t1:", t1)
	print("t1.k1=", t1.k1)
	func3(t1)
	print("t1.k1=", t1.k1)
	print()

	local s1 = "hello"
	print("s1:", s1)
	func3(s1)
	print("s1:", s1)

	print "------------------------------------\n"

	local func4 = function()
		-- multi-retrun
		return 1, 2
	end

	-- multi-assignment
	local x, y, z = func4()
	print(x, y, z)

	return 0
end

function test6()

	local func1 = function()
	end

	return 0
end

function test_notyet()
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

