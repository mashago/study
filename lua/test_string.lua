#!/usr/local/bin/lua

require("util")

--[[
log = function(fmt, ...)
	if fmt == nil then
		return io.stdout:write("\n")
	else
		return io.stdout:write(string.format(fmt, ...) .. "\n")
	end
end
--]]

function test1()

	local x = "hello world"
	log(type(x))

	local y = "h\101\108\108\111"
	log("y=%s", y)

	local z = "\0481\0492\50"
	log("z=%s", z)

	z = 1 .. 2 .. 3
	log("z=%s", z)
	log()

	z = 101
	log("type(z)=%s", type(z))
	log("z=%d", z)
	z = tostring(z)
	log("type(z)=%s", type(z))
	log("z=%s", z)
	log()

	log('type("" .. 1)=%s', (type("" .. 1)))
	log()

	z = "10" + 1
	log("type(z)=%s", type(z))
	log("z=%d", z)
	log()

	z = "10" + "100"
	log("type(z)=%s", type(z))
	log("z=%d", z)
	log()

	z = "10" * "20"
	log("type(z)=%s", type(z))
	log("z=%d", z)
	log()

	-- z = "10A" + 1 -- error

	z = tonumber("10A")
	if z == nil then
		log("tonumber('10A') is nil")
	end
	log()

	print(tostring(10) == "10")
	print(10 .. "" == "10")
	log()

	z = "hello"
	log("z=%s", z)
	log("#z=%d", #z)
	log("string.len(z)=%d", string.len(z))
	z = "hello\0world"
	log("z=%s", z)
	log("#z=%d", #z)
	log("string.len(z)=%d", string.len(z))

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

	-- string.sub(s, from, to)
	print("string.sub(x, 1, 1)=", string.sub(x, 1, 1))
	print("string.sub(x, 1, 5)=", string.sub(x, 1, 5))
	print("string.sub(x, -1, -1)=", string.sub(x, -1, -1))
	print("string.sub(x, -5, -1)=", string.sub(x, -5, -1))
	print("string.sub(x, 5, -5)=", string.sub(x, 5, -5))

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
	--[[
	-- . all char
	-- %a alphabet
	-- %c control char
	-- %d num
	-- %l lower-case alphabet
	-- %p punctuation
	-- %s space
	-- %u upper-case alphabet
	-- %w alphabet and num
	-- %x hex num
	-- %z '\0'
	-- ( ) . % + - * ? [ ] ^ $ magic char
	-- % can convert magic char
	-- [] char-set, [%w_] means alphabet and num amd _
	-- [^] char-set complement, [^%d] means except num
	-- + 1 or more times
	-- * 0 or more times, max match
	-- - 0 or more times, min match
	-- ? optional, [+-]?
	-- ^  start from, ^%s*
	-- $  end with, %s*$
	-- (?) capture, %1 is first capture
	--]]

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
	x = "123Hello_World123"
	print("x=" .. x)
	print("string.gsub(x, \"o\", \"O\")=", string.gsub(x, "o", "O"))
	print("string.gsub(x, \"o\", \"O\", 1)=", string.gsub(x, "o", "O", 1))
	print("string.gsub(x, \"%a\", \" \")=", string.gsub(x, "%a", " "))
	print("string.gsub(x, \"%d\", \" \")=", string.gsub(x, "%d", " "))
	print("string.gsub(x, \"%d+\", \"\")=", string.gsub(x, "%d+", ""))
	print("string.gsub(x, \"[%d_]\", \"\")=", string.gsub(x, "[%d]", ""))
	print("string.gsub(x, \"[^%d_]\", \"\")=", string.gsub(x, "[^%d]", ""))
	print()

	function trim(s)
		return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
	end
	x = "  123Hello World123  "
	print("trim(" .. x .. ")=", trim(x))
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

	-- capture ()
	buffer = "123abc"
	buffer =string.gsub(buffer, "(%d+)(%a+)", "%2%1")
	print("4 buffer=[" .. buffer .. "]")

	print()

	-- replace from table
	local t = {player="masha", x="peter"}
	buffer = "player is best"
	buffer = string.gsub(buffer, "%w+", t)
	print("4 buffer=[" .. buffer .. "]")

	-- replace from function, replace by capture data
	buffer = "$player is best, $x is loser"
	buffer = string.gsub(buffer, "$(%w+)", function (n) return tostring(t[n]) end)
	print("6 buffer=[" .. buffer .. "]")

	-- capture empty
	print(string.match("abcde", "()b()c()"))
	print()

	-- \x ==> char(x)
	buffer = "%123abc&567def*"
	print(string.match(buffer, "[\97-\122]+"))
	-- string.gmatch: like a iterator use string.match
	for w in string.gmatch(buffer, "[\97-\122]+") do
		print("w=" .. w)
	end

	buffer = "$Rev: 3588 $"
	print(string.gsub(buffer, "[ ]*$[%a:]*[ ]*", ""))
	print(string.match(buffer, "%$Rev:%s*(%d+)%s*%$"))

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

