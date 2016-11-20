#!/usr/local/bin/lua

print("hello stu2")

print("---- about param ----")
g_param_a = 10
local g_param_b = 11

print("g_param_a=", g_param_a, " g_param_b=", g_param_b)

function func_a()
	g_param_a = 1
	g_param_c = 12 -- global param
	local param_d = 13    -- local param in function
end

func_a()
print("g_param_a=", g_param_a, " g_param_c=", g_param_c, " param_d=", param_d)

do
	g_param_b = 15
	local param_e = 16  -- local param in do
end
print("g_param_b=", g_param_b, " param_e=", param_e)


print()

g_param_a, g_param_b = 100, 200
print("g_param_a=", g_param_a, " g_param_b=", g_param_b)
g_param_a, g_param_b = g_param_b, g_param_a
print("after swap g_param_a=", g_param_a, " g_param_b=", g_param_b)

print()

g_param_c = 300
print("g_param_a=", g_param_a, " g_param_b=", g_param_b, " g_param_c=", g_param_c)
-- param > value, last param will be nil
g_param_a, g_param_b, g_param_c = 100, 200
print("after assign 1 g_param_a=", g_param_a, " g_param_b=", g_param_b, " g_param_c=", g_param_c)

-- param < value, last value will be ignore
g_param_a, g_param_b, g_param_c = 100, 200, 300, 400
print("after assign 2 g_param_a=", g_param_a, " g_param_b=", g_param_b, " g_param_c=", g_param_c)


print()

print("---- about index ----")

g_table_a = {}

g_table_a[1] = "masha"
g_table_a["k1"] = "v1"
g_table_a["k2"] = 10
g_table_a[2] = 11
g_table_a[3] = nil
print("#g_table_a=" .. #g_table_a)
for k, v in pairs(g_table_a) do
	print(k .. "=" .. v)
end

print("g_table_a[1]=", g_table_a[1])
print("g_table_a[\"k1\"]=", g_table_a["k1"])
print("g_table_a[\"k2\"]=", g_table_a.k2) -- string key can call by table.key
-- print("g_table_a.1=", g_table_a.1) -- error





