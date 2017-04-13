#!/usr/local/bin/lua

require "util"

function test1()

	local co
	local coFunc = function(n)
		log("inside status=%s n=%d", coroutine.status(co), n)
		log()

		local r = coroutine.yield(n + 1)
		print("r=" .. r)
		log()
		return n * r
	end

	co = coroutine.create(coFunc)
	log(co)
	log("type(co)=%s", type(co))
	log("before resume status=%s", coroutine.status(co))
	log()

	local flag, ret = coroutine.resume(co, 10)
	log("after resume status=%s", coroutine.status(co))
	print("flag=", (flag and "true") or "false", "ret=", ret)
	log()

	flag, ret = coroutine.resume(co, 20)
	print("flag=", (flag and "true") or "false", "ret=", ret)
	log("final status=%s", coroutine.status(co))
	log()

	flag, ret = coroutine.resume(co, 30)
	print("flag=", (flag and "true") or "false", "ret=", ret)

	return 0
end

function test2()

	local send = function(n)
		coroutine.yield(n)
	end

	local product = function(n)
		while true do
			n = n * 2
			send(n)
		end
	end

	local productor = coroutine.create(product)

	function recv(n)
		local status, item = coroutine.resume(productor, n)
		return item
	end

	function consume()
		for i = 1, 5 do
			local item = recv(i)
			print("item=" .. item)
		end
	end

	consume()

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
