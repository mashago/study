
function func_t13(n)
	print("n=", n)

	local t = mylib13:new()
	print("t:get_a()=", t:get_a())
	t:set_a(100)
	print("t:get_a()=", t:get_a())
end
