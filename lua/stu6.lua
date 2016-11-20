#!/usr/local/bin/lua

print("hello stu6")

print("---------- about coroutine ----------")

-- return a coroutine
g_co_a = coroutine.create(
	function(i)
		print("status 2=" .. coroutine.status(g_co_a))
		print("i=" .. i)
	end
)
print("type(g_co_a)=", type(g_co_a))

print("status 1=" .. coroutine.status(g_co_a))

-- run coroutine, and block until corotine return or yield
g_flag_a, g_ret_a = coroutine.resume(g_co_a, 10)
print("status 3=" .. coroutine.status(g_co_a))
print("g_flag_a=", g_flag_a, " g_ret_a=", g_ret_a)

g_flag_a, g_ret_a = coroutine.resume(g_co_a, 10)
print("g_flag_a=", g_flag_a, " g_ret_a=", g_ret_a)

print()

g_co_b = coroutine.create(
	function(i)
		return i*2
	end
)

g_flag_b, g_ret_b = coroutine.resume(g_co_b, 10)
print("g_flag_b=", g_flag_b, " g_ret_b=", g_ret_b)

print()

g_co_c = coroutine.create(
	function()
		for i = 1, 3 do
			print("i=" .. i)
			print("g_co_c:", coroutine.running())
			coroutine.yield()
		end
	end
)

print("main:", coroutine.running())
g_flag_c, g_ret_c = coroutine.resume(g_co_c)
print("g_flag_c=", g_flag_c, " g_ret_c=", g_ret_c)
g_flag_c, g_ret_c = coroutine.resume(g_co_c)
print("g_flag_c=", g_flag_c, " g_ret_c=", g_ret_c)
g_flag_c, g_ret_c = coroutine.resume(g_co_c)
print("g_flag_c=", g_flag_c, " g_ret_c=", g_ret_c)
g_flag_c, g_ret_c = coroutine.resume(g_co_c)
print("g_flag_c=", g_flag_c, " g_ret_c=", g_ret_c)
g_flag_c, g_ret_c = coroutine.resume(g_co_c)
print("g_flag_c=", g_flag_c, " g_ret_c=", g_ret_c)


print()

function g_func_d(i)
	-- coroutine.yield() will block until coroutine.resume(), return param of resume()
	return coroutine.yield(i * 2)
end

g_co_d = coroutine.create(
	function(x, y)
		print("x=", x, " y=", y)
		local r1 = g_func_d(x)
		print("r1=", r1)

		local r2
		r1, r2 = g_func_d(r1)
		print("r1=", r1, " r2=", r2)

		return "done"
	end
)

g_flag_d, g_ret_d = coroutine.resume(g_co_d, 10, 15)
print("g_flag_d=", g_flag_d, " g_ret_d=", g_ret_d)

g_flag_d, g_ret_d = coroutine.resume(g_co_d, 100)
print("g_flag_d=", g_flag_d, " g_ret_d=", g_ret_d)

g_flag_d, g_ret_d = coroutine.resume(g_co_d, "masha", "peter")
print("g_flag_d=", g_flag_d, " g_ret_d=", g_ret_d)


print()

-- return a function, different way to create a coroutine
g_func_e = coroutine.wrap(
	function(i)
		print("i=" .. i)
	end
)
print("type(g_func_e)=", type(g_func_e))

-- call the function to run coroutine
g_func_e(3)


print()

local productor

function send_item(i)
	coroutine.yield(i)
end

function product(i)
	while true do
		i = i * 2
		send_item(i)
	end
end

productor = coroutine.create(product)

function recv_item(i)
	local status, item = coroutine.resume(productor, i)
	return item
end

function consume()
	for i = 1, 5 do
		local item = recv_item(i)
		print("item=", item)
	end
end

consume()















