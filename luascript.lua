
function testFile()
    local txtcfg = cfg.createTextCfg()
    txtcfg:load('cfg.txt')
    print(txtcfg:getValue('Application.Port', ''))
end

testFile()
