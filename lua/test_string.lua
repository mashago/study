#!/usr/bin/lua

function test1()

	local x = "hello world"
	print(type(x))

	local y = "h\101\108\108\111"
	print("y=" .. y)

	local z = "\0481\0492\50"
	print("z=" .. z)
	print()

	z = 1 .. 2 .. 3
	print("z=" .. z)
	print()

	z = 101
	z = tostring(z)
	print("z=" .. z)
	print(type(z))
	print()

	print(type("" .. 1))
	print()

	z = "10" + 1
	print("z=" .. z)
	print(type(z))
	print()

	z = "10" + "100"
	print("z=" .. z)
	print(type(z))
	print()

	-- z = "10A" + 1 -- error
	z = tonumber("10A")
	if z == nil then
		print("z is nil")
	end
	print()

	z = "hello"
	print("z=" .. z)
	print("#z=" .. #z)
	z = "hello\0world"
	print("z=" .. z)
	print("#z=" .. #z)

	return 0
end

function test2()

	local x = [[
	haha
	bcbc
	123456 ]]
	print("x=[" .. x .. "]")

	x = [=[
	abc
	123]=]
	print("x=[" .. x .. "]")

	return 0
end

function test3()
	x = "123Hello World"
	print("x=" .. x)

	print("string.upper(x)=" .. string.upper(x))
	print("string.lower(x)=" .. string.lower(x))
	print()

	-- string.gsub(input, find string, replace string, times)
	print("string.gsub(x, \"o\", \"O\")=" .. string.gsub(x, "o", "O"))
	print("string.gsub(x, \"o\", \"O\", 1)=" .. string.gsub(x, "o", "O", 1))
	print()

	print("string.find(x, \"Hel\", 1)=" .. string.find(x, "Hel", 1))
	print("string.find(x, \"456\", 1)=", string.find(x, "456", 1))
	print()

	print("string.reverse(x)=" .. string.reverse(x))

	print("string.len(x)=" .. string.len(x))
	print("#x=" .. #x)
	print("string.rep(x, 1)=" .. string.rep(x, 1))
	print("string.rep(x, 2)=" .. string.rep(x, 2))
	print()

	g_string_fmt = "%d %s"
	print("string.format(g_string_fmt)=" .. string.format(g_string_fmt, 10, "hahaha"))

	print("string.char(97, 98, 99, 100)=" .. string.char(97, 98, 99, 100))

	-- string.byte(input, index = 1)
	print("string.byte(\"abc\")=" .. string.byte("abc"))
	print("string.byte(\"abc\", 2)=" .. string.byte("abc", 2))
	print()
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

