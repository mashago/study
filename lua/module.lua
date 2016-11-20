
-- like a lib, not a clase
module = {}
module.constant = "abc123"

function module.func1()
	io.write("module.func1\n")
end

local function func2()
	io.write("private func2\n")
end

function module.func3()
	func2()
end

return module
