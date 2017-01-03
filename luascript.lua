
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
    
--        local cmdline = util.newCmdLine(gogo)
--        print(cmdline)
--        cmdline:start()
--        cmdline:delete()
--        print(cmdline)
    
    local parser = util.newCmdLineParser()
    print(parser:parse('iperf -c -ip 192.168.0.1 -port 33 -t'))
end

testFile()
