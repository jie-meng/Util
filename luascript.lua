
function testFile()
    local re = regex.create("\\d+")
    if re:search("sbb123gg") then
        print(re:getMatchedGroupByIndex(0))
        print(re:getMatchedGroupByIndex(1))
    end
end

testFile()
