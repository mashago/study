#!/usr/local/bin/lua

require "util"

function test1()

	do
		function mk_iter(t)
			local i = 0
			return function () i = i + 1; return t[i] end
		end

		local t = {10, 20, 30}
		local iter = mk_iter(t)
		while true do
			local x = iter()
			if x == nil then break end
			log("x=%d", x)
		end
		log()

		for x in mk_iter(t) do
			log("x=%d", x)
		end

	end

	return 0
end

function test2()

	-- for <var-list> in <exp-list> do
	-- var-list: _1, _2,...  _1 is control_value, when _1 == nil, loop break
	-- exp-list: canbe a iter factory call, like ipairs(xxx)
	-- exp-list: <iter_function>, <constant_status>, <init_control_value>
	-- var-list = iter_function(constant_status, control_value)

	do
		-- no upvalue iter factory
		function mk_iter(t)
			return function (_t, pos) 
				log("pos=%d", pos)
				pos = pos + 1
				if _t[pos] == nil then
					return nil -- nil break
				end
				return pos, _t[pos]
			end, t, 0
		end

		local t = {10, 20, 30}
		for k, v in mk_iter(t) do
			log("k=%d v=%d", k, v)
		end

		log()
	end

	do
		-- has upvalue iter factory
		function mk_iter(t)
			local pos = 0
			return function ()
				-- use pos, t as upvalue
				log("pos=%d", pos)
				pos = pos + 1
				if t[pos] == nil then
					return nil -- nil break
				end
				return pos, t[pos]
			end, t, 0
		end

		local t = {10, 20, 30}
		for k, v in mk_iter(t) do
			log("k=%d v=%d", k, v)
		end

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

