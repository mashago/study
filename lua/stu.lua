#!/usr/local/bin/lua

-- lua study file
--[[
create 2016-09-03
--]]

print("hello stu")

g_a = 10
print("1 g_a=" .. g_a)
g_a = nil
-- print("2 g_a=" .. g_a) -- error
print("2 g_a=", g_a) -- , can print nil

print()
print("---------- about type ----------")

print(type("Hello World"))
print(type(10.1))
print(type(print))
print(type(true))
print(type(nil))
print(type(type(nil)))
print(type(g_b))

print()
print("---------- about boolean ----------")
print("g_bool_a = false")
g_bool_a = false
if g_bool_a then
	print("1 g_bool_a true")
else
	print("1 g_bool_a false")
end

print("g_bool_a = nil")
g_bool_a = nil
if g_bool_a then
	print("2 g_bool_a true")
else
	print("2 g_bool_a false")
end

print()
print("---------- about string ----------")
g_string_a = "good morning"
print("1 g_string_a=" .. g_string_a)
g_string_a = [[
haha
bcbc
123456 ]]
print("2 g_string_a=" .. g_string_a)
print "print hello without ()"

print()

g_string_b = "11" * 2
print("1 g_string_b=" .. g_string_b)
print("1 type(g_string_b)=" .. type(g_string_b))

g_string_b = "11" * "2"
print("2 g_string_b=" .. g_string_b)
print("2 type(g_string_b)=" .. type(g_string_b))

