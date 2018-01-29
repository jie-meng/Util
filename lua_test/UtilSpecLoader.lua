require('modulization')

local SpecLoader = import('spec.SpecLoader')

local TestSpec = import('specs.TestSpec')

local UtilSpecLoader = class('UtilSpecLoader', SpecLoader)

function UtilSpecLoader:load()
    self:add(TestSpec:create())
end

return UtilSpecLoader