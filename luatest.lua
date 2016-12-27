
function testFile()
    local c = csv.create()
    local r = regex.create("")
    local m = memory.create(20)
    local p = process.create()
    local udp = net.udpCreate()
    local tcp = net.tcpCreate()
    local raw = net.rawCreate()
    local mutex = thread.mutexCreate()
    local lock = thread.lockCreate()
end

testFile()
