
local file_ext = {}

function file_ext.findFilesInDir(dir, func)
    local paths = util.findFilesInDir(dir)
    local ret = {}
    for _, v in ipairs(paths) do
        if not func or func(v) then
            table.insert(ret, v)
        end
    end
    return ret
end

function file_ext.findPathInDir(dir, func)
    local paths = util.findPathInDir(dir)
    local ret = {}
    for _, v in ipairs(paths) do
        if not func or func(v) then
            table.insert(ret, v)
        end    
    end
    return ret
end

function file_ext.findFilesInDirRecursively(dir, func)
    local paths = util.findFilesInDirRecursively(dir)
    local ret = {}
    for _, v in ipairs(paths) do
        if not func or func(v) then
            table.insert(ret, v)
        end
    end
    return ret
end

function findFilesFlatRecursively(path_deque, path_filter, file_filter, output, limit)
    if #path_deque > 0 then
        local dir = table.remove(path_deque, 1)
        local files = file_ext.findFilesInDir(dir, file_filter)
        for _, v in ipairs(files) do
            table.insert(output, v)
            if limit and #output >= limit then
                return
            end
        end
        local dirs = file_ext.findPathInDir(dir, path_filter)
        for _, v in ipairs(dirs) do
            table.insert(path_deque, v)
        end
        
        findFilesFlatRecursively(path_deque, path_filter, file_filter, output, limit)
    end
end

function file_ext.findFilesFlat(dir, path_filter, file_filter, limit)
    local output = {}
    local deque = {}
    table.insert(deque, dir)
    findFilesFlatRecursively(deque, path_filter, file_filter, output, limit)
    return output
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

function file_ext.makeDirRecursively(dir)
    if not util.isPathDir(dir) then
        if not util.mkDir(dir) then
            local parent_path, _ = util.splitPathname(dir)
            file_ext.makeDirRecursively(parent_path)
            util.mkDir(dir)
        end
    end
end

return file_ext
