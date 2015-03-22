#ifndef _FUNCTION_DISPATHER_H_
#define _FUNCTION_DISPATHER_H_

#include "lua_traits.h"
//#include "func_traits.h"
#include "function_dispather_traits.h"

class function_dispather_t
{
    enum function_type_e
    {
        GLOBAL_FUNCTION = 0,
        STATIC_FUNCTION = 1
    };
public:
    template <typename FUNC>
    static lua_function_t  dispather_global_function(lua_State* ls_, FUNC func_)
    {
        return do_dispather_function_i<FUNC, GLOBAL_FUNCTION>(ls_, func_);
    }

    template <typename FUNC>
    static lua_function_t  dispather_static_function(lua_State* ls_, FUNC func_)
    {
        return do_dispather_function_i<FUNC, STATIC_FUNCTION>(ls_, func_);
    }

private:
    template <typename T, function_type_e type>
    static lua_function_t do_dispather_function_i(lua_State* ls_, T func_);
};

template <typename T, function_dispather_t::function_type_e type>
lua_function_t function_dispather_t::do_dispather_function_i(lua_State* ls_, T func_)
{
    /*
    auto dump = [&](lua_State* ls_){
        int i;
        int top = lua_gettop(ls_);
        for (i = 1; i <= top; i++) {
            int t = lua_type(ls_, i);
            switch (t) {
                case LUA_TSTRING:
                    printf("%s", lua_tostring(ls_, i));
                    break;
                case LUA_TBOOLEAN:
                    printf(lua_toboolean(ls_, i) ? "true" : "false");
                    break;
                case LUA_TNUMBER:
                    printf("%g", lua_tonumber(ls_, i));
                    break;
                default:
                    printf("%s", lua_typename(ls_, t));
                    break;
            }
            printf("\n");
        }
            printf("======================\n");
    };
    */

    lua_function_t closure_func = &function_dispather_traits_t<T, type>::closure_function;
    void* user_data_ptr = lua_newuserdata(ls_, sizeof(func_));
    new(user_data_ptr) T(func_);
    lua_pushcclosure(ls_, closure_func, 1);
    return closure_func;
}

#endif
