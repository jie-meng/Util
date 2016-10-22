local Tools = require("tools")

if Tools.make() then
    print("Run app started ...\n")
    os.execute(Tools.getAppName())
    print("\n\nRun app stopped.\n")
end