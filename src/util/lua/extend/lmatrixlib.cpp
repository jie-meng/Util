#include "lmatrixlib.hpp"
#include "lfilelib.hpp"
#include "util/luaextend.hpp"
#include "util/matrix.hpp"
#include "util/unittest.hpp"

using namespace std;

namespace util
{

static int create(lua_State* plua_state)
{
    int row = luaGetInteger(plua_state, 1, 0);
    int col = luaGetInteger(plua_state, 2, 0);
    int type = luaGetInteger(plua_state, 3, 0);

    Matrix* pm = new Matrix((size_t)row, (size_t)col, (Matrix::MatrixType)type);

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm);

    luaPushLightUserData(plua_state, (void*)pm);

    return 1;
}

static int destroy(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));

    LuaHeapRecyclerManager::getInstance().removeHeapObject(plua_state, (void*)pm);

    if (pm)
        delete pm;

    return 0;
}

static int clear(lua_State* plua_state)
{
    LuaHeapRecyclerManager::getInstance().clear(plua_state, typeid(Matrix));
    return 0;
}

static int toString(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        luaPushString(plua_state, "");
    else
        luaPushString(plua_state, toString(*pm));

    return 1;
}

static int rows(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pm->rows());

    return 1;
}

static int cols(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        luaPushInteger(plua_state, -1);
    else
        luaPushInteger(plua_state, pm->cols());

    return 1;
}

static int get(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-get: null pointer");
    }
    else
    {
        int i = luaGetInteger(plua_state, 2, -1);
        int j = luaGetInteger(plua_state, 3, -1);

        if (i < 0 || j < 0)
            throw Exception("LuaExtend-matrix-get: coordinate error");

        luaPushDouble(plua_state, (*pm)(i, j));
    }

    return 1;
}

static int set(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-get: null pointer");
    }
    else
    {
        int i = luaGetInteger(plua_state, 2, -1);
        int j = luaGetInteger(plua_state, 3, -1);
        int v = luaGetInteger(plua_state, 4, 0);

        if (i < 0 || j < 0)
            throw Exception("LuaExtend-matrix-get: coordinate error");

        (*pm)(i, j) = v;
    }

    return 0;
}

static int setElements(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        luaPushBoolean(plua_state, false);
    }
    else
    {
        int count = luaGetTop(plua_state);
        if (count > 1)
        {
            vector<Matrix::value_type> vec;
            for (int i=2; i<=count; ++i)
                vec.push_back(luaGetDouble(plua_state, i, 0));

            luaPushBoolean(plua_state, pm->setElements(vec));
        }
        else
        {
            luaPushBoolean(plua_state, false);
        }
    }

    return 1;
}

static const u_luaL_Reg matrix_lib[] =
{
    {"create", create},
    {"destroy", destroy},
    {"clear", clear},
    {"toString", toString},
    {"rows", rows},
    {"cols", cols},
    {"get", get},
    {"set", set},

    {"setElements", setElements},

    {0, 0}
};

int lualibMatrixCreate(lua_State* plua_state)
{
    luaCreateLib(plua_state, (u_luaL_Reg*)matrix_lib);
    return 1;
}

}
