function os.capture(cmd, raw)
	local f = assert(io.popen(cmd, 'r'))
	local s = assert(f:read('*a'))
	f:close()
	if (raw) then return s end
	s = string.gsub(s, '^%s+', '')
	s = string.gsub(s, '%s+$', '')
	s = string.gsub(s, '[\n\r]+', ' ')
	return s
end


function preinst()
	local out = os.capture("ls", 1)
	out = out .. "\n"
	out = out .. " pre-install script finished"

	return true, out
end

function postinst()
	local out = "Post installed script called"

	return true, out
end
