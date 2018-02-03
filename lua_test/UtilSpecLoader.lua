require('modulization')
local SpecLoader = import('spec.SpecLoader')
local StrSpec = import('specs.StrSpec')
local FileSpec = import('specs.FileSpec')
local JSONSpec = import('specs.JSONSpec')

local UtilSpecLoader = class('UtilSpecLoader', SpecLoader)

function UtilSpecLoader:load()
    self:add(StrSpec:create())
    self:add(FileSpec:create())
    self:add(JSONSpec:create())
end

return UtilSpecLoader
