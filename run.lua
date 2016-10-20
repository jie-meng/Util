print("Running started ...\n")

if strContains(platformInfo(), "win", false) then
    os.execute("Util.exe")
else
    os.execute("./Util")
end

print("\n\nRunning stopped.")
