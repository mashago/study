#!/usr/bin/lua

function test1()
	print(type(true))

	local x = true
	local y = false
	print(type(x))
	print(type(y))
	-- print("x=" .. x) -- error, boolean cannot append into string
	print("x=", x)
	print("y=", y)

	return 0
end

function test2()

	-- false, nil is false, other are true
	-- 0 is true

	local x = true
	local y = false

	if x then
		print("x true")
	end

	if not y then
		print("y not true")
	end

	local z = nil
	if not z then
		print("nil false")
	end

	if not nil then
		print("nil false")
	end

	z = 0
	if z then
		print(z .. " true")
	end

	z = 1
	if z then
		print(z .. " true")
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

