require('modulization')

local SpecLoader = import('spec.SpecLoader')

local FileSpec = import('specs.FileSpec')

local UtilSpecLoader = class('UtilSpecLoader', SpecLoader)

function UtilSpecLoader:load()
    self:add(FileSpec:create())
end

return UtilSpecLoader
