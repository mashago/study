#!/usr/local/bin/lua

require "for_test_module"
require "util"

function init_path()
	log("old package.path=%s", package.path)
	local new_path = {}
	table.insert(new_path, package.path)
	table.insert(new_path, './mydir/?.lua')
	package.path = table.concat(new_path, ';')
	log("new package.path=%s", package.path)
end
init_path()

function test1()
	mymodule.printHello()
	require "for_test_module"
	log()

	package.loaded["for_test_module"] = nil
	log()

	require "for_test_module"

	return 0
end

function test2()

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

