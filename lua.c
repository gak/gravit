
#include "gravit.h"

int luaInit() {

	luaFree();

	state.lua = lua_open();
	if (!state.lua) {
		conAdd(LERR, "Error loading LUA");
		return 0;
	}
	luaopen_base(state.lua);
	luaopen_base(state.lua);
	luaopen_table(state.lua);
	luaopen_io(state.lua);
	luaopen_string(state.lua);
	luaopen_math(state.lua);

#define AddFunction(a,b) lua_pushcfunction(state.lua, b); lua_setglobal(state.lua, a);

	AddFunction("particle", luag_spawn)
	AddFunction("log", luag_log)

	return 1;

}

void luaFree() {

	if (state.lua)
		lua_close(state.lua);
	
	state.lua = NULL;

}

int luaExecute(char *f) {

	if (luaL_loadfile(state.lua, f) || lua_pcall(state.lua, 0, 0, 0)) {
		const char *err = lua_tostring(state.lua, -1);
		conAdd(LERR, "%s", err);
		lua_pop(state.lua, 1);
		// luag_stackDump(state.lua, 0);
		conAdd(LNORM, "Stack size: %i", lua_gettop(state.lua));
		return 0;
	}

	return 1;

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
	
	doVideoUpdateInSpawn();

	return 0;

}

int luag_log(lua_State *L) {

	char *s = (char*)lua_tostring(L, -1);
	conAdd(1, s);
	lua_pop(L,1);

	return 0;

}
