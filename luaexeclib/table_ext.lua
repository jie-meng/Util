
-- declare local variables
--// exportstring(string)
--// returns a "Lua" portable version of the string
local function exportstring(s)
    return string.format("%q", s)
end

local table_ext = {}

--// The Save Function
function table_ext.save(tbl, filename)
    local charS,charE = "   ","\n"
    local file,err = io.open( filename, "wb" )
    if err then return err end

    -- initiate variables for save procedure
    local tables,lookup = { tbl },{ [tbl] = 1 }
    file:write( "return {"..charE )

    for idx,t in ipairs( tables ) do
        file:write( "-- Table: {"..idx.."}"..charE )
        file:write( "{"..charE )
        local thandled = {}

        for i,v in ipairs( t ) do
        thandled[i] = true
        local stype = type( v )
        -- only handle value
        if stype == "table" then
            if not lookup[v] then
                table.insert( tables, v )
                lookup[v] = #tables
            end
            file:write( charS.."{"..lookup[v].."},"..charE )
        elseif stype == "string" then
            file:write(  charS..exportstring( v )..","..charE )
        elseif stype == "number" then
            file:write(  charS..tostring( v )..","..charE )
        end
        end

        for i,v in pairs( t ) do
        -- escape handled values
        if (not thandled[i]) then
        
            local str = ""
            local stype = type( i )
            -- handle index
            if stype == "table" then
                if not lookup[i] then
                    table.insert( tables,i )
                    lookup[i] = #tables
                end
                str = charS.."[{"..lookup[i].."}]="
            elseif stype == "string" then
                str = charS.."["..exportstring( i ).."]="
            elseif stype == "number" then
                str = charS.."["..tostring( i ).."]="
            end
        
            if str ~= "" then
                stype = type( v )
                -- handle value
                if stype == "table" then
                    if not lookup[v] then
                    table.insert( tables,v )
                    lookup[v] = #tables
                    end
                    file:write( str.."{"..lookup[v].."},"..charE )
                elseif stype == "string" then
                    file:write( str..exportstring( v )..","..charE )
                elseif stype == "number" then
                    file:write( str..tostring( v )..","..charE )
                end
            end
        end
        end
        file:write( "},"..charE )
    end
    file:write( "}" )
    file:close()
end

--// The Load Function
function table_ext.load(sfile)
    local ftables,err = loadfile(sfile)
    if err then return _,err end
    local tables = ftables()
    for idx = 1,#tables do
        local tolinki = {}
        for i,v in pairs( tables[idx] ) do
        if type( v ) == "table" then
            tables[idx][i] = tables[v[1]]
        end
        if type( i ) == "table" and tables[i[1]] then
            table.insert( tolinki,{ i,tables[i[1]] } )
        end
        end
        -- link indices
        for _,v in ipairs( tolinki ) do
        tables[idx][v[2]],tables[idx][v[1]] =  tables[idx][v[1]],nil
        end
    end
    return tables[1]
end

function table_ext.copy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}      
        for orig_key, orig_value in pairs(orig) do
            copy[orig_key] = orig_value        
        end
    else -- number, string, boolean, etc
        copy = orig
    end

    return copy
end

function table_ext.clone(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[table_ext.clone(orig_key)] = table_ext.clone(orig_value)
        end
        setmetatable(copy, table_ext.clone(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end

    return copy
end

function table_ext.foreach(tb, func)
    for k, v in pairs(tb) do
        func(k, v)
    end
end

return table_ext
