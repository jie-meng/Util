#include "lmatrixlib.hpp"
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
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "tostring", pm, "null pointer");

    luaPushString(plua_state, toString(*pm));

    return 1;
}

static int rows(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "rows", pm, "null pointer");

    luaPushInteger(plua_state, pm->rows());

    return 1;
}

static int cols(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "cols", pm, "null pointer");

    luaPushInteger(plua_state, pm->cols());

    return 1;
}

static int get(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "get", pm, "null pointer");

    int i = luaGetInteger(plua_state, 2, -1);
    int j = luaGetInteger(plua_state, 3, -1);

    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "get", i>=0 && i<(int)pm->rows() && j>=0 && j<(int)pm->cols(), "sub-index error");

    luaPushDouble(plua_state, (*pm)(i, j));

    return 1;
}

static int set(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "set", pm, "null pointer");

    int i = luaGetInteger(plua_state, 2, -1);
    int j = luaGetInteger(plua_state, 3, -1);
    double v = luaGetDouble(plua_state, 4, 0);

    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "set", i>=0 && i<(int)pm->rows() && j>=0 && j<(int)pm->cols(), "sub-index error");

    (*pm)(i, j) = v;

    return 0;
}

static int setElements(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "setElements", pm, "null pointer");

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

    return 1;
}

static int empty(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "empty", pm, "null pointer");

    luaPushBoolean(plua_state, pm->empty());

    return 1;
}

static int square(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "square", pm, "null pointer");

    luaPushBoolean(plua_state, pm->square());

    return 1;
}

static int elements(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "elements", pm, "null pointer");

    luaPushInteger(plua_state, (int)pm->elements());

    return 1;
}

static int rank(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "rank", pm, "null pointer");

    luaPushInteger(plua_state, (int)pm->rank());

    return 1;
}

static int determinant(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "determinant", pm, "null pointer");

    luaPushDouble(plua_state, (int)pm->determinant());

    return 1;
}

static int invertable(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "invertable", pm, "null pointer");

    luaPushBoolean(plua_state, pm->invertable());

    return 1;
}

static int inverse(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "inverse", pm, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->inverse();

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int transpose(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "transpose", pm, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->transpose();

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int conjugate(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "conjugate", pm, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->conjugate();

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int adjoint(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "adjoint", pm, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->adjoint();

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int addScalar(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "addScalar", pm, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->add(luaGetInteger(plua_state, 2, 0));

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int subtractScalar(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "subtractScalar", pm, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->substract(luaGetInteger(plua_state, 2, 0));

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int multiplyScalar(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "multiplyScalar", pm, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->multiply(luaGetInteger(plua_state, 2, 0));

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int divideScalar(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "divideScalar", pm, "null pointer");

    int s = luaGetInteger(plua_state, 2, 0);
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "divideScalar", s, "cannot divide by 0");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = pm->divide(s);

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int add(lua_State* plua_state)
{
    Matrix* plhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    Matrix* prhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 2, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "add", plhs && prhs, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = (*plhs) + (*prhs);

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int subtract(lua_State* plua_state)
{
    Matrix* plhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    Matrix* prhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 2, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "subtract", plhs && prhs, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = (*plhs) - (*prhs);

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int multiply(lua_State* plua_state)
{
    Matrix* plhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    Matrix* prhs = static_cast<Matrix*>(luaGetLightUserData(plua_state, 2, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "multiply", plhs && prhs, "null pointer");

    Matrix* pm_ret = new Matrix();
    (*pm_ret) = (*plhs) * (*prhs);

    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

    return 1;
}

static int getRow(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "getRow", pm, "null pointer");

    int row = luaGetInteger(plua_state, 2, -1);
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "getRow", row >=0 && row <(int)(pm->rows()), "row number error");

    size_t i;
    for (i=0; i<pm->cols(); ++i)
        luaPushDouble(plua_state, (*pm)(row, i));

    return (int)i;
}

static int getCol(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "getCol", pm, "null pointer");

    int col = luaGetInteger(plua_state, 2, -1);
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "getCol", col >=0 && col <(int)(pm->cols()), "col number error");

    size_t i;
    for (i=0; i<pm->rows(); ++i)
        luaPushDouble(plua_state, (*pm)(i, col));

    return (int)i;
}

static int clone(lua_State* plua_state)
{
    Matrix* pm = static_cast<Matrix*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibMatrix, "clone", pm, "null pointer");

    Matrix* pm_ret = new Matrix(*pm);
    LuaHeapRecyclerManager::getInstance().addHeapObject<Matrix>(plua_state, (void*)pm_ret);
    luaPushLightUserData(plua_state, (void*)pm_ret);

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
    {"addScalar", addScalar},
    {"subtractScalar", subtractScalar},
    {"multiplyScalar", multiplyScalar},
    {"divideScalar", divideScalar},
    {"add", add},
    {"subtract", subtract},
    {"multiply", multiply},
    {"getRow", getRow},
    {"getCol", getCol},
    {"clone", clone},

    {0, 0}
};

int lualibMatrixCreate(lua_State* plua_state)
{
    luaCreateLib(plua_state, (u_luaL_Reg*)matrix_lib);
    return 1;
}

}
