#include "LuaEventSubscription.hpp"

#include <string_view>

using namespace Config::Lua;

static constexpr const char* MT = "HL.EventSubscription";

namespace {
    struct SEventSubscriptionRef {
        CLuaEventHandler* handler = nullptr;
        uint64_t          handle  = 0;
        bool              active  = false;
    };
}

static int eventSubscriptionRemove(lua_State* L) {
    auto* ref = static_cast<SEventSubscriptionRef*>(luaL_checkudata(L, 1, MT));

    if (!ref->active || !ref->handler)
        return 0;

    ref->handler->unregisterEvent(ref->handle);
    ref->active = false;
    return 0;
}

static int eventSubscriptionIsActive(lua_State* L) {
    auto* ref = static_cast<SEventSubscriptionRef*>(luaL_checkudata(L, 1, MT));

    lua_pushboolean(L, ref->active);
    return 1;
}

static int eventSubscriptionIndex(lua_State* L) {
    luaL_checkudata(L, 1, MT);
    const std::string_view key = luaL_checkstring(L, 2);

    if (key == "remove")
        lua_pushcfunction(L, eventSubscriptionRemove);
    else if (key == "is_active")
        lua_pushcfunction(L, eventSubscriptionIsActive);
    else
        lua_pushnil(L);

    return 1;
}

void Objects::CLuaEventSubscription::setup(lua_State* L) {
    registerMetatable(L, MT, eventSubscriptionIndex, gcRef<SEventSubscriptionRef>);
}

void Objects::CLuaEventSubscription::push(lua_State* L, CLuaEventHandler* handler, uint64_t handle) {
    new (lua_newuserdata(L, sizeof(SEventSubscriptionRef))) SEventSubscriptionRef{.handler = handler, .handle = handle, .active = true};
    luaL_getmetatable(L, MT);
    lua_setmetatable(L, -2);
}
