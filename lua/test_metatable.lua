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
	local mt = {c=5, __index=content}
	local mt2 = {a=100, b=200}
    mt2.__index = mt2

	do
		local t = {a=1}
		if t.a then
			log("t.a=%d", t.a)
		end
		if t.b == nil then
			log("t.b == nil")
		end
		log()

		-- return setted table
		local ret = setmetatable(t, mt)
        log("t.b=%s", t.b)
        log("t.c=%s", t.c)
		print("t:", t)
		print("ret:", ret)
		log()

		ret = getmetatable(t)
		print("mt:", mt)
		print("ret:", ret)
		log()

		content.c = 3
		log("t.c=%d", t.c)
		print("t.c=", rawget(t, 'c'))
		log()

	end

    do
        local t2 = {a=1}
        setmetatable(t2, mt2)
        log("t2.a=%s", t2.a)
        log("t2.b=%s", t2.b)
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
		log()

		log("t1")
		for k, v in pairs(t1) do
			print(k, v)
		end
		log()

		log("mt")
		for k, v in pairs(mt) do
			print(k, v)
		end
		log()

		rawset(t1, 3, 3)

		log("t1")
		for k, v in pairs(t1) do
			print(k, v)
		end
		log()

		log("mt")
		for k, v in pairs(mt) do
			print(k, v)
		end
		log()

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

function test7()
	do
		-- weak table
		-- weak key
		local mt = {__mode = "k"}

		local t1 = {}
		setmetatable(t1, mt)

		local key = {}
		t1[key] = 1

		for k, v in pairs(t1) do print(k, v) end
		log()

		key = {}
		t1[key] = 2
		log("before gc")
		for k, v in pairs(t1) do print(k, v) end
		log()

		collectgarbage()
		log("after gc")
		for k, v in pairs(t1) do print(k, v) end
		log()

	end

	do
		-- weak table
		-- weak value
		local mt = {__mode = "v"}

		local t1 = {}
		setmetatable(t1, mt)

		local value = {}
		t1[1] = value

		for k, v in pairs(t1) do print(k, v) end
		log()

		value = {}
		t1[2] = value
		log("before gc")
		for k, v in pairs(t1) do print(k, v) end
		log()

		collectgarbage()
		log("after gc")
		for k, v in pairs(t1) do print(k, v) end
		log()

	end

	do
		-- weak table
		-- weak key & value
		local mt = {__mode = "kv"}

		local t1 = {}
		setmetatable(t1, mt)

		do
			local key = {}
			t1[key] = 1
			local value = {}
			t1[1] = value

			log("before gc")
			for k, v in pairs(t1) do print(k, v) end
			log()
		end

		collectgarbage()
		log("after gc")
		for k, v in pairs(t1) do print(k, v) end
		log()

	end

	do
		-- weak table
		-- weak key & value
		-- string will not gc
		local mt = {__mode = "kv"}

		local t1 = {}
		setmetatable(t1, mt)

		do
			local key = "hello"
			t1[key] = 1
			local value = "world"
			t1[1] = value

			log("before gc")
			for k, v in pairs(t1) do print(k, v) end
			log()
		end

		collectgarbage()
		log("after gc")
		for k, v in pairs(t1) do print(k, v) end
		log()

	end

	return 0
end

function test8()

	do
		local mt = {}
		mt.a = 1
		mt.__index = mt
		mt.__gc = function() print("do gc") end

		local t = {}
		setmetatable(t, mt)
		print("1")
		t = nil
		print("2")
		collectgarbage()
		print("3")

	end
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
,	test7
,	test8
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

