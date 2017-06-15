#!/usr/local/bin/lua

require "util"

function test1()

	do
		local t = os.time()
		log("t=%d", t)

		t = os.time{year=1989, month=10, day=30, hour=18, min=0, sec=1}
		log("t=%d", t)
		log()

		local d = os.date("*t", t)
		for k, v in pairs(d) do print(k, v) end
		log()

		d = os.date("*t") -- default use now time
		for k, v in pairs(d) do print(k, v) end
		log()

		d = os.date("Y=%Y m=%m d=%d H=%H M=%M S=%S")
		log("d:%s", d)
		log()

		local x = os.clock() -- cpu time
		local s = 0
		for i=1, 10000000 do s = s + i end
		log("use cpu time=%.2f", os.clock() - x)
		log()
	end

	do
		local now = os.time()
		local date = os.date("*t")
		print("date=" .. tableToString(date))
		date.hour = 0
		date.min = 0
		date.sec = 0
		local day_start_time = os.time(date)

		log("now=%d day_start_time=%d", now, day_start_time)

	end

	do
		local now = os.time()
		local days_of_month = os.date("%d",os.time({year=os.date("%Y", now), month=os.date("%m", now)+1,day=0}))
		log("days_of_month=%d", days_of_month)
	end


	return 0
end

function test2()

	log(os.getenv("HOME"))

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

