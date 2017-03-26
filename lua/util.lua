

log = function(fmt, ...)
	if fmt == nil then
		return io.stdout:write("\n")
	elseif type(fmt) ~= "string" then
		return print(fmt, ...)	
	else
		return io.stdout:write(string.format(fmt, ...) .. "\n")
	end
end
