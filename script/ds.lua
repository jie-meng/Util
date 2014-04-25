ds = net.udpCreate()

assert(net.udpBind(ds, "", 9988))

net.udpSetBlock(ds, 0)
len, ip, port = net.udpRecvFrom(ds)

print(len)
print(ip)
print(port)

if len > 0 then
	print(memory.toString(net.udpGetRecvBuf(ds)))
end

net.udpClose(ds)
net.udpDestroy(ds)