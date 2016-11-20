#!/usr/local/bin/lua

print("hello stu5")

print()
print("---------- about metatable ----------")
-- table inside of the table

print("---- about __index:")

g_table_a = {}
g_metatable_a = {}
setmetatable(g_table_a, g_metatable_a)

print()

g_indextable_b = {k3 = "v3"}
g_table_b = setmetatable({k1 = "v1"}, { __index = g_indextable_b})
g_table_ptr = getmetatable(g_table_b)
print("g_table_b.k1 = " .. g_table_b.k1)
print("g_table_b.k3 = " .. g_table_b.k3)

print()

-- __index only for visit
-- find key from table
-- 1.find from table, if find, return
-- 2.if has no metatable, return nil
-- 3.if metatable.__index is nil, return nil.
-- 4.if metatable.__index is a table, do 1~3 again.
-- 5.if metatable.__index is a function, call the function, pass table and key

g_table_c = setmetatable({k1 = "v1"}, { 
	__index = function (tab, key)
		if key == "k2" then
			return "v2"
		else
			return nil
		end
	end
})
print("g_table_c.k1 = " .. g_table_c.k1)
print("g_table_c.k2 = " .. g_table_c.k2)
print("g_table_c.k3 = " , g_table_c.k3)


print()

print("---- about __newindex:")

-- __newindex only for update or add
-- 1.if table has not the key, if has now __newindex, just set table.key, return
-- 2.if has __newindex, will call __newindex(table, key, value)

g_table_d = setmetatable({k1 = "v1"}, { 
	__newindex = function (tab, k, v)
		if k == "k2" then
			print("avoid set k2")
		else
			-- do rawset avoid call __newindex again
			rawset(tab, k, v)
		end
	end
})

print("g_table_d.k1 = " .. g_table_d.k1)
print("g_table_d.k2 = ", g_table_d.k2)
g_table_d.k2 = "v2"
print("g_table_d.k2 = ", g_table_d.k2)
g_table_d.k3 = "v3"
print("g_table_d.k3 = ", g_table_d.k3)

print()

card_class = {}
card_class = {

	new = function (self, c)
		local proxy = {}
		if c == nil then
			print("c == nil")
			c = {}
		end
		proxy.c = c
		setmetatable(proxy, self)
		return proxy
	end,

	__index = function (t, k)
		local v
		-- use rawget, avoid call __index again
		v = rawget(t.c, k)
		if v == nil then
			v = rawget(card_class, k)
		end
		return v
	end,

	__newindex = function (t, k, v)
		if k == "id" then
			print("cannot change id")
			return
		end
		if k == "name" then
			print("cannot change name")
			return
		end
		rawset(t.c, k, v)
	end,
}

-- deep clone, every pairs and metatable will be copied
function clone(t)
	if type(t) ~= "table" then
		return t
	end

	local copy = {}
	local metatable
	for k, v in pairs(t) do
		local kk, vv
		kk = clone(k)
		vv = clone(v)
		copy[kk] = vv
	end
	metatable = getmetatable(t)
	if metatable ~= nil then
		metatable = clone(metatable)
		setmetatable(copy, metatable)
	end
	return copy
end

boris = {
	id = 1,
	name = "boris skullcrusher",
	power = 10,
	hp = 30,
}

-- boris is a actually card class
card_boris = card_class:new(boris)

card_a = clone(card_boris)
card_b = clone(card_boris)
print("card_a id=" .. card_a.id .. " name=" .. card_a.name .. " power=" .. card_a.power .. " hp=" .. card_a.hp)
print("card_b id=" .. card_b.id .. " name=" .. card_b.name .. " power=" .. card_b.power .. " hp=" .. card_b.hp)

card_a.id = 20
card_a.power = 15
card_a.hp = 40
print("card_a id=" .. card_a.id .. " name=" .. card_a.name .. " power=" .. card_a.power .. " hp=" .. card_a.hp)
print("card_b id=" .. card_b.id .. " name=" .. card_b.name .. " power=" .. card_b.power .. " hp=" .. card_b.hp)


print()

print("---- about __add:")

g_table_h = setmetatable({3, 2, 1}, {
	__add = function(t, newt)
		for i=1, #newt do
			table.insert(t, #t+1, newt[i])
		end
		return t
	end,
})

g_table_i = {6, 5, 4}
g_table_h = g_table_h + g_table_i
for k, v in ipairs(g_table_h) do
	print(k, v)
end


print()

print("---- about __call:")

g_table_h = setmetatable({3, 2, 1}, {
	__call = function(t, newt)
		local sum = 0
		for k, v in ipairs(t) do
			sum = sum + v
		end
		for k, v in ipairs(newt) do
			sum = sum + v
		end
		return sum
	end,
})

g_table_i = {6, 5, 4}
print("g_table_h(g_table_i)=" .. g_table_h(g_table_i))


print()

print("---- about __tostring:")

g_table_h = setmetatable({3, 2, 1}, {
	__tostring = function(t)
		local data = ""
		for i = 1, #t do
			if i ~= 1 then
				data = data .. "," .. t[i]
			else
				data = t[i]
			end
		end
		return data
	end,
})

-- print("g_table_h=" .. g_table_h) -- error
print(g_table_h)

print()

print("---- about oo:")

Rectangle = {
	area = 0,
	length = 0,
	breadth = 0,

	printArea = function(self)
		print("1 area=", self.area)
	end,
}

function Rectangle:printArea2()
	print("2 area=", self.area)
end

function Rectangle:new(t)
	t = t or {}

	-- __index metatable will has class member function, therefore new object can access these function
	self.__index = self 
	setmetatable(t, self)

	t.length = t.length or 0
	t.breadth = t.breadth or 0
	t.area = t.length * t.breadth
	return t
end

r1 = Rectangle:new{length = 3, breadth = 6}
r1:printArea() -- will pass itself as param
r1.printArea(r1) -- pass param r1 as self

r1:printArea2() -- caller r1 as self
r1.printArea2(r1) -- param r1 as self
-- r1.printArea2() -- error, need self

print()

r2 = Rectangle:new{length = 4, breadth = 8}
r2:printArea()
r1:printArea()

print()

r1.printArea(r2) -- param r2 will use as self
r1.printArea2(r2) -- param r2 will use as self
r1:printArea(r2) -- param r2 will ignore
r1:printArea2(r2) -- param r2 will igonre

print()

r3 = Rectangle:new{length = 4}
r3:printArea()
r1:printArea()
r2:printArea()

