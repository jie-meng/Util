include("cmdline.lua")

function main()
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

---- start
main()

