require('modulization')

local Spec = import('spec.Spec')

local TestSpec = class('TestSpec', Spec)

function TestSpec:run()
    self:describe('des1', function ()
        self:describe('des2', function ()
            self:it('should return 3 when arguments are (1, 2)', function ()
                self:assertEquals(3, 1 + 2)
            end)
        end)
    end)
end

return TestSpec
