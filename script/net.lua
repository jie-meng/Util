function cmdlineCorutine(process, port)
	local ds = net.udpCreate()
	assert(net.udpBind(ds, "", port), "cmdline coroutine bind to port " .. port .. "fail")
	net.udpSetBlock(ds, 1)
	while true do
		local len = net.udpRecvFrom(ds)
		if len > 0 then
			local cmdline = memory.toString(net.udpGetRecvBuf(ds))
			if not process(cmdline) then
				break
			end
		end
		coroutine.yield()
	end
	net.udpDestroy(ds)
end
