
function gogo(cmd)
    print('ss:' .. cmd)
end

function testFile()
    print(util.platformInfo())
    print(util.strContains("sbb134", "324"))
    
    local re = util.newRegex("\\d+");
    if re:search("asdfasf123kxdasf") then
        print(re:getMatchedGroupByIndex(0))
    end
    
    local cfg = util.newTextCfg()
    print(cfg:getCfgFile())
    cfg:delete()
    print(cfg)
    
    local parser = util.newCmdLineParser()
    print(parser:parse('iperf -c -ip 192.168.0.1 -port 33 -t'))
    
    local maker = util.newCmdLineMaker()
    maker:setCmd('ping')
    maker:addKeyValue('t', '30')
    maker:addKeyValue('b', '2.5')
    print(maker:getCmdLine())
    maker:sort('b')
    print(maker:getCmdLine())
    
    local m = util.newMemory(20);
    util.strcpy(m:buf(), 'ABC')
    print(util.memContent(m:buf(), m:size()))
    print(util.memToString(m:buf(-19)))
end

testFile()
