--[[print("start ss ...")

ss = net.tcpCreate()

--net.tcpSetBlock(ss, 1)
assert(net.tcpBind(ss, "", 9988))

print("bind ok ...")

assert(net.tcpListen(ss, 5))

print("listen ok ...")

a = net.tcpAccept(ss)
assert(a > 0)

print("accept ok ...")

len = net.tcpServerRecv(ss, a)
if len > 0 then
	print(memory.toString(net.tcpGetRecvBuf(ss)))
end

print(len)


net.tcpClose(ss)
net.tcpDestroy(ss)
--]]

ss = net.tcpCreate()
net.tcpInit(ss)
assert(net.tcpConnect(ss, "127.0.0.1", 9966))

net.tcpClientSend(ss, "sb007")

len = net.tcpClientRecv(ss)
if len > 0 then
	print(memory.toString(net.tcpGetRecvBuf(ss)))
end

print(len)

--closeSocket(a)

net.tcpClose(ss)
net.tcpDestroy(ss)