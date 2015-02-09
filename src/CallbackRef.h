//
//  CallbackRef.h
//  MarXsCube
//
//  Created by SeconDatke on 2/10/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_CallbackRef_h
#define MarXsCube_CallbackRef_h

namespace luabridge {

class CallbackRef {

private:
	friend struct Stack <CallbackRef>;
	
	struct FromStack { };

	CallbackRef (lua_State* L, FromStack)
	: m_L (L) {
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX); }

	CallbackRef (lua_State* L, int index, FromStack)
	: m_L (L) {
		lua_pushvalue (m_L, index);
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX); }

	template <class T>
	CallbackRef (T) { }
	
	CallbackRef () { }
	
public:

	CallbackRef (CallbackRef const& other)
	: m_L (other.m_L), m_ref (other.m_ref) { }

	~CallbackRef () { }

	void push (lua_State* L) const {
		assert (equalstates (L, m_L));
		lua_rawgeti (L, LUA_REGISTRYINDEX, m_ref);
	}

	void pop (lua_State* L) {
		assert (equalstates (L, m_L));
		luaL_unref (m_L, LUA_REGISTRYINDEX, m_ref);
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
	}
	
	void de_ref() const {
		lua_unref(m_L, m_ref); }
	
	//----------------------------------------------------------------------------
	/**
	 Call Lua code.
	 
	 These overloads allow Lua code to be called with up to 8 parameters.
	 The return value is provided as a CallbackRef (which may be LUA_REFNIL).
	 If an error occurs, a LuaException is thrown.
  */
	CallbackRef const operator() () const {
		push (m_L);
		LuaException::pcall (m_L, 0, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1>
	CallbackRef const operator() (P1 p1) const {
		push (m_L);
		Stack <P1>::push (m_L, p1);
		LuaException::pcall (m_L, 1, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2>
	CallbackRef const operator() (P1 p1, P2 p2) const {
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		LuaException::pcall (m_L, 2, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3) const {
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		LuaException::pcall (m_L, 3, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3, class P4>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4) const {
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		Stack <P4>::push (m_L, p4);
		LuaException::pcall (m_L, 4, 1);
		return CallbackRef (m_L, FromStack ());
	}

private:
	lua_State* m_L;
	int m_ref;
};

template <>
struct Stack <CallbackRef> {
public:
	static inline void push (lua_State* L, CallbackRef const& v) {
		v.push (L); }
	
	static inline CallbackRef get (lua_State* L, int index) {
		return CallbackRef (L, index, CallbackRef::FromStack ()); }
};

}

#endif
