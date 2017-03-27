#!/usr/local/bin/lua

require "util"

function test1()

	local filename = 'io_test_file.txt'

	io.write('hello world\n') -- default use stdout

	io.output(filename) -- set output file
	io.write('hello world2\n1234\n') -- write to file

	io.output(io.stdout)
	io.write('hello world3\n')

	-- cannot get file? data not write into file?
	io.input(filename) 
	local t = io.read("*all") -- empty
	print(t)

	return 0
end

function test2()

	local filename = 'io_test_file.txt'

	do
		io.input(filename) -- set input

		local t = io.read("*all")
		print(t)
		log()
	end

	do
		io.input(filename) -- set input

		for i=1, math.huge do
			local line = io.read("*line") -- io.read default is *line
			if not line then break end
			log("i=%d line=[%s]", i, line)
		end
		log()
	end

	do
		io.input(filename) -- set input

		for line in io.lines() do
			print(line)
		end
		log()
	end

	-- io.read(*number)
	-- io.read(n) -- read n char
	-- io.read(0) -- check is at file end, '' or nil

	return 0
end

function test3()

	print(io.open('not_exists_file.txt', 'r'))
	local filename = 'io_test_file.txt'
	log()

	local filename = 'io_test_file.txt'
	local f = assert(io.open(filename, 'w'))
	f:write("hahaha\nhello 123\n")
	f:flush()
	f:close()

	f = assert(io.open(filename, 'r'))
	local t = f:read('*all')
	print(t)
	log()


	f:seek("set", 0) -- file:seek("set"/"cur"/"end", offset default 0)
	local BUFFER_SIZE = 2^13
	while true do
		local line, rest = f:read(BUFFER_SIZE, "*line") -- buffer to speedup, line + rest + \n = a full line
		if not line then break end
		if rest then line = line .. rest end
		print(line)
	end

	f:close()

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

