require('modulization')

local SpecLoader = class('SpecLoader')

function SpecLoader:ctor()
    self.specs_ = {}
    self:load()
end

function SpecLoader:getSpecs()
    return self.specs_
end

-- add spec class instance here
function SpecLoader:load()
    --self:add(XXXSpec:create())
end

function SpecLoader:add(spec)
    table.insert(self.specs_, spec)
end

return SpecLoader
