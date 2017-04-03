#!/usr/local/bin/lua

require "util"

gv1 = 1000
gv2 = "masha"

function test1()

	log("gv1=%d", gv1)
	log("gv2=%s", gv2)

	local key
	local value

	-- get global variable by _G
	-- _G is just a global table which save environment table
	key = "gv1"
	log("_G[key]=%d", _G[key])
	key = "gv2"
	log("_G[key]=%s", _G[key])
	log()

	key = "gv1"
	value = "new value"
	_G[key] = value
	log("_G[key]=%s", _G[key])
	

	return 0
end

function getfield(f)
	local t = _G
	for w in string.gmatch(f, "[%w_]+") do
		t = t[w] 
		if t == nil then
			return nil
		end
	end
	return t
end

function setfield(f, v)
	local t = _G
	for w, d in string.gmatch(f, "([%w_]+)(%.?)") do
		if d == "." then -- not last word
			t[w] = t[w] or {}
			t = t[w]
		else
			-- last word
			t[w] = v
		end
	end
end

function test2()

	local v
	local field = "ga.gb.gc"
	v = getfield(field)
	print(field, v)

	setfield(field, 10)
	v = getfield(field)
	print(field, v)
	print(ga.gb.gc)
	print(_G["ga.gb.gc"])


	return 0
end

function test3()

	ga = 10 -- a global in environment
	local oldenv = getfenv()
	log("ga=%d", ga)
	log("oldenv.ga=%d", oldenv.ga)
	log("_G.ga=%d", _G.ga) -- _G is just a normal global table which point to environment table
	log()

	local log = log

	local env = {} -- an empty environment
	setfenv(1, env) -- 1 for current function, 2 for the function calling this function, ...

	-- all global variable lost becaust empty environment, inclue _G
	if _G == nil then
		log("_G == nil")
	end

	-- oldenv is a local variable, will still here
	if oldenv ~= nil then
		log("oldenv ~= nil")
	end

	if print == nil then
		log("print == nil")
	end

	if oldenv.print ~= nil then
		log("oldenv.print ~= nil")
	end

	log()

	_G = oldenv -- change _G will not change environment, environment still empty
	if print == nil then
		log("print == nil")
	end
	
	return 0
end

function test4()

	ga = 41
	local oldenv = getfenv()
	local newenv = {}
	setmetatable(newenv, {__index = oldenv})
	setfenv(1, newenv)

	log("ga=%d", ga) -- ga in oldenv

	ga = 42 -- new a ga in newenv, cover oldenv ga
	log("ga=%d", ga)

	log("_G.ga=%d", _G.ga) -- _G is in oldenv
	log()

	_G.ga = 44
	log("ga=%d", ga)
	log("_G.ga=%d", _G.ga)


	return 0
end

function test5()

	function factory()
		return function()
			return ga -- return a global variable in environment
		end
	end

	ga = 10

	f1 = factory()
	f2 = factory()
	print(f1())
	print(f2())
	log()

	setfenv(f1, {ga = 20})
	print(f1())
	print(f2())

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

