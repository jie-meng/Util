require('modulization')

local SpecRunner = import('spec.SpecRunner')
local UtilSpecLoader = import('UtilSpecLoader')

local spec_runner = SpecRunner:create(UtilSpecLoader:create())
spec_runner:start()
