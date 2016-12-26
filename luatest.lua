
function testFile()
    local c = csv.create()
    print(c)
    c:destroy()
    print(c)
end

testFile()