g_string_b = "11 * 2"
print("3 g_string_b=" .. g_string_b)
print("3 type(g_string_b)=" .. type(g_string_b))
print("#g_string_b=" .. #g_string_b)

print()

g_string_c = "123Hello World"
print("g_string_c=" .. g_string_c)

print("string.upper(g_string_c)=" .. string.upper(g_string_c))
print("string.lower(g_string_c)=" .. string.lower(g_string_c))
-- string.gsub(input, find string, replace string, times)
print("string.gsub(g_string_c, \"o\", \"O\")=" .. string.gsub(g_string_c, "o", "O"))
print("string.gsub(g_string_c, \"o\", \"O\", 1)=" .. string.gsub(g_string_c, "o", "O", 1))
print("string.find(g_string_c, \"123\", 1)=" .. string.find(g_string_c, "123", 1))
print("string.find(g_string_c, \"456\", 1)=", string.find(g_string_c, "456", 1))
print("string.reverse(g_string_c)=" .. string.reverse(g_string_c))

g_string_fmt = "%d %s"
print("string.format(g_string_fmt)=" .. string.format(g_string_fmt, 10, "hahaha"))

print("string.char(97, 98, 99, 100)=" .. string.char(97, 98, 99, 100))

-- string.byte(input, index = 1)
print("string.byte(\"abc\")=" .. string.byte("abc"))
print("string.byte(\"abc\", 2)=" .. string.byte("abc", 2))
print("string.len(g_string_c)=" .. string.len(g_string_c))
print("string.rep(g_string_c, 1)=" .. string.rep(g_string_c, 1))
print("string.rep(g_string_c, 2)=" .. string.rep(g_string_c, 2))


print()
print("---------- about array and table ----------")

g_table_a = {}
print("type(g_table_a)=" .. type(g_table_a))
g_table_a = {"masha", "peter", "kelton"}
print("1 #g_table_a=" .. #g_table_a)
-- k start from 1
for k, v in pairs(g_table_a) do
	print(k .. " : " .. v)
end

print()
g_table_a["k1"] = "v1"
-- #table will only calculate start from 1 and continuity index
print("2 #g_table_a=" .. #g_table_a) 

print("2 #g_table_a pairs:")
-- pairs get all value
for k, v in pairs(g_table_a) do
	print(k .. " : " .. v)
end

-- ipairs only get index value
print("2 #g_table_a ipairs:")
for k, v in ipairs(g_table_a) do
	print(k .. " : " .. v)
end

print()
g_table_b = {}
g_table_b["k1"] = "v1"
g_table_b["k2"] = "v2"
print("#g_table_b=" .. #g_table_b)
print("g_table_b[\"k1\"]=" .. g_table_b["k1"])
print("g_table_b[\"k2\"]=" .. g_table_b["k2"])
-- print("g_table_b[\"k3\"]=" .. g_table_b["k3"]) -- nil error
if g_table_b["k3"] == nil then
	print("g_table_b[\"k3\"] is nil")
else
	print("g_table_b[\"k3\"] not nil")
end

print()

g_table_c = {}
for i = 1, 3, 1 do
	g_table_c[i] = {} -- define as table
	for j = 1, 2, 1 do
		g_table_c[i][j] = i * j
	end
end

for k1, v1 in ipairs(g_table_c) do
	for k2, v2 in ipairs(v1) do
		print(k1 .. " " .. k2 .. " " .. v2)
	end
end

for i=1, #g_table_c do
	for j=1, #g_table_c[i] do
		print("g_table_c[" .. i .. "][" .. j .. "]=" .. g_table_c[i][j])
	end
end

print()

g_table_d = nil
g_table_e = g_table_d or {}
if g_table_e ~= nil then
	print("g_table_e ~= nil")
else
	print("g_table_e == nil")
end
	
print()

g_table_f = {}
g_table_f["k1"] = "v1"
g_table_f = nil -- gc

g_table_f = {"v1", "v2", "v3"}
g_table_f.a = "a1"  -- means g_table_f["a"] = "a1"
g_table_f["b"] = "b1"

print("g_table_f ipairs:")
for k, v in ipairs(g_table_f) do
	print(k, v)
end
print ""

print("g_table_f pairs:")
for k, v in pairs(g_table_f) do
	print(k, v)
end
print ""

g_table_g = g_table_f -- not copy, just ptr assign
g_table_g[1] = "v10"
print("2 g_table_f:")
for k, v in ipairs(g_table_f) do
	print(k, v)
end
print ""

g_table_g = nil -- just clear ptr
print("3 g_table_f:")
for k, v in ipairs(g_table_f) do
	print(k, v)
end
print ""

-- table.concat only concat table index items
-- NOTE: table.concat may core when table has nil index value
print("table.concat(g_table_f)=" .. table.concat(g_table_f) .. " #g_table_f=" .. #g_table_f)
print("table.concat(g_table_f, \" \")=" ..  table.concat(g_table_f, " ") .. " #g_table_f=" .. #g_table_f)
print("table.concat(g_table_f, \"-\", 2, 3)=" .. table.concat(g_table_f, "-", 2, 3) .. " #g_table_f=" .. #g_table_f)

table.insert(g_table_f, "v4") -- default pos is end
print("after insert(\"v4\") table.concat(g_table_f, \" \")=" ..  table.concat(g_table_f, " ") .. " #g_table_f=" .. #g_table_f)

table.insert(g_table_f, 1, "v0")
print("after insert(1, \"v0\") table.concat(g_table_f, \" \")=" ..  table.concat(g_table_f, " ") .. " #g_table_f=" .. #g_table_f)

table.remove(g_table_f) -- default pos is #table
print("after remove() table.concat(g_table_f, \" \")=" ..  table.concat(g_table_f, " "))

table.remove(g_table_f, 1) 
print("after remove(1) table.concat(g_table_f, \" \")=" ..  table.concat(g_table_f, " "))

table.sort(g_table_f)
print("after sort() table.concat(g_table_f, \" \")=" ..  table.concat(g_table_f, " "))

print()

g_table_h = {"v1", "v2", "v3"};
print("#g_table_h=", #g_table_h) -- 3
g_table_h[4] = "v4"
print("#g_table_h=", #g_table_h) -- 4
g_table_h[7] = "v7"
print("#g_table_h=", #g_table_h) -- 4
g_table_h[6] = "v6"
print("#g_table_h=", #g_table_h) -- 7 , funny....
g_table_h[5] = "v5"
print("#g_table_h=", #g_table_h) -- 7


print()
print("---------- about function ----------")

function func_a()
	print("hello func_a")
end
func_a()

print()

function func_b(num)
	return num * 10
end
print("func_b(10)=" .. func_b(10))
print("type(func_b(10))=" .. type(func_b(10)))

print()

function func_c(num)
	-- muti return
	return num, num * 5, num *10 
end

func_ret1, func_ret2, func_ret3 = func_c(2)
print("1 func_ret1=" .. func_ret1 .. " func_ret2=" .. func_ret2 .. " func_ret3=" .. func_ret3)

func_ret1, func_ret2 = func_c(3)
print("2 func_ret1=" .. func_ret1 .. " func_ret2=" .. func_ret2 .. " func_ret3=" .. func_ret3)


print()

function func_d(tab, fun)
	for k, v in pairs(tab) do
		fun(k, v)
	end
end

g_table_x = {123, "abc", 2.1}

-- use anonymous function
func_d(g_table_x, function(k, v)
	print(k .. "=" .. v)
end)

print()

-- ... for variable element
local function func_e(...)
	local sum = 0;
	local args = {...} -- input params
	for _, v in ipairs(args) do
		sum = sum + v
	end
	return sum / #args
end

print("func_e(1, 2, 3, 4)=" .. func_e(1, 2, 3, 4))

print()

function func_f()
	-- 1.local variable, in stack
	local num = 0
	local step = 2

	-- closure function
	-- 2.create a prototype for new function
	return function()
		-- 3.add upvalue for "num" and "step", ptr to the variable, not copy value, in prototype upvalue list
		num = num + step
		return num
	end
end

local func_ret

-- first closure
-- 4.leaving the variable "num", "step" lifecycle, so copy "num" and "step" value into upvalue, remove ptr to stack variable "num" and "step". closure finish.
local func_closure_a = func_f()
print("func_closure_a", func_closure_a)
print("type(func_closure_a)=", type(func_closure_a))
func_ret = func_closure_a()
print("1 func_ret=", func_ret)
func_ret = func_closure_a()
print("2 func_ret=", func_ret)
func_ret = func_closure_a()
print("3 func_ret=", func_ret)

print()

-- second closure
local func_closure_b = func_f()
print("func_closure_b", func_closure_b)
print("type(func_closure_b)=", type(func_closure_b))
func_ret = func_closure_b()
print("1 func_ret=", func_ret)
func_ret = func_closure_b()
print("2 func_ret=", func_ret)
func_ret = func_closure_b()
print("3 func_ret=", func_ret)

print()

function func_g()
	local num = 1

	local func_inner_a = function()
		-- 1.add upvalue into prototype upvalue list
		print("num=", num)
	end
	
	local func_inner_b = function()
		-- 2.find if already has this upvalue, if find, will not add upvalue again
		num = num * 2
	end

	return func_inner_a, func_inner_b
end

-- 3.two closure function will use the same upvalue "num"
local func_closure_print_a, func_closure_add_a = func_g()
func_closure_print_a()
func_closure_add_a()
func_closure_print_a()
func_closure_add_a()
func_closure_print_a()

print()

local func_closure_print_b, func_closure_bdd_b = func_g()
func_closure_print_b()
func_closure_bdd_b()
func_closure_print_b()
func_closure_bdd_b()
func_closure_print_b()


print()

print("---------- about opeator ----------")

count = 10
count = count + 2
count = count - 2
count = count * 2
count = count / 2
count = count % 2

count = 5
count = count ^ 2
print("count=" .. count)

flag_a = true
flag_b = false
if flag_a and flag_b then
	print("flag_a and flag_b")
end

if flag_a or flag_b then
	print("flag_a or flag_b")
end

if flag_a and not(flag_b) then
	print("flag_a and not(flag_b)")
end

print()

print("---------- about iterator ----------")

function square(max_count, current)
	if current < max_count then
		current = current + 1
		return current, current * current
	end
end

-- iterator: iterator function, const param, control param
-- iter(const param, control param)
for k, v in square, 3, 0 do
	print(k, v)
end

print()

function my_iter(tab, index)
	index = index + 1
	if tab[index] then
		return index, tab[index]
	end
end

function my_ipairs(tab)
	return my_iter, tab, 0
end

g_mytable_a = {10, 20, 30}

-- 1.call my_ipairs, get iter_func, const param, control param
-- 2.call k, v = iter_func(const param, control param)
-- if k == nil, break loop
-- 3.contorl param = k, call iter_func again
print("use my_ipairs")
for k, v in my_ipairs(g_mytable_a) do
	print(k, v)
end

print()

function my_ipairs2(tab)
	local index = 0
	-- local max = #tab
	-- check nil is better
	local max = #(tab or {})

	-- closure function
	return function ()
		index = index + 1
		if (index <= max) then
			return index, tab[index]
		else
			return nil
		end
	end
end

print("use my_ipairs2:")
for k, v in my_ipairs2(g_mytable_a) do
	print(k, v)
end

for k, v in my_ipairs2(nil) do
	print(k, v)
end


