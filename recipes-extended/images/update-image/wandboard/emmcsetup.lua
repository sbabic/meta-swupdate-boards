function preinst()
	local out = "Pre installed script called"

	return true, out

end

function postinst()
	local out = "Post installed script called"

	return true, out
end
