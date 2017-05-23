
local file_ext = {}

function file_ext.findFilesInDir(dir, func)
    local paths = util.findFilesInDir(dir)
    local ret = {}
    for _, v in ipairs(paths) do
        if func(v) then
            table.insert(ret, v)
        end
    end
    return ret
end

function file_ext.findPathInDir(dir, func)
    local paths = util.findPathInDir(dir)
    local ret = {}
    for _, v in ipairs(paths) do
        if func(v) then
            table.insert(ret, v)
        end    
    end
    return ret
end

function file_ext.findFilesInDirRecursively(dir, func)
    local paths = util.findFilesInDirRecursively(dir)
    local ret = {}
    for _, v in ipairs(paths) do
        if func(v) then
            table.insert(ret, v)
        end
    end
    return ret
end

function file_ext.readLines(filename)
    local lines = {}
    local f = io.open(filename, "r")
    if f then
        local line = f:read('*line')
        while line do
            table.insert(lines, line)
            line = f:read("*line")
        end
        io.close(f)
    end
    return lines
end

function file_ext.foreachLine(filename, func)
    local f = io.open(filename, "r")
    if f then
        local info = {}
        local line = f:read('*line')
        local line_number = 1
        while line do
            local ret = func(line, line_number, info)
            if ret then
                break
            end
            line = f:read("*line")
            line_number = line_number + 1
        end
        io.close(f)
    end
end

return file_ext
