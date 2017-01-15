#!/usr/local/bin/lua

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

function test1()

	print("type(test1)", type(test1))

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

function test2()

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

