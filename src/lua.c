/*

Gravit - A gravity simulator
Copyright 2003-2005 Gerald Kaszuba

Gravit is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Gravit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gravit; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "gravit.h"

#if HAVE_LUA

int luaInit() {

    luaFree();
    
    state.lua = luaL_newstate();
    if (!state.lua) {
        conAdd(LERR, "Error loading LUA");
        return 0;
    }

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM > 500
    //new API for LUA 5.1.x and newer
    luaL_openlibs(state.lua);
    luaopen_base(state.lua);
    luaopen_table(state.lua);
    luaopen_string(state.lua);
    luaopen_math(state.lua);
#else
    //old API for LUA 5.0.x
    luaopen_base(state.lua);
    luaopen_io(state.lua);
    luaopen_table(state.lua);
    luaopen_string(state.lua);
    luaopen_math(state.lua);
#endif

#define AddFunction(a,b) lua_pushcfunction(state.lua, b); lua_setglobal(state.lua, a);

    AddFunction("particle", luag_spawn)
    AddFunction("log", luag_log)
    AddFunction("load", luag_load);

    return 1;

}

void luaFree() {

    if (state.lua)
        lua_close(state.lua);

    state.lua = NULL;

}

void luaHandleError() {

    conAdd(LNORM, "Stack size: %i", lua_gettop(state.lua));

    //const char *err = lua_tostring(state.lua, -1);
    //conAdd(LERR, "%s", err);
    conAdd(LERR, "%s", lua_tostring(state.lua, -1));
    lua_pop(state.lua, 1);
    
}

int luaExecute(char *f) {

    int ret = luaL_loadfile(state.lua, f);
    
    if (ret == LUA_ERRSYNTAX) {
        luaHandleError();
        return 0;
    } else if (ret == LUA_ERRMEM) {
        conAdd(LERR, "Out of memory loading %s", f);        
        return 0;
    } else if (ret == 0) {
        // Fall through
    } else {
        conAdd(LERR, "Unknown Lua error: %i", ret);
        return 0;
    }

    view.recordParticlesDone = 0;
    
    ret = lua_pcall(state.lua, 0, 0, 0);
    if (ret == 0)
        return 1;
    
    if (ret == LUA_ERRRUN) {
        luaHandleError();
        return 0;
    } else if (ret == LUA_ERRMEM) {
        conAdd(LERR, "Out of memory loading %s", f);        
        return 0;
    } 
    
    conAdd(LERR, "Unknown Lua error: %i", ret);
    return 0;

}

void luag_TableToVector(lua_State *L, float *v) {

    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    v[0] = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    v[1] = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_pushstring(L, "z");
    lua_gettable(L, -2);
    v[2] = lua_tonumber(L, -1);
    lua_pop(L, 1);

}

int luag_load(lua_State *L) {
    
    char *s = (char*)lua_tostring(L, -1);
    char *f;
    conAdd(LLOW, s);
    lua_pop(L, 1);
    
    s = va("spawn/%s", s);    
    luaExecute(findFile(s));
    
    return 0;
    
}

int luag_spawn(lua_State *L) {

    particle_t *p;
    particleDetail_t *pd;
    VectorNew(pos);
    VectorNew(vel);
    float mass;
    int id;

    VectorZero(pos);
    VectorZero(vel);
    mass = lua_tonumber(L, -1);
    id = -1;

    lua_pop(L, 1);
    luag_TableToVector(L, vel);

    lua_pop(L, 1);
    luag_TableToVector(L, pos);

    lua_pop(L, 1);
    id = lua_tonumber(L, -1);

    if (id < 0 || id >= state.particleCount) {
        conAdd(LERR, "Particle %i out of range", id);
        return 0;
    }

    p = getParticleFirstFrame(id);
    pd = getParticleDetail(id);

    VectorCopy(pos, p->pos);
    VectorCopy(vel, p->vel);
    pd->mass = mass;

    view.recordParticlesDone++;
    doVideoUpdateInSpawn();

    return 0;

}

int luag_log(lua_State *L) {

    char *s = (char*)lua_tostring(L, -1);
    conAdd(LNORM, s);
    lua_pop(L,1);

    return 0;

}

#else
#pragma message( __FILE__ " : warning : define HAVE_LUA   to enable LUA spawn script support." )
#endif

