
function testFile()

    print(util.platformInfo())
    print(util.strContains("sbb134", "324"))
    
    local re = util.newRegex("\\d+");
    if re:search("asdfasf123kxdasf") then
        print(re:getMatchedGroupByIndex(0))
    end
    
    local cfg = util.newTextCfg()
    cfg:delete()
    print(cfg)
    
end

testFile()
