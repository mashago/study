#!/usr/local/bin/lua

require "util"

function test1()

	local a = 2
	local b = a ^ 4
	log("a=%d", a)
	log("b=%d", b)
	log()

	a = 10
	b = a % 3
	log("a=%d", a)
	log("b=%d", b)
	log()

	a = math.pi
	b = a % 1
	c = a % 0.01
	log("a=%f", a)
	log("b=%f", b)
	log("c=%f", c)
	log("a-b=%f", a-b)
	log("a-c=%f", a-c)

	return 0
end

function test2()

	local a = 10
	local b = 10
	local c = "10"
	local d = nil
	local e = false

	if a == b then
		log("a==b")
	end

	if a ~= c then
		log("a~=c")
	end

	if d ~= e then
		log("d~=e")
	end

	return 0
end

function logicTrue()
	log("logicTrue()")
	return true
end

function logicFalse()
	log("logicFalse()")
	return false
end

function test3()
	log("1")
	if logicTrue() and logicFalse() then
	end
	log()

	log("2")
	if logicFalse() and logicTrue() then
	end
	log()

	log("3")
	if logicTrue() or logicFalse() then
	end
	log()

	log("4")
	if logicFalse() or logicTrue() then
	end
	log()

	log("5")
	if not logicFalse() then
	end
	log()

	local y = x or 10
	log("y=%d", y)
	log()

	local a, b = 10, 15
	local max = (a > b) and a or b
	log("max=%d", max)
	a, b = 15, 15
	max = (a > b) and a or b
	log("max=%d", max)
	a, b = 15, 20
	max = (a > b) and a or b
	log("max=%d", max)

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

