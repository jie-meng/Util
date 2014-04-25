-- run("script/regex.lua")

re = regex.create("([xyz])(?<data>\\d+)")
print(regex.match(re, "33h"))
print(regex.search(re, "xy33"))
print(regex.getMatchedGroupCnt(re))
print(regex.getMatchedGroupByIndex(re, 0))
print(regex.getMatchedGroupByIndex(re, 1))
print(regex.getMatchedGroupByIndex(re, 2))
print(regex.getMatchedGroupByName(re, "data"))
regex.destroy(re)
re = 0

