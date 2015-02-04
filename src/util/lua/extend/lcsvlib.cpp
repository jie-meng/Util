#include "lcsvlib.hpp"
#include "lfilelib.hpp"
#include "util/luaextend.hpp"
#include "util/csv.hpp"

using namespace std;

namespace util
{

static int create(lua_State* plua_state)
{
    string file = luaGetString(plua_state, 1, "");
    string delimiter = luaGetString(plua_state, 2, ",");
    string enclosure = luaGetString(plua_state, 3, "\"");

    Csv* pcsv = new Csv(file, delimiter.at(0), enclosure.at(0));

    LuaHeapRecyclerManager::getInstance().addHeapObject(plua_state, (void*)pcsv, deleteVoid<Csv>);

    luaPushLightUserData(plua_state, (void*)pcsv);

    return 1;
}

static int destroy(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pcsv);

    if (pcsv)
        delete pcsv;

    return 0;
}

static int read(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pcsv->read(luaGetString(plua_state, 2, "")));

    return 1;
}

static int write(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
    {
            luaPushBoolean(plua_state, false);
    }
    else
    {
        string file = luaGetString(plua_state, 2, "");
        if (file == "")
            luaPushBoolean(plua_state, pcsv->write());
        else
            luaPushBoolean(plua_state, pcsv->write(file));
    }

    return 1;
}

static int empty(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
        luaPushBoolean(plua_state, false);
    else
        luaPushBoolean(plua_state, pcsv->empty());

    return 1;
}

static int clear(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (pcsv)
        pcsv->clear();

    return 0;
}

static int getTotalRows(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pcsv->getTotalRows());

    return 1;
}

static int getTotalCols(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pcsv->getTotalCols());

    return 1;
}

static int getCellValue(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
    {
        luaPushString(plua_state, "");
    }
    else
    {
        int row = luaGetInteger(plua_state, 2, -1);
        int col = luaGetInteger(plua_state, 3, -1);

        if (row < 0 || col <0)
            luaPushString(plua_state, "");
        else
            luaPushString(plua_state, pcsv->getCellValue(row, col));
    }

    return 1;
}

static int setCellValue(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
    {
        luaPushBoolean(plua_state, false);
    }
    else
    {
        int row = luaGetInteger(plua_state, 2, -1);
        int col = luaGetInteger(plua_state, 3, -1);

        if (row < 0 || col <0)
            luaPushBoolean(plua_state, false);
        else
            luaPushBoolean(plua_state, pcsv->setCellValue(row, col, luaGetString(plua_state, 4, "")));
    }

    return 1;
}

static int addRow(lua_State* plua_state)
{
    Csv* pcsv = static_cast<Csv*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pcsv)
    {
        luaPushBoolean(plua_state, false);
    }
    else
    {
        int count = luaGetTop(plua_state);
        if (count > 0)
        {
            vector<string> vec;
            for (int i=0; i<count; ++i)
                vec.push_back(luaGetString(plua_state, i+1, ""));

            luaPushBoolean(plua_state, pcsv->addRow(vec));
        }
        else
        {
            luaPushBoolean(plua_state, false);
        }
    }

    return 1;
}

static const u_luaL_Reg csv_lib[] =
{
    {"create", create},
    {"destroy", destroy},
    {"read", read},
    {"write", write},
    {"empty", empty},
    {"clear", clear},
    {"getTotalRows", getTotalRows},
    {"getTotalCols", getTotalCols},
    {"getCellValue", getCellValue},
    {"setCellValue", setCellValue},
    {"addRow", addRow},

    {0, 0}
};

int lualibCsvCreate(lua_State* plua_state)
{
    luaCreateLib(plua_state, (u_luaL_Reg*)csv_lib);
    return 1;
}

}
