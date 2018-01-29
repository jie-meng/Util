require('modulization')
local table_ext = require('table_ext')

local Spec = class('Spec')

function Spec:ctor()
    self.success_ = 0
    self.fail_ = 0
    self.indent_ = 0
    self.before_funcs_ = {}
    self.after_funcs_ = {}
end

function Spec:describe(name, callable)
    self:addIndent()
    
    print(self:getIndentWhiteSpace() .. name)
    callable()
    
    self:removeBeforeAfterFunc()
    self:removeIndent()
end

function Spec:it(name, callable)
    for i=1, self.indent_ do
        if self.before_funcs_[i] then
            self.before_funcs_[i]()
        end
    end

    print(self:getIndentWhiteSpace() .. "  - ".. name)
    local ok, err = pcall(callable)
    if not ok then
        print(err)
        self.fail_ = self.fail_ + 1
    else
        self.success_ = self.success_ + 1
    end
    
    for i=self.indent_, 1, -1 do
        if self.after_funcs_[i] then
            self.after_funcs_[i]()
        end
    end
end

function Spec:beforeEach(callable)
    if self.indent_ > 0 then
        self.before_funcs_[self.indent_] = callable    
    end
end

function Spec:afterEach(callable)
    if self.indent_ > 0 then
        self.after_funcs_[self.indent_] = callable
    end
end

function Spec:run()
end

function Spec:getSuccess()
    return self.success_
end

function Spec:getFail()
    return self.fail_
end

function Spec:clear()
    self.success_ = 0
    self.fail_ = 0
    self.indent_ = 0
    table_ext.clear(self.before_funcs_)
    table_ext.clear(self.after_funcs_)
end

function Spec:addIndent()
    self.indent_ = self.indent_ + 1
end

function Spec:removeIndent()
    if self.indent_ > 0 then
        self.indent_ = self.indent_ -1 
    end
end

function Spec:removeBeforeAfterFunc()
    self.before_funcs_[self.indent_] = nil
    self.after_funcs_[self.indent_] = nil
end

function Spec:getIndentWhiteSpace()
    local indent = ''
    for i=1, self.indent_ do
        indent = indent .. '  '
    end
    return indent
end

function Spec:assertTrue(condition, message)
    message = message or ''
    assert(condition, string.format('%s expect true, actual is false. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:assertFalse(condition, message)
    message = message or ''
    assert(not condition, string.format('%s expect false, actual is true. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:assertNil(value, message)
    message = message or ''
    assert(value == nil, string.format('%s expect nil, actual is not nil. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:assertNotNil(value, message)
    message = message or ''
    assert(value ~= nil, string.format('%s expect not nil, actual is nil. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:assertEquals(expect, actual, message)
    message = message or ''
    assert(expect == actual, string.format('%s expect %s, actual is %s. %s', self:__getAssertLocaltionInfo(), tostring(expect), tostring(actual), message))
end

function Spec:assertNotEquals(expect, actual, message)
    message = message or ''
    assert(expect ~= actual, string.format('%s expect not %s, actual is %s. %s', self:__getAssertLocaltionInfo(), tostring(expect), tostring(actual), message))
end

function Spec:assertStringEmpty(str, message)
    message = message or ''
    assert(string.len(str) == 0, string.format('%s expect empty string, actual is not empty string. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:assertStringNotEmpty(str, message)
    message = message or ''
    assert(string.len(str) ~= 0, string.format('%s expect not empty string, actual is empty string. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:assertStringWhiteSpace(str, message)
    message = message or ''
    assert(string.len(string.trim(str)) == 0, string.format('%s expect whitespace string, actual is not whitespace string. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:assertStringNotWhiteSpace(str, message)
    message = message or ''
    assert(string.len(string.trim(str)) ~= 0, string.format('%s expect not whitespace string, actual is whitespace string. %s', self:__getAssertLocaltionInfo(), message))
end

function Spec:__getAssertLocaltionInfo()
    return string.format('["%s"]:%d: ', debug.getinfo(3, 'S').source, debug.getinfo(3, 'l').currentline)
end

return Spec
