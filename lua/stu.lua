#!/usr/local/bin/lua

-- lua study file
--[[
create 2016-09-03
--]]

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


