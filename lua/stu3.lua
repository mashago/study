#!/usr/local/bin/lua

print("hello stu3")

print("---- about while ----")

count = 0
while (count < 5)
do 
	print("count=" .. count)
	count = count + 1
end

print()

count = 0
while (count < 5)
do 
	print("count=" .. count)
	count = count + 1
	if count == 3 then
		break -- use break to end loop
	end
end


print()

print("---- about for and table ----")

-- for x=start, end(incloud), step(option, default 1)
-- x is a local param
for i = 0, 4, 1
do
	print("i=" .. i)
end

print("i=", i)

print()

for i = 4, 0, -1
do
	print("i=" .. i)
end

print()

-- double for
for i = 1, 3, 1
do
	for j = 1, 2, 1
	do
		print("i=" .. i .. " j=" .. j)
	end
end

print()

g_table_a = {"aaa", "bbb", 123}
print("#g_table_a=" .. #g_table_a)
-- ipairs() get array pair
for k, v in ipairs(g_table_a)
do
	print(k .. " : " .. v)
end

print()

g_table_b = {}
g_table_b["k1"] = "v1"
g_table_b["k2"] = "v2"
g_table_b[2] = "v3"
g_table_b[3] = "v4"
g_table_b[5] = "v5"
print("#g_table_b=" .. #g_table_b) -- funny, when table without table[1], #table will == 0

g_table_b[1] = "v0"
-- #table will only calculate start from 1 and continuity index
-- when table has table[1][2][3][5], #table count [1]~[3]
print("#g_table_b=" .. #g_table_b) 

print("g_table_b:")
-- pairs() get all table pairs
for k, v in pairs(g_table_b)
do
	print(k .. " : " .. v)
end

print()

print("---- about repeat ----")

count = 0
repeat
	print("count=" .. count)
	count = count + 1
until (count > 4)

print()

print("---- about if ----")

count = 10
if count > 100 then
	print("count > 100")
elseif count > 50 then
	print("count > 50")
else
	print("count is else")
end




