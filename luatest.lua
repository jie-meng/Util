
function testFile()
    local c = csv.create()
    local r = regex.create("")
    local m = memory.createMemory(20)
    local p = process.create()
    local udp = net.createUdp()
    local tcp = net.createTcp()
    local raw = net.createRaw()
    local mutex = thread.createMutex()
    local lock = thread.createLock()
end

testFile()
