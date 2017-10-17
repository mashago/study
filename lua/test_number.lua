#!/usr/local/bin/lua

function test1()

	local x = 10
	print(x)
	print()

	local int_max = 2147483647
	local int_min = -2147483648
	x = int_max
	print(x)
	print()

	x = x + 1
	print(x)
	print()

	x = int_max * 2
	print(x)
	print()

	x = int_max * 4
	print(x)
	print()

	x = int_min
	print(x)
	print()

	x = x + int_min
	print(x)
	print()

	-- x = 1125899906842624
	x = 2^50
	print(x)
	print()

	x = 3.1415926
	print(x)
	print()

	return 0
end

function test2()

	local n = 10
	local x = tostring(n)
	print(type(x))

	return 0
end

function test3()
	local a = 123
	local b = 456
	local c = (a << 16) | b

	local x = (c >> 16)
	local y = (x << 16) ~ c

	print("a=", a, " b=", b, " c=", c)
	print("x=", x, " y=", y)

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

