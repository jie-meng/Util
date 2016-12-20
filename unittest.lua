local Tools = require("tools")

if Tools.make() then
    print("UnitTest started ...\n")
    os.execute(Tools.getAppName() .. " $")
    print("UnitTest stopped.\n")
end
