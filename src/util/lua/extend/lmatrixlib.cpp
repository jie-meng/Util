#include "lmatrixlib.hpp"
#include "lfilelib.hpp"
#include "util/luaextend.hpp"
#include "util/matrix.hpp"

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
        throw Exception("LuaExtend-matrix-toString: null pointer");
    else
        luaPushString(plua_state, toString(*pm));

    return 1;
}

static int rows(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-rows: null pointer");
    else
        luaPushInteger(plua_state, pm->rows());

    return 1;
}

static int cols(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-cols: null pointer");
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
            throw Exception("LuaExtend-matrix-get: sub-index error");

        luaPushDouble(plua_state, (*pm)(i, j));
    }

    return 1;
}

static int set(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-set: null pointer");
    }
    else
    {
        int i = luaGetInteger(plua_state, 2, -1);
        int j = luaGetInteger(plua_state, 3, -1);
        int v = luaGetInteger(plua_state, 4, 0);

        if (i < 0 || j < 0)
            throw Exception("LuaExtend-matrix-set: sub-index error");

        (*pm)(i, j) = v;
    }

    return 0;
}

static int setElements(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-setElements: null pointer");
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

static int empty(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-empty: null pointer");
    else
        luaPushBoolean(plua_state, pm->empty());

    return 1;
}

static int square(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-square: null pointer");
    else
        luaPushBoolean(plua_state, pm->square());

    return 1;
}

static int elements(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-elements: null pointer");
    else
        luaPushInteger(plua_state, (int)pm->elements());

    return 1;
}

static int rank(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-rank: null pointer");
    else
        luaPushInteger(plua_state, (int)pm->rank());

    return 1;
}

static int determinant(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-determinant: null pointer");
    else
        luaPushDouble(plua_state, (int)pm->determinant());

    return 1;
}

static int invertable(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
        throw Exception("LuaExtend-matrix-invertable: null pointer");
    else
        luaPushBoolean(plua_state, pm->invertable());

    return 1;
}

static int inverse(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-inverse: null pointer");
    }
    else
    {
        Matrix* pm_ret = new Matrix();
        (*pm_ret) = pm->inverse();

        LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
        luaPushLightUserData(plua_state, (void*)pm_ret);
    }

    return 1;
}

static int transpose(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-transpose: null pointer");
    }
    else
    {
        Matrix* pm_ret = new Matrix();
        (*pm_ret) = pm->transpose();

        LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
        luaPushLightUserData(plua_state, (void*)pm_ret);
    }

    return 1;
}

static int conjugate(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-conjugate: null pointer");
    }
    else
    {
        Matrix* pm_ret = new Matrix();
        (*pm_ret) = pm->conjugate();

        LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
        luaPushLightUserData(plua_state, (void*)pm_ret);
    }

    return 1;
}

static int adjoint(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-adjoint: null pointer");
    }
    else
    {
        Matrix* pm_ret = new Matrix();
        (*pm_ret) = pm->adjoint();

        LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
        luaPushLightUserData(plua_state, (void*)pm_ret);
    }

    return 1;
}

static int add(lua_State* plua_state)
{
    Matrix* plhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    Matrix* prhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 2, 0));
    if (!plhs || !prhs)
    {
        throw Exception("LuaExtend-matrix-add: null pointer");
    }
    else
    {
        Matrix* pm_ret = new Matrix();
        (*pm_ret) = (*plhs) + (*prhs);

        LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
        luaPushLightUserData(plua_state, (void*)pm_ret);
    }

    return 1;
}

static int subtract(lua_State* plua_state)
{
    Matrix* plhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    Matrix* prhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 2, 0));
    if (!plhs || !prhs)
    {
        throw Exception("LuaExtend-matrix-subtract: null pointer");
    }
    else
    {
        Matrix* pm_ret = new Matrix();
        (*pm_ret) = (*plhs) - (*prhs);

        LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
        luaPushLightUserData(plua_state, (void*)pm_ret);
    }

    return 1;
}

static int multiply(lua_State* plua_state)
{
    Matrix* plhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    Matrix* prhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 2, 0));
    if (!plhs || !prhs)
    {
        throw Exception("LuaExtend-matrix-multiply: null pointer");
    }
    else
    {
        Matrix* pm_ret = new Matrix();
        (*pm_ret) = (*plhs) * (*prhs);

        LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
        luaPushLightUserData(plua_state, (void*)pm_ret);
    }

    return 1;
}

static int getRow(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-multiply: null pointer");
    }
    else
    {
        int row = luaGetInteger(plua_state, 2, -1);
        if (row >=0 && row <(int)(pm->rows()))
        {
            size_t i;
            for (i=0; i<pm->cols(); ++i)
                luaPushDouble(plua_state, (*pm)(row, i));

            return (int)i;
        }
        else
        {
            throw Exception("LuaExtend-matrix-getCol: col number error");
        }
    }
}

static int getCol(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    if (!pm)
    {
        throw Exception("LuaExtend-matrix-multiply: null pointer");
    }
    else
    {
        int col = luaGetInteger(plua_state, 2, -1);
        if (col >=0 && col <(int)(pm->cols()))
        {
            size_t i;
            for (i=0; i<pm->rows(); ++i)
                luaPushDouble(plua_state, (*pm)(i, col));

            return (int)i;
        }
        else
        {
            throw Exception("LuaExtend-matrix-getCol: col number error");
        }
    }
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
    {"empty", empty},
    {"square", square},
    {"elements", elements},
    {"rank", rank},
    {"determinant", determinant},
    {"invertable", invertable},
    {"inverse", inverse},
    {"transpose", transpose},
    {"conjugate", conjugate},
    {"adjoint", adjoint},
    {"add", add},
    {"subtract", subtract},
    {"multiply", multiply},
    {"getRow", getRow},
    {"getCol", getCol},

    {0, 0}
};

int lualibMatrixCreate(lua_State* plua_state)
{
    luaCreateLib(plua_state, (u_luaL_Reg*)matrix_lib);
    return 1;
}

}
