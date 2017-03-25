

log = function(fmt, ...)
	if fmt == nil then
		return io.stdout:write("\n")
	else
		return io.stdout:write(string.format(fmt, ...) .. "\n")
	end
end
