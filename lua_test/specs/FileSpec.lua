require('modulization')
local Spec = import('spec.Spec')

local FileSpec = class('FileSpec', Spec)

function FileSpec:run()
    self:describe('FileSpec', function ()
        self:describe('util.mkFullDir', function ()
            self:it('should has dir "testdir/a/b/c" after execute util.mkFullDir("testdir/a/b/c")', function ()
                util.pathRemoveAll('testdir')
                self:assertTrue(util.mkFullDir('testdir/a/b/c'))
                self:assertTrue(util.isPathDir('testdir/a/b/c'))
                util.pathRemoveAll('testdir')
            end)
        end)
    end)
end

return FileSpec
