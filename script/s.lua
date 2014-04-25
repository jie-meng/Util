p1 = process.create()
p2 = process.create()

--process.start(p1, "ss.exe", "", true, true, "script/b.lua", "output", p1, "p1")
--process.start(p2, "ss.exe", "", true, true, "script/b.lua", "output", p2, "p2")

process.start(p1, "ss.exe", "", true, true, "script/b.lua", "output")
process.start(p2, "ss.exe", "", true, true, "script/b.lua", "output")

io.read()

process.destroy(p1)
process.destroy(p2)

print("over")