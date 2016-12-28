local Tools = require("tools")

if Tools.make() then
    print("Run app started ...\n")
    os.execute(Tools.getAppName() .. ' ./luascript.lua')
    print("\nRun app stopped.\n")
end