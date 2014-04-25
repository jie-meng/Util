--------------------------------
-- class : CmdLineParser
--------------------------------
CmdLineParser = {}

-- field
CmdLineParser.cs_ = false
CmdLineParser.kvdel_ = " "
CmdLineParser.cmd_ = ""
CmdLineParser.kv_map_ = {}

-- method
function CmdLineParser:new(cs, kvdel)
	o = {}
	setmetatable(o, self)
	self.__index = self
	
	o.cs_ = cs
	o.kvdel_ = kvdel
	o.cmd_ = ""
	o.kv_map_ = {}
	
	return o
end

function CmdLineParser:setFormat(cs, kvdel)
	self.cs_ = cs
	self.kvdel_ = kvdel
end

function CmdLineParser:isCaseSensitive()
	return self.cs_
end

function CmdLineParser:parse(cmdline)
	
	self.cmd_ = ""
	self.kv_map_ = {}
	
	local beg = 1
	while true do
		local i, j, k, v = string.find(cmdline, "[-](%w+)%s*" .. self.kvdel_ .. "%s*(%S+)", beg)
		if i == nil then 
			break
		end
		
		if 1 == beg and i > 1 then
			self.cmd_ = string.sub(cmdline, 1, i-1)
			self.cmd_ = string.sub(self.cmd_, string.find(self.cmd_, "%w+"))
			if not self:isCaseSensitive() then
				self.cmd_ = string.lower(self.cmd_)
			end
		end
		
		-- add key and value to map
		if not self:isCaseSensitive() then
			k = string.lower(k)
		end
		self.kv_map_[k] = v
		
		beg = j + 1
	end
end

function CmdLineParser:getCmd()
	return self.cmd_
end

function CmdLineParser:hasKey(key)
	if not self:isCaseSensitive() then
		key = string.lower(key)
	end
	return nil ~= self.kv_map_[key]
end

function CmdLineParser:getValue(key)
	if not self:isCaseSensitive() then
		key = string.lower(key)
	end
	return self.kv_map_[key]
end

function CmdLineParser:getNumValue(key)
	if not self:isCaseSensitive() then
		key = string.lower(key)
	end
	return self.kv_map_[key] + 0
end

--[[
function example()
	parser = CmdLineParser:new()
	print(parser:isCaseSensitive())
	parser:parse("cmD -Fuck Shit -cao sb -ri 2.32")
	
	print(parser:getCmd())
	print(parser:hasKey("FuCk"))
	print(parser:getValue("rI"))
	
	if 2.32 == parser:getNumValue("rI") then
		print("ok")
	end
end
--]]