#!/usr/local/bin/lua

require "util"

-- store all class all member map
local _class={} 

-- _class = {[class_ptr] = { k=v, __index=func search from super}}
-- x_class = {__index = _class[x_class], __newindex=func set into _class[x_class]} 
 
function class(super)
	local class_type={}
	class_type.ctor=false
	class_type.super=super

	class_type.new=function(...) 
		-- function to new a obj
		local obj={}
		do
			local create
			create = function(c,...)
				-- recursion do create super
				if c.super then
					create(c.super,...)
				end
				-- call constructor
				if c.ctor then
					c.ctor(obj,...)
				end
			end

			create(class_type,...)
		end

		-- set obj get metatable(get from base class)
		setmetatable(obj,{ __index=_class[class_type] })
		return obj
	end

 	-- set base class set metatable
	-- therefore base class set data or function, obj can get it
	local vtbl={}
	_class[class_type]=vtbl
	setmetatable(class_type,{__newindex=
		function(t,k,v)
			print("__newindex k=", k)
			vtbl[k]=v
		end
	})
 
 	-- set base class get metatable(get from super)
	-- if base class has super, base class can get data from super which not found in base class
	if super then
		setmetatable(vtbl,{__index=
			function(t,k)
				print("__index k=", k)
				local ret=_class[super][k]
				vtbl[k]=ret
				return ret
			end
		})
	end
 
	return class_type
end

function test1()

	-- define BaseClass
	BaseClass = class()
	function BaseClass:ctor(x)
		print("BaseClass ctor")
		self.x = x
	end

	function BaseClass:func1()
		print("BaseClass func1", self.x)
	end

	function BaseClass:func2()
		print("BaseClass func2")
	end

	function BaseClass:func3()
		print("BaseClass func3")
	end

	function BaseClass:func3()
		print("BaseClass func3 again")
	end

	print("111")


	-- define ExtendClass
	ExtendClass = class(BaseClass)
	function ExtendClass:ctor(a, b)
		print("ExtendClass ctor")
		self.a = a
		self.b = b
	end

	function ExtendClass:print_ab()
		print("ExtendClass print_ab", self.a, self.b)
	end

	function ExtendClass:func2()
		print("ExtendClass func2")
	end

	print("222")

	-- define MoreExtendClass
	ThirdExtendClass = class(ExtendClass)
	function ThirdExtendClass:ctor(a, b)
		print("ThirdExtendClass ctor")
		self.a = a
		self.b = b
	end

	print("222")

	local b1 = BaseClass.new(10)
	b1:func1()
	b1:func2()

	log()

	local e1 = ExtendClass.new(20, 30)
	e1:func1()
	e1:print_ab()
	e1:func2()

	log()

	local te1 = ThirdExtendClass.new(400, 500)
	te1.func3()
	local func4 = te1.func4

	return 0
end

function test2()

	BaseClass = {
		new = function (self, x, y)
			x = x or 0
			y = y or 0

			local obj = {}
			setmetatable(obj, self)

			obj.x = x
			obj.y = y

			return obj
		end,

		info = function (self)
			print("x=", self.x, " y=", self.y)
		end,

		step = function (self)
			return 5
		end,

		go = function (self)
			self.x = self.x + self.step()
		end,
	}
	BaseClass.__index = BaseClass -- for obj

	-- an extend class
	ExtClass = {
		new = function (self, x, y, z)
			x = x or 0
			y = y or 0
			z = z or 0

			local obj = {}
			obj = BaseClass:new(x, y)
			setmetatable(obj, self)

			obj.z = z

			return obj
		end,

		info = function (self)
			print("x=", self.x, " y=", self.y, " z=", self.z)
		end,

		step = function (self) -- override
			return 10
		end,
	}
	setmetatable(ExtClass, BaseClass) -- extend
	ExtClass.__index = ExtClass -- for obj

	local b1 = BaseClass:new(10, 20)
	b1:info()
	b1:go()
	b1:info()

	log()

	local e1 = ExtClass:new(10, 20, 30)
	e1:info()
	e1:go()
	e1:info()

	return 0
end

function test_notyet()
	return 0
end

test_list =
{
	test1
,	test2
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

