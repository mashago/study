#!/usr/local/bin/lua
--#!/usr/bin/lua

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
	local x = "123Hello World123"
	print("x=" .. x)

	print("string.upper(x)=", string.upper(x))
	print("string.lower(x)=", string.lower(x))
	print("string.reverse(x)=", string.reverse(x))
	print()

	print("string.len(x)=", string.len(x))
	print("#x=", #x)
	print("string.rep(x, 1)=", string.rep(x, 1))
	print("string.rep(x, 2)=", string.rep(x, 2))
	print()

	local fmt = "%d %s"
	print("fmt=", fmt)
	print("string.format(fmt)=", string.format(fmt, 10, "hahaha"))

	print("string.char(97, 98, 99, 100)=", string.char(97, 98, 99, 100))

	-- string.byte(input, index = 1)
	print("string.byte(\"abc\")=", string.byte("abc"))
	print("string.byte(\"abc\", 2)=", string.byte("abc", 2))
	print()
	return 0
end

function test4()

	-- about pattern

	local x = "123Hello World123"
	print("x=" .. x)

	-- string.find(buffer, pattern, start index=1)
	-- return start index and end index
	print("string.find(x, \"123\")=", string.find(x, "123"))
	print("string.find(x, \"123\", 1)=", string.find(x, "123", 1))
	print("string.find(x, \"123\", 2)=", string.find(x, "123", 2))
	print("string.find(x, \"123\", 15)=", string.find(x, "123", 15))
	print("string.find(x, \"123\", 16)=", string.find(x, "123", 16))
	print("string.find(x, \"456\")=", string.find(x, "456"))
	print()

	-- string.match(buffer, pattern)
	-- return match string
	x = "today is 01/14/2017"
	print("x=" .. x)
	print("string.match(x, \"2017\")=", string.match(x, "2017"))
	print("string.match(x, \"%d+/%d+/%d+\")=", string.match(x, "%d+/%d+/%d+"))
	print()

	-- string.gsub(input, pattern, replace string, times)
	x = "123Hello World123"
	print("x=" .. x)
	print("string.gsub(x, \"o\", \"O\")=", string.gsub(x, "o", "O"))
	print("string.gsub(x, \"o\", \"O\", 1)=", string.gsub(x, "o", "O", 1))
	print("string.gsub(x, \"%d+\", \" \")=", string.gsub(x, "%d+", " "))
	print()

	local buffer = ""

	-- string.gsub
	-- replace from string
	-- %w is letters and integer
	buffer = "$player is best"
	buffer = string.gsub(buffer, "$%w+", "masha")
	print("1 buffer=[" .. buffer .. "]")

	buffer = "$player is best"
	buffer = string.gsub(buffer, "$(%w+)", "masha")
	print("2 buffer=[" .. buffer .. "]")

	buffer = "$player is best"
	buffer = string.gsub(buffer, "$(%w+)", "%1")
	print("3 buffer=[" .. buffer .. "]")

	print()

	local my_table = {player = "masha"}

	-- replace from table
	buffer = "player is best"
	buffer = string.gsub(buffer, "%w+", my_table)
	print("4 buffer=[" .. buffer .. "]")

	-- replace from table, search capture data in table
	buffer = "$player is best"
	buffer = string.gsub(buffer, "$(%w+)", my_table)
	print("5 buffer=[" .. buffer .. "]")

	-- replace from function
	buffer = "$player is best, $a is loser"
	buffer = string.gsub(buffer, "$(%w+)", function (n) return tostring(my_table[n]) end)
	print("6 buffer=[" .. buffer .. "]")

	-- capture empty
	print(string.match("abcde", "()b()c()"))
	print()

	-- \x ==> char(x)
	buffer = "%123abc&567def*"
	print(string.match(buffer, "[\97-\122]+"))
	-- string.gmatch: a iterator like string.match
	for w in string.gmatch(buffer, "[\97-\122]+") do
		print("w=" .. w)
	end

	buffer = "$Rev: 3588 $"
	print(string.gsub(buffer, "[ ]*$[a-zA-Z:]*[ ]*", ""))
	print(string.match(buffer, "%d+"))

	return 0
end

function url_escape(s)
	s = string.gsub(s, "[&=%%%c]", function (c)
		return string.format("%%%02X", string.byte(c))
	end)
	s = string.gsub(s, " ", "+")
	return s
end

function url_encode(params)
	local b = {}
	for k, v in pairs(params) do
		b[#b+1] = (url_escape(k) .. "=" .. url_escape(v))
	end
	return table.concat(b, "&")
end

function url_unescape(s)
	s = string.gsub(s, "+", " ")
	s = string.gsub(s, "%%(%x%x)", function (h)
		return string.char(tonumber(h, 16))
	end)
	return s
end

function url_decode(s)
	local params = {}
	for k, v in string.gmatch(s, "([^&=]+)=([^&=]+)") do
		k = url_unescape(k)
		v = url_unescape(v)
		params[k] = v
	end
	return params
end

function test5()

	local params = { a = 10, b = "hello world", c = "&1%2ha ha" }
	print("params:")
	for k, v in pairs(params) do
		print("k=" .. k .. " v=" .. v)
	end
	print()

	local s = url_encode(params)
	print("s: " .. s)
	print()

	local t = url_decode(s)
	print("t:")
	for k, v in pairs(t) do
		print("k=" .. k .. " v=" .. v)
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

