require('modulization')
local Spec = import('spec.Spec')

local FileSpec = class('FileSpec', Spec)

function FileSpec:run()
    self:describe('FileSpec', function ()
        local TEST_TEMP_PATH = 'test_temp'
        self:afterEach(function ()
            util.pathRemoveAll(TEST_TEMP_PATH)
        end)

        self:describe('util.mkFullDir', function ()
            self:it(string.format([[should has dir "%s/a/b/c" after execute util.mkFullDir("%s/a/b/c")]], TEST_TEMP_PATH, TEST_TEMP_PATH), function ()
                self:assertTrue(util.mkFullDir(TEST_TEMP_PATH .. '/a/b/c'))
                self:assertTrue(util.isPathDir(TEST_TEMP_PATH .. '/a/b/c'))
            end)
        end)
        
        self:describe('util.fileCopyFullPath', function ()
            self:it('should success when copy file to a non-existing path', function ()
                self:assertTrue(util.fileCopyFullPath('lua_test.lua', TEST_TEMP_PATH .. '/test/abc/x.lua', true))
                self:assertTrue(util.isPathFile(TEST_TEMP_PATH .. '/test/abc/x.lua'))
                self:assertEquals(util.readTextFile('lua_test.lua'), util.readTextFile(TEST_TEMP_PATH .. '/test/abc/x.lua'))
                self:assertFalse(util.fileCopyFullPath('lua_test.lua', TEST_TEMP_PATH .. '/test/abc/x.lua', true))
                self:assertTrue(util.fileCopyFullPath('lua_test.lua', TEST_TEMP_PATH .. '/test/abc/x.lua', false))
                self:assertEquals(util.readTextFile('lua_test.lua'), util.readTextFile(TEST_TEMP_PATH .. '/test/abc/x.lua'))
            end)
        end)

        self:describe('util.copyTree', function ()
            self:it('should success when copyTree a directory', function ()
                --copy single file
                util.copyTree("lua_test.lua", TEST_TEMP_PATH .. "/test/abc/x.lua")
                self:assertTrue(util.isPathFile(TEST_TEMP_PATH .. "/test/abc/x.lua"))
                self:assertEquals(util.readTextFile("lua_test.lua"), util.readTextFile(TEST_TEMP_PATH .. "/test/abc/x.lua"))
                
                --copy tree
                util.copyTree("specs", util.currentPath() .. "/" .. TEST_TEMP_PATH .. "/cpspecs")
                self:assertTrue(util.isPathDir(util.currentPath() .. "/" .. TEST_TEMP_PATH .. "/cpspecs"))
                self:assertTrue(util.isPathFile(util.currentPath() .. "/" .. TEST_TEMP_PATH .. "/cpspecs/FileSpec.lua"))
                self:assertTrue(util.isPathFile(util.currentPath() .. "/" .. TEST_TEMP_PATH .. "/cpspecs/StrSpec.lua"))
            end)
        end)
    end)
end

return FileSpec
