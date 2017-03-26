#!/usr/local/bin/lua

require "util"

function printf(format, ...)
	return io.stdout:write(string.format(format, ...))
end

function test1()
	do
		local t = {}
		print("getmetatable(t)=", getmetatable(t))

		local s = "hi"
		print("getmetatable(s)=", getmetatable(s))
	end

	return 0
end

function test2()
	do
		local t1 = {11, 12}
		local t2 = {21, 22}

		-- local t3 = t1 + t2 -- error
	end

	do
		local mt = {}
		local t1 = {11, 12}
		local t2 = {21, 22}
		setmetatable(t1, mt)
		setmetatable(t2, mt)
		print("getmetatable(t1)=", getmetatable(t1))
		print("getmetatable(t2)=", getmetatable(t2))

		mt.__add = function(t1, t2)
			local ret = {}
			for _, v in pairs(t1) do
				ret[#ret+1] = v
			end
			for _, v in pairs(t2) do
				ret[#ret+1] = v
			end
			return ret
		end

		local t3 = t1 + t2
		log(table.concat(t3, " "))
		log()
	end

	do
		local mt = {}
		local t1 = {11, 12}
		local t2 = {21, 22}
		-- setmetatable(t1, mt)
		setmetatable(t2, mt)
		print("getmetatable(t1)=", getmetatable(t1))
		print("getmetatable(t2)=", getmetatable(t2))

		mt.__add = function(t1, t2)
			local ret = {}
			for _, v in pairs(t1) do
				ret[#ret+1] = v
			end
			for _, v in pairs(t2) do
				ret[#ret+1] = v
			end
			return ret
		end

		local t3 = t1 + t2 -- will call use t2 mt.__add
		log(table.concat(t3, " "))
		log()
	end

	return 0
end

function test3()
	do
		local mt = {}
		local t1 = {11, 12}
		setmetatable(t1, mt)
		print("getmetatable(t1)=", getmetatable(t1))

		mt.__metatable = "not your business" -- protect metatable access
		print("getmetatable(t1)=", getmetatable(t1))

		local func = function(t)
			setmetatable(t, {})
		end
		print(pcall(func, t1))

		log()
	end

	return 0
end

function test4()

	-- __index only for visit
	-- find key from table
	-- 1.find from table, if find, return
	-- 2.if has no metatable, return nil
	-- 3.if metatable.__index is nil, return nil.
	-- 4.if metatable.__index is a table, do 1~3 again.
	-- 5.if metatable.__index is a function, call the function, pass table and key

	local content = {a=10, b=20}
	local mt = {__index=content}

	do
		local t = {a=1}
		if t.a then
			printf("t.a=%d\n", t.a)
		end
		if t.b == nil then
			printf("t.b == nil\n")
		end

		-- return setted table
		local ret = setmetatable(t, mt)
		if t.b then
			printf("t.b=%d\n", t.b)
		end
		print("t:", t)
		print("ret:", ret)

		ret = getmetatable(t)
		print("mt:", mt)
		print("ret:", ret)

		content.c = 3
		if t.c then
			printf("t.c=%d\n", t.c)
		end
	end

	return 0
end

function test5()

	do
		local mt = 
		{
			__index = function(t, key)
				print("__index: t=", t)
				print("__index: key=", key, " type(key)=", type(key))
				return "get from __index"
			end
		}
		local t = {a=1}
		if t.a then
			printf("t.a=%d\n", t.a)
		end
		if t.b == nil then
			printf("t.b == nil\n")
		end

		setmetatable(t, mt)
		print("t=", t)

		local x = t.b
		if x then
			print("t.b=", x)
		end
		print()

		x = t[0]
		if x then
			print("t[0]=", x)
		end
		print()

		x = t["0"]
		if x then
			print("t[\"0\"]=", x)
		end
		print()

	end

	return 0
end

function test6()

	do
		local mt = {}
		local t1 = {}
		setmetatable(t1, mt)
		mt.__newindex = mt

		t1[1] = 1
		t1[2] = 2
		log("t1:" .. table.concat(t1, " "))
		log("mt:" .. table.concat(mt, " "))

		log()
	end

	do

		local mt = 
		{
			__newindex = function(t, key, v)
				if k == "a" then
					print("avoid set a")
				else
					-- t.key = v -- will dead loop
					rawset(t, key, v)
				end
			end
		}

		local t = {a=1}
		if t.a then
			printf("t.a=%d\n", t.a)
		end
		if t.b == nil then
			printf("t.b == nil\n")
		end

		setmetatable(t, mt)

		t.b = 2
		print("t.b=", t.b)
		print()
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

