#include "lcsvext.hpp"
#include "util/luaextend.hpp"
#include "util/csv.hpp"

using namespace std;

namespace util
{
    
const std::string kCsvHandle = "Csv*";

static int createCsv(lua_State* plua_state)
{
    string file = luaGetString(plua_state, 1, "");
    string delimiter = luaGetString(plua_state, 2, ",");
    string enclosure = luaGetString(plua_state, 3, "\"");

    LuaObject<Csv>* p = luaNewEmptyObject<Csv>(plua_state, kCsvHandle);
    p->setData(new Csv(file, delimiter.at(0), enclosure.at(0)));

    return 1;
}

static int destroy(lua_State* plua_state)
{
    return luaObjectDestroy<Csv>(plua_state, kCsvHandle);
}

static int read(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);
    luaPushBoolean(plua_state, pcsv->read(luaGetString(plua_state, 2, "")));

    return 1;
}

static int write(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);

    string file = luaGetString(plua_state, 2, "");
    if (file == "")
        luaPushBoolean(plua_state, pcsv->write());
    else
        luaPushBoolean(plua_state, pcsv->write(file));

    return 1;
}

static int empty(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);
    luaPushBoolean(plua_state, pcsv->empty());

    return 1;
}

static int clear(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);
    pcsv->clear();

    return 0;
}

static int rows(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);
    luaPushInteger(plua_state, pcsv->rows());

    return 1;
}

static int cols(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);
    luaPushInteger(plua_state, pcsv->cols());

    return 1;
}

static int getCellValue(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);
    int row = luaGetInteger(plua_state, 2, -1);
    int col = luaGetInteger(plua_state, 3, -1);

    luaExtendAssert(plua_state, kLuaExtendLibUtil, "getCellValue", row>=0 && row<(int)pcsv->rows() && col>=0 && col<(int)pcsv->cols(), "sub-index error");
    luaPushString(plua_state, pcsv->getCellValue(row, col));

    return 1;
}

static int setCellValue(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);

    int row = luaGetInteger(plua_state, 2, -1);
    int col = luaGetInteger(plua_state, 3, -1);

    if (row>=0 && row<(int)pcsv->rows() && col>=0 && col<(int)pcsv->cols())
        luaPushBoolean(plua_state, pcsv->setCellValue(row, col, luaGetString(plua_state, 4, "")));
    else
        luaPushBoolean(plua_state, false);

    return 1;
}

static int addRow(lua_State* plua_state)
{
    Csv* pcsv = luaGetObjectData<Csv>(plua_state, kCsvHandle);
    int count = luaGetTop(plua_state);
    if (count > 1)
    {
        vector<string> vec;
        for (int i=2; i<=count; ++i)
            vec.push_back(luaGetString(plua_state, i, ""));

        luaPushBoolean(plua_state, pcsv->addRow(vec));
    }
    else
    {
        luaPushBoolean(plua_state, false);
    }

    return 1;
}

static int toString(lua_State* plua_state)
{
    return luaObjectToString<Csv>(plua_state, kCsvHandle);
}

static const LuaReg csv_lib[] =
{
    {"newCsv", createCsv},
    
    {0, 0}
};

static const LuaReg csv_obj_lib[] = {
    {"delete", destroy},
    {"read", read},
    {"write", write},
    {"empty", empty},
    {"clear", clear},
    {"rows", rows},
    {"cols", cols},
    {"getCellValue", getCellValue},
    {"setCellValue", setCellValue},
    {"addRow", addRow},
    {"__gc", destroy},
    {"__tostring", toString},
    
    {0, 0}
};

void extendCsv(lua_State* plua_state)
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)csv_lib);
    luaCreateMeta(plua_state, kCsvHandle, (LuaReg*)csv_obj_lib);
}

}
