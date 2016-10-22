local Tools = require("tools")

if Tools.make() then
    print("UnitTest started ...\n")
    os.execute(Tools.getAppName() .. " $")
    print("\n\nUnitTest stopped.\n")
end
