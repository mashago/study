#!/usr/local/bin/lua

require "util"

function test1()

	local List = {}
	function List.push_back(list, value)
		local node = list
		while node.next do
			node = node.next
		end
		node.next = {next=nil, value=value}
	end
	
	local list = {next=nil, value=nil}

	for i=1, 10 do
		List.push_back(list, i*2)
	end

	local node = list.next
	while node do
		log(node.value)
		node = node.next
	end

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

