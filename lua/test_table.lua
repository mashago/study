#!/usr/local/bin/lua

function test1()

	-- array

	local t = {}
	print("type(t)=" .. type(t))

	t = {"masha", "peter", "kelton", "kittie"}
	print("#t=" .. #t)
	if _VERSION == "Lua 5.1" then
	print("table.getn(t)=" .. table.getn(t))
	print("table.maxn(t)=" .. table.maxn(t))
	end

	-- numeric for
	for i=1, #t, 1 do
		print(i .. " : " .. t[i])
	end
	print()

	-- ipairs and pairs
	-- ipairs = index pairs
	for k, v in ipairs(t) do
		print(k .. " : " .. v)
	end
	print()

	for k, v in pairs(t) do
		print(k .. " : " .. v)
	end
	print()

	-- non-exists index member is nil
	if t[4] ~= nil then
		print("t[4] ~= nil")
	end

	if t[5] == nil then
		print("t[5] == nil")
	end
	print()

	-- update
	t[1] = "masha_max"
	for k, v in ipairs(t) do
		print(k .. " : " .. v)
	end
	print()

	print"-----------------"
	print()

	-- pointer copy only
	local t2 = t
	print(t, t2)
	if t == t2 then
		print("t == t2")
	end
	print()

	t2 = nil
	for k, v in ipairs(t) do
		print(k .. " : " .. v)
	end
	print()

	t2 = t
	t2[5] = "five"
	for k, v in ipairs(t) do
		print(k .. " : " .. v)
	end
	print()

	print"------------------------"
	print()

	t2 = {"a", [2]="b"}
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	-- add element into array end
	for i=1, 2, 1 do
		t2[#t2+1] = string.char(98+i)
	end
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	-- table.insert(t, element) -- insert into table end
	-- table.insert(t, position, element)
	table.insert(t2, "new1")
	table.insert(t2, "new2")
	print("insert 1")
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	table.insert(t2, 1, "new3")
	table.insert(t2, 3, "new4")
	print("insert 2")
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	-- table.remove(t) -- remove table end element
	-- table.remove(t, position)
	table.remove(t2)
	print("remove 1")
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	table.remove(t2, 2)
	print("remove 2")
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	-- table.sort(t) -- default compare with <
	-- table.sort(t, function(v1, v2)) -- func return true, v1 before v2
	table.sort(t2)
	print("sort 1")
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	table.sort(t2, function(v1, v2) return v1 > v2 end)
	print("sort 2")
	for k, v in ipairs(t2) do
		print(k .. " : " .. v)
	end
	print()

	-- table.concat(t, sep="")
	-- table.concat(t, sep, start_pos, end_pos=#t)
	local s = table.concat(t2)
	print("concat 1")
	print(s)
	print()

	s = table.concat(t2, ",")
	print("concat 2")
	print(s)
	print()

	s = table.concat(t2, " ", 2)
	print("concat 3")
	print(s)
	print()

	s = table.concat(t2, " ", 2, 4)
	print("concat 4")
	print(s)
	print()

	print"--------------------------------------"
	print()

	-- about remove in table

	local t3 = {1, 3, 5, 5, 7, 7, 4, 4}
	for k, v in ipairs(t3) do
		print(k .. " : " .. v)
	end
	print()
	local copybase = function()
		local t = {}
		for k, v in ipairs(t3) do
			t[k]=v
		end
		return t
	end

	do
		local t4 = copybase()
		print("set nil")
		for k, v in ipairs(t4) do
			if v > 5 then
				t4[k] = nil -- set nil is a very BAD idea
			end
		end

		for k, v in ipairs(t4) do
			print(k .. " : " .. v)
		end
		print()
	end
	
	do
		local t4 = copybase()
		print("remove")
		for k, v in ipairs(t4) do
			if v > 5 then
				table.remove(t4, k) -- remove in asc for also not good
			end
		end

		for k, v in ipairs(t4) do
			print(k .. " : " .. v)
		end
		print()
	end
	
	do
		local t4 = copybase()
		print("remove")
		for i=#t4, 1, -1 do
			if t4[i] > 5 then
				table.remove(t4, i) -- remove in desc for is ok
			end
		end

		for k, v in ipairs(t4) do
			print(k .. " : " .. v)
		end
		print()
	end


	return 0
end

function test2()

	local t = {}
	t["k1"] = "v1"
	t["k2"] = "v2"
	t.k3 = "v3"
	local key = "k4"
	t[key] = "v4"

	-- cannot get map size by # or table.getn()
	print("#t=" .. #t)
	if _VERSION == "Lua 5.1" then
	print("table.getn(t)=" .. table.getn(t))
	print("table.maxn(t)=" .. table.maxn(t))
	end
	print("t[\"k1\"]=" .. t["k1"])
	print("t.k2=" .. t.k2)

	if t.k5 == nil then
		print("t.k5 == nil")
	end

	-- ipairs cannot for loop map element
	print("do ipairs")
	for k, v in ipairs(t) do
		print(i .. " : " .. t[i])
	end
	print()

	-- only pairs can do this
	local size = 0
	print("do pairs")
	for k, v in pairs(t) do
		print(k .. " : " .. v)
		size = size + 1
	end
	print("size=" .. size)
	print()

	-- sort key for map
	local a = {}
	for k, v in pairs(t) do a[#a+1] = k end
	table.sort(a)
	for k, v in ipairs(a) do
		print(v .. " : " .. t[v])
	end
	print()
	
	-- can save index element into map
	t[1] = "new1"
	t[#t+1] = "new2"
	for k, v in pairs(t) do
		print(k .. " : " .. v)
		size = size + 1
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
