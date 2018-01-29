function preinst()
	local out = "Post installed script called"

	return true, out

end

function postinst()
	local out = "Post installed script called"

	return true, out
end
