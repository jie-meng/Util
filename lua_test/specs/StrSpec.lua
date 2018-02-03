require('modulization')
local Spec = import('spec.Spec')

local StrSpec = class('StrSpec', Spec)

function StrSpec:run()
    self:describe('StrSpec', function ()
        self:describe('util.strTrimLeft', function ()
            self:it([[should return "abcd " when given "  \n\t\rabcd"]], function ()
                self:assertEquals('abcd ', util.strTrimLeft('  \n\t\rabcd '))
            end)
        end)
        
        self:describe('util.strTrimRight', function ()
            self:it([[should return "\nabcd" when given " abcd  \n\t\r"]], function ()
                self:assertEquals('\nabcd', util.strTrimRight('\nabcd  \n\t\r'))
            end)
        end)
        
        self:describe('util.strTrim', function ()
            self:it([[should return "abcd" when given " abcd  \n\t\r"]], function ()
                self:assertEquals('abcd', util.strTrim('\nabcd  \n\t\r'))
            end)
        end)
    end)
end

return StrSpec
