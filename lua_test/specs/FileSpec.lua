require('modulization')

local Spec = import('spec.Spec')

local FileSpec = class('FileSpec', Spec)

function FileSpec:run()
    self:describe('FileSpec', function ()
        self:it('should has testdir/a/b/c when util.mkFullDir("testdir/a/b/c")', function ()
            self:assertTrue(util.mkFullDir('testdir/a/b/c'))
            self:assertTrue(util.isPathDir('testdir/a/b/c'))
        end)       
    end)
end

return FileSpec
