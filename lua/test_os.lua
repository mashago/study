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

		local date2 = os.date("*t")
		local tmp = {year=date2.year, month=date2.month, day=date2.day}
		local time = os.time(tmp) -- time in 12:00 !!!
		log("time=%d", time)

	end

	do
		local now = os.time()
		local days_of_month = os.date("%d",os.time({year=os.date("%Y", now), month=os.date("%m", now)+1,day=0}))
		log("days_of_month=%d", days_of_month)
	end


	return 0
end

local function get_day_start_time(input_time)
	local t = input_time or os.time()
	local date = os.date("*t", t)
	-- print("date=" .. tableToString(date))
	date.hour = 0
	date.min = 0
	date.sec = 0
	local day_start_time = os.time(date)
	return day_start_time
end

-- weekday: monday is 1, sunday is 7
local function get_next_weekday_time(weekday, hour, min, sec)
	local now_time = os.time()
	local now_weekday = tonumber(os.date("%w",os.time())) -- %w: monday is 1, wday: sunday is 1
	local day_start_time = get_day_start_time(now_time)

	local dtime = hour * 60 * 60 + min * 60 + sec

	if weekday > now_weekday then
		local next_time = day_start_time + (weekday - now_weekday) * 86400 + dtime
		return next_time
	end

	if weekday < now_weekday then
		local next_time = day_start_time + (weekday + 7 - now_weekday) * 86400 + dtime
		return next_time
	end

	-- same weekday
	local next_time = day_start_time + dtime
	if next_time < now_time then
		next_time = next_time + 7 * 86401
	end
	return next_time
end

function test2()

	local now_time = os.time()
	local now_date = os.date("*t", now_time)
	log("now_time=%d", now_time)
	log("now_date=%s", tableToString(now_date))

	-- date.wday: sunday == 1, monday == 2, 
	-- date %w:   sunday == 0, monday == 1,  
	log("wday=%d", now_date.wday)
	log("weekday=%d", tonumber(os.date("%w",now_time)))
	log("---------------------------------")

	local x_time = now_time + 86400
	local x_date = os.date("*t", x_time)
	log("x_time=%d", x_time)
	log("x_date=%s", tableToString(x_date))
	log("wday=%d", x_date.wday)
	log("weekday=%d", tonumber(os.date("%w",x_time)))
	log()

	do
		local function print_date(d)
			log("year=%d month=%d day=%d hour=%d min=%d sec=%d", d.year, d.month, d.day, d.hour, d.min, d.sec)
		end
		local next_time = get_next_weekday_time(4, 10, 50, 0)
		log("1 next_time=%d", next_time)
		print_date(os.date("*t", next_time))
		log()

		local next_time = get_next_weekday_time(5, 10, 50, 0)
		log("2 next_time=%d", next_time)
		print_date(os.date("*t", next_time))
		log()

		local next_time = get_next_weekday_time(5, 18, 50, 0)
		log("3 next_time=%d", next_time)
		print_date(os.date("*t", next_time))
		log()

		local next_time = get_next_weekday_time(6, 5, 10, 0)
		log("4 next_time=%d", next_time)
		print_date(os.date("*t", next_time))
		log()

	end

	return 0
end

function test3()

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

