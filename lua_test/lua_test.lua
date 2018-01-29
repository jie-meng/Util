require('modulization')

local path, _  = util.splitPathname(util.appPath())
util.setCurrentPath(path .. '/lua_test')

local SpecRunner = import('spec.SpecRunner')
local UtilSpecLoader = import('UtilSpecLoader')

local spec_runner = SpecRunner:create(UtilSpecLoader:create())
if spec_runner:start() then
    os.exit(0)
else
    os.exit(-1)
end