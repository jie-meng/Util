require('modulization')

local SpecRunner = class('SpecRunner')

function SpecRunner:ctor(spec_loader)
    self.spec_loader_ = spec_loader
    self.success_ = 0
    self.fail_ = 0
end

function SpecRunner:start()
    print('\n\n********************************************************\n                    SpecRunner start\n********************************************************\n\n')

    self:clear()

    for i, v in ipairs(self.spec_loader_:getSpecs()) do
        v:clear()
        v:run()
        self.success_ =  self.success_ + v:getSuccess()
        self.fail_ = self.fail_ + v:getFail()
    end
    
    self:summary()
    
    print('\n\n********************************************************\n                    SpecRunner end\n********************************************************\n\n')
    return self.fail_ == 0
end

function SpecRunner:clear()
    self.success_ = 0
    self.fail_ = 0
end

function SpecRunner:summary()
    local emoji = 'ᕦ(ò_ó)ᕤ'
    if self.fail_ > 0 then
        emoji = '(T__T)'
    end
    print(string.format('\n\n  TestResult: Run %d tests totally, %d success, %d fail.        %s\n', self.success_ + self.fail_, self.success_, self.fail_, emoji))
end
    
return SpecRunner
