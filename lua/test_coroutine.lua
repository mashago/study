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

local CoroutineMgr
CoroutineMgr = 
{
	_cor_list = {},
	create = function(func)
		-- 1. first resume in create, pass the call func, and yield self coroutine
		-- 2. second resume in logic code, pass params for func, and call func
		-- 3. if no yield in func, just yield func ret

		local self = CoroutineMgr
		local cor = table.remove(self._cor_list)
		if cor then
			print("reuse cor 2")
			coroutine.resume(cor, func)
			return cor	
		end
		cor = coroutine.create(function(f)
			local params = table.pack(coroutine.yield(coroutine.running()))
			while true do
				local ret = table.pack(f(table.unpack(params)))
				print("reuse cor 1")
				table.insert(self._cor_list, cor)
				f = coroutine.yield(table.unpack(ret))
				params = table.pack(coroutine.yield(coroutine.running()))
			end
		end)
		return select(2, coroutine.resume(cor, func))
	end,

	resume = function(cor, ...)
		return coroutine.resume(cor, ...)
	end,

	yield = function(...)
		return coroutine.yield(...)
	end
}

function test3()
	local cor_func1 = function(n)
		n = CoroutineMgr.yield(n + 1)
		print("cor_func1 n1=", n)
		n = CoroutineMgr.yield(n + 1)
		print("cor_func1 n2=", n)
		return n + 1
	end

	local cor1 = CoroutineMgr.create(cor_func1)
	print("a1 ", CoroutineMgr.resume(cor1, 1))
	print("a2 ", CoroutineMgr.resume(cor1, 3))

	local cor2 = CoroutineMgr.create(cor_func1)
	print("b1 ", CoroutineMgr.resume(cor2, 2))
	print("b2 ", CoroutineMgr.resume(cor2, 4))

	print("a3 ", CoroutineMgr.resume(cor1, 5))
	print("b3 ", CoroutineMgr.resume(cor2, 6))

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

