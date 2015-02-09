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

//------------------------------------------------------------------------------
/**
 Lightweight reference to a Lua object.
 
 The reference is maintained for the lifetime of the C++ object.
 */
class CallbackRef
{
private:
	class Proxy;
	friend struct Stack <Proxy>;
	
	//----------------------------------------------------------------------------
	/**
	 Pop the Lua stack.
	 
	 Pops the specified number of stack items on destruction. We use this
	 when returning objects, to avoid an explicit temporary variable, since
	 the destructor executes after the return statement. For example:
	 
	 template <class U>
	 U cast (lua_State* L)
	 {
	 StackPop p (L, 1);
	 ...
	 return U (); // dtor called after this line
	 }
	 
	 @note The `StackPop` object must always be a named local variable.
  */
	class StackPop
	{
	public:
		/** Create a StackPop object.
		 
		 @param count The number of stack entries to pop on destruction.
		 */
		StackPop (lua_State* L, int count)
		: m_L (L)
		, m_count (count)
		{
		}
		
		~StackPop ()
		{
			lua_pop (m_L, m_count);
		}
		
	private:
		lua_State* m_L;
		int m_count;
	};
	
	//----------------------------------------------------------------------------
	/**
	 A proxy for representing table values.
  */
	class Proxy
	{
	private:
		lua_State* m_L;
		int m_tableRef;
		int m_keyRef;
		
	public:
		//--------------------------------------------------------------------------
		/**
		 Construct a Proxy from a table value.
		 
		 The table is in the registry, and the key is at the top of the stack.
		 The key is popped off the stack.
		 */
		Proxy (lua_State* L, int tableRef)
		: m_L (L)
		, m_tableRef (tableRef)
		, m_keyRef (luaL_ref (L, LUA_REGISTRYINDEX))
		{
		}
		
		//--------------------------------------------------------------------------
		/**
		 Create a Proxy via copy constructor.
		 
		 It is best to avoid code paths that invoke this, because it creates
		 an extra temporary Lua reference. Typically this is done by passing
		 the Proxy parameter as a `const` reference.
		 */
		Proxy (Proxy const& other)
		: m_L (other.m_L)
		, m_tableRef (other.m_tableRef)
		{
			// If this assert goes off it means code is taking this path,
			// which is better avoided.
			//
			assert (0);
			
			lua_rawgeti (m_L, LUA_REGISTRYINDEX, other.m_keyRef);
			m_keyRef = luaL_ref (m_L, LUA_REGISTRYINDEX);
		}
		
		//--------------------------------------------------------------------------
		/**
		 Destroy the proxy.
		 
		 This does not destroy the table value.
		 */
		~Proxy ()
		{
			luaL_unref (m_L, LUA_REGISTRYINDEX, m_keyRef);
		}
		
		//--------------------------------------------------------------------------
		/**
		 Return a reference to the table value.
		 */
		int createRef () const
		{
			push (m_L);
			return luaL_ref (m_L, LUA_REGISTRYINDEX);
		}
		
		//--------------------------------------------------------------------------
		/**
		 Assign a new value to this table key.
		 
		 This may invoke metamethods.
		 */
		template <class T>
		Proxy& operator= (T v)
		{
			StackPop p (m_L, 1);
			lua_rawgeti (m_L, LUA_REGISTRYINDEX, m_tableRef);
			lua_rawgeti (m_L, LUA_REGISTRYINDEX, m_keyRef);
			Stack <T>::push (m_L, v);
			lua_rawset (m_L, -3);
			return *this;
		}
		
		//--------------------------------------------------------------------------
		/**
		 Assign a new value to this table key.
		 
		 The assignment is raw, no metamethods are invoked.
		 */
		template <class T>
		Proxy& rawset (T v)
		{
			StackPop p (m_L, 1);
			lua_rawgeti (m_L, LUA_REGISTRYINDEX, m_tableRef);
			lua_rawgeti (m_L, LUA_REGISTRYINDEX, m_keyRef);
			Stack <T>::push (m_L, v);
			lua_settable (m_L, -3);
			return *this;
		}
		
		//==========================================================================
		//
		// This group of member functions mirrors the member functions in CallbackRef.
		
		/** Retrieve the lua_State associated with the table value.
		 */
		lua_State* state () const
		{
			return m_L;
		}
		
		//--------------------------------------------------------------------------
		/**
		 Push the value onto the Lua stack.
		 */
		void push (lua_State* L) const
		{
			assert (equalstates (L, m_L));
			lua_rawgeti (L, LUA_REGISTRYINDEX, m_tableRef);
			lua_rawgeti (L, LUA_REGISTRYINDEX, m_keyRef);
			lua_gettable (L, -2);
			lua_remove (L, -2); // remove the table
		}
		
		//--------------------------------------------------------------------------
		/**
		 Determine the object type.
		 
		 The return values are the same as for `lua_type`.
		 */
		int type () const
		{
			int result;
			push (m_L);
			result = lua_type (m_L, -1);
			lua_pop (m_L, 1);
			return result;
		}
		
		inline bool isNil () const { return type () == LUA_TNIL; }
		inline bool isNumber () const { return type () == LUA_TNUMBER; }
		inline bool isString () const { return type () == LUA_TSTRING; }
		inline bool isTable () const { return type () == LUA_TTABLE; }
		inline bool isFunction () const { return type () == LUA_TFUNCTION; }
		inline bool isUserdata () const { return type () == LUA_TUSERDATA; }
		inline bool isThread () const { return type () == LUA_TTHREAD; }
		inline bool isLightUserdata () const { return type () == LUA_TLIGHTUSERDATA; }
		
		//--------------------------------------------------------------------------
		/**
		 Perform an explicit conversion.
		 */
		template <class T>
		T cast () const
		{
			StackPop p (m_L, 1);
			push (m_L);
			
			// lua_gettop is used because Userdata::getClass() doesn't handle
			// negative stack indexes.
			//
			return Stack <T>::get (m_L, lua_gettop (m_L));
		}
		
		//--------------------------------------------------------------------------
		/**
		 Universal implicit conversion operator.
		 
		 NOTE: Visual Studio 2010 and 2012 have a bug where this function
		 is not used. See:
		 
		 http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/e30b2664-a92d-445c-9db2-e8e0fbde2014
		 https://connect.microsoft.com/VisualStudio/feedback/details/771509/correct-code-doesnt-compile
		 
		 // This code snippet fails to compile in vs2010,vs2012
		 struct S {
		 template <class T> inline operator T () const { return T (); }
		 };
		 int main () {
		 S () || false;
		 return 0;
		 }
		 */
		template <class T>
		inline operator T () const
		{
			return cast <T> ();
		}
		
		//--------------------------------------------------------------------------
		/**
		 Universal comparison operators.
		 */
		/** @{ */
		template <class T>
		bool operator== (T rhs) const
		{
			StackPop p (m_L, 2);
			push (m_L);
			Stack <T>::push (m_L, rhs);
			return lua_compare (m_L, -2, -1, LUA_OPEQ) == 1;
		}
		
		template <class T>
		bool operator< (T rhs) const
		{
			StackPop p (m_L, 2);
			push (m_L);
			Stack <T>::push (m_L, rhs);
			return lua_compare (m_L, -2, -1, LUA_OPLT) == 1;
		}
		
		template <class T>
		bool operator<= (T rhs) const
		{
			StackPop p (m_L, 2);
			push (m_L);
			Stack <T>::push (m_L, rhs);
			return lua_compare (m_L, -2, -1, LUA_OPLE) == 1;
		}
		
		template <class T>
		bool operator> (T rhs) const
		{
			StackPop p (m_L, 2);
			push (m_L);
			Stack <T>::push (m_L, rhs);
			return lua_compare (m_L, -1, -2, LUA_OPLT) == 1;
		}
		
		template <class T>
		bool operator>= (T rhs) const
		{
			StackPop p (m_L, 2);
			push (m_L);
			Stack <T>::push (m_L, rhs);
			return lua_compare (m_L, -1, -2, LUA_OPLE) == 1;
		}
		
		template <class T>
		bool rawequal (T rhs) const
		{
			StackPop p (m_L, 2);
			push (m_L);
			Stack <T>::push (m_L, rhs);
			return lua_rawequal (m_L, -1, -2) == 1;
		}
		/** @} */
		
		//--------------------------------------------------------------------------
		/**
		 Access a table value using a key.
		 
		 This invokes metamethods.
		 */
		template <class T>
		Proxy operator[] (T key) const
		{
			return CallbackRef (*this) [key];
		}
		
		//--------------------------------------------------------------------------
		/**
		 Access a table value using a key.
		 
		 The operation is raw, metamethods are not invoked. The result is
		 passed by value and may not be modified.
		 */
		template <class T>
		CallbackRef rawget (T key) const
		{
			StackPop (m_L, 1);
			push (m_L);
			Stack <T>::push (m_L, key);
			lua_rawget (m_L, -2);
			return CallbackRef (m_L, FromStack ());
		}
		
		//--------------------------------------------------------------------------
		/**
		 Append a value to the table.
		 
		 If the table is a sequence this will add another element to it.
		 */
		template <class T>
		void append (T v) const
		{
			push (m_L);
			Stack <T>::push (m_L, v);
			luaL_ref (m_L, -2);
			lua_pop (m_L, 1);
		}
		
		//--------------------------------------------------------------------------
		/**
		 Call the length operator.
		 
		 This is identical to applying the Lua # operator.
		 */
		int length () const
		{
			StackPop p (m_L, 1);
			push (m_L);
			return get_length (m_L, -1);
		}
		
		//--------------------------------------------------------------------------
		/**
		 Call Lua code.
		 
		 These overloads allow Lua code to be called with up to 8 parameters.
		 The return value is provided as a CallbackRef (which may be LUA_REFNIL).
		 If an error occurs, a LuaException is thrown.
		 */
		/** @{ */
		CallbackRef const operator() () const
		{
			push (m_L);
			LuaException::pcall (m_L, 0, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1>
		CallbackRef const operator() (P1 p1) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			LuaException::pcall (m_L, 1, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1, class P2>
		CallbackRef const operator() (P1 p1, P2 p2) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			Stack <P2>::push (m_L, p2);
			LuaException::pcall (m_L, 2, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1, class P2, class P3>
		CallbackRef const operator() (P1 p1, P2 p2, P3 p3) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			Stack <P2>::push (m_L, p2);
			Stack <P3>::push (m_L, p3);
			LuaException::pcall (m_L, 3, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1, class P2, class P3, class P4>
		CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			Stack <P2>::push (m_L, p2);
			Stack <P3>::push (m_L, p3);
			Stack <P4>::push (m_L, p4);
			LuaException::pcall (m_L, 4, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1, class P2, class P3, class P4, class P5>
		CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			Stack <P2>::push (m_L, p2);
			Stack <P3>::push (m_L, p3);
			Stack <P4>::push (m_L, p4);
			Stack <P5>::push (m_L, p5);
			LuaException::pcall (m_L, 5, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1, class P2, class P3, class P4, class P5, class P6>
		CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			Stack <P2>::push (m_L, p2);
			Stack <P3>::push (m_L, p3);
			Stack <P4>::push (m_L, p4);
			Stack <P5>::push (m_L, p5);
			Stack <P6>::push (m_L, p6);
			LuaException::pcall (m_L, 6, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
		CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			Stack <P2>::push (m_L, p2);
			Stack <P3>::push (m_L, p3);
			Stack <P4>::push (m_L, p4);
			Stack <P5>::push (m_L, p5);
			Stack <P6>::push (m_L, p6);
			Stack <P7>::push (m_L, p7);
			LuaException::pcall (m_L, 7, 1);
			return CallbackRef (m_L, FromStack ());
		}
		
		template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
		CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const
		{
			push (m_L);
			Stack <P1>::push (m_L, p1);
			Stack <P2>::push (m_L, p2);
			Stack <P3>::push (m_L, p3);
			Stack <P4>::push (m_L, p4);
			Stack <P5>::push (m_L, p5);
			Stack <P6>::push (m_L, p6);
			Stack <P7>::push (m_L, p7);
			Stack <P8>::push (m_L, p8);
			LuaException::pcall (m_L, 8, 1);
			return CallbackRef (m_L, FromStack ());
		}
		/** @} */
		
		//==========================================================================
	};
	
private:
	friend struct Stack <CallbackRef>;
	
	//----------------------------------------------------------------------------
	/**
	 Type tag for stack construction.
  */
	struct FromStack { };
	
	//----------------------------------------------------------------------------
	/**
	 Create a reference to an object at the top of the Lua stack and pop it.
	 
	 This constructor is private and not invoked directly.
	 Instead, use the `fromStack` function.
	 
	 @note The object is popped.
  */
	CallbackRef (lua_State* L, FromStack)
	: m_L (L)
	{
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Create a reference to an object on the Lua stack.
	 
	 This constructor is private and not invoked directly.
	 Instead, use the `fromStack` function.
	 
	 @note The object is not popped.
  */
	CallbackRef (lua_State* L, int index, FromStack)
	: m_L (L)
	{
		lua_pushvalue (m_L, index);
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
	}
	
	//----------------------------------------------------------------------------
	
	// This type of construction is disallowed, since we don't have a `lua_State`.
	//
	template <class T>
	CallbackRef (T)
	{
	}
	
	//----------------------------------------------------------------------------
	/**
	 Create a reference to this ref.
	 
	 This is used internally.
  */
	int createRef () const
	{
		if (m_ref != LUA_REFNIL)
		{
			push (m_L);
			return luaL_ref (m_L, LUA_REGISTRYINDEX);
		}
		else
		{
			return LUA_REFNIL;
		}
	}
	
public:
	
	//	CallbackRef () __attribute__((deprecated))
	//	: m_L (nullptr)
	//	, m_ref (LUA_REFNIL)
	//	{
	//	}
	
	//----------------------------------------------------------------------------
	/**
	 Create a nil reference.
	 
	 The CallbackRef may be assigned later.
  */
	CallbackRef (lua_State* L)
	: m_L (L)
	, m_ref (LUA_REFNIL)
	{
	}
	
	//----------------------------------------------------------------------------
	/**
	 Create a reference to a value.
  */
	template <class T>
	CallbackRef (lua_State* L, T v)
	: m_L (L)
	{
		Stack <T>::push (m_L, v);
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Create a reference to a table value.
  */
	CallbackRef (Proxy const& v)
	: m_L (v.state ())
	, m_ref (v.createRef ())
	{
	}
	
	//----------------------------------------------------------------------------
	/**
	 Create a new reference to an existing reference.
  */
	CallbackRef (CallbackRef const& other)
	: m_L (other.m_L)
	, m_ref (other.m_ref)
	{
	}
	
	//----------------------------------------------------------------------------
	/**
	 Destroy a reference.
	 
	 The corresponding Lua registry reference will be released.
	 
	 @note If the state refers to a thread, it is the responsibility of the
	 caller to ensure that the thread still exists when the CallbackRef
	 is destroyed.
  */
	~CallbackRef ()
	{
		
	}
	
	//----------------------------------------------------------------------------
	/**
	 Return a CallbackRef from a stack item.
	 
	 The stack item is not popped.
  */
	static CallbackRef fromStack (lua_State* L, int index)
	{
		lua_pushvalue (L, index);
		return CallbackRef (L, FromStack ());
	}
	
	//----------------------------------------------------------------------------
	/**
	 Create a new empty table and return a reference to it.
	 
	 It is also possible to use the free function `newTable`.
	 
	 @see ::getGlobal
  */
	static CallbackRef newTable (lua_State* L)
	{
		lua_newtable (L);
		return CallbackRef (L, FromStack ());
	}
	
	//----------------------------------------------------------------------------
	/**
	 Return a reference to a named global.
	 
	 It is also possible to use the free function `getGlobal`.
	 
	 @see ::getGlobal
  */
	static CallbackRef getGlobal (lua_State *L, char const* name)
	{
		lua_getglobal (L, name);
		return CallbackRef (L, FromStack ());
	}
	
	//----------------------------------------------------------------------------
	/**
	 Assign a different value to this CallbackRef.
  */
	template <class T>
	CallbackRef& operator= (T rhs)
	{
		luaL_unref (m_L, LUA_REGISTRYINDEX, m_ref);
		Stack <T>::push (m_L, rhs);
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
		return *this;
	}
	
	//----------------------------------------------------------------------------
	/**
	 Assign another CallbackRef to this CallbackRef.
  */
	CallbackRef& operator= (CallbackRef const& rhs)
	{
		luaL_unref (m_L, LUA_REGISTRYINDEX, m_ref);
		rhs.push (m_L);
		m_L = rhs.state ();
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
		return *this;
	}
	
	void merge(CallbackRef const& rhs) {
		// luaL_unref (m_L, LUA_REGISTRYINDEX, m_ref);
		rhs.push (m_L);
		m_L = rhs.m_L;
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
	}
	
	//----------------------------------------------------------------------------
	/**
	 converts to a string using luas tostring function
  */
	std::string tostring() const
	{
		lua_getglobal (m_L, "tostring");
		push (m_L);
		lua_call (m_L, 1, 1);
		const char* str = lua_tostring(m_L, 1);
		lua_pop(m_L, 1);
		return std::string(str);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Print a text description of the value to a stream.
	 
	 This is used for diagnostics.
  */
	void print (std::ostream& os) const
	{
		switch (type ())
		{
			case LUA_TNIL:
				os << "nil";
				break;
				
			case LUA_TNUMBER:
				os << cast <lua_Number> ();
				break;
				
			case LUA_TBOOLEAN:
				os << (cast <bool> () ? "true" : "false");
				break;
				
			case LUA_TSTRING:
				os << '"' << cast <std::string> () << '"';
				break;
				
			case LUA_TTABLE:
				os << "table: " << tostring();
				break;
				
			case LUA_TFUNCTION:
				os << "function: " << tostring();
				break;
				
			case LUA_TUSERDATA:
				os << "userdata: " << tostring();
				break;
				
			case LUA_TTHREAD:
				os << "thread: " << tostring();
				break;
				
			case LUA_TLIGHTUSERDATA:
				os << "lightuserdata: " << tostring();
				break;
				
			default:
				os << "unknown";
				break;
		}
	}
	
	//============================================================================
	//
	// This group of member functions is mirrored in Proxy
	//
	
	/** Retrieve the lua_State associated with the reference.
  */
	lua_State* state () const
	{
		return m_L;
	}
	
	//----------------------------------------------------------------------------
	/**
	 Place the object onto the Lua stack.
  */
	void push (lua_State* L) const
	{
		assert (equalstates (L, m_L));
		lua_rawgeti (L, LUA_REGISTRYINDEX, m_ref);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Pop the top of Lua stack and assign the ref to m_ref
  */
	void pop (lua_State* L)
	{
		assert (equalstates (L, m_L));
		luaL_unref (m_L, LUA_REGISTRYINDEX, m_ref);
		m_ref = luaL_ref (m_L, LUA_REGISTRYINDEX);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Determine the object type.
	 
	 The return values are the same as for `lua_type`.
  */
	/** @{ */
	int type () const
	{
		int result;
		if (m_ref != LUA_REFNIL)
		{
			push (m_L);
			result = lua_type (m_L, -1);
			lua_pop (m_L, 1);
		}
		else
		{
			result = LUA_TNIL;
		}
		
		return result;
	}
	
	// should never happen
	//inline bool isNone () const { return m_ref == LUA_NOREF; }
	
	inline bool isNil () const { return type () == LUA_TNIL; }
	inline bool isNumber () const { return type () == LUA_TNUMBER; }
	inline bool isString () const { return type () == LUA_TSTRING; }
	inline bool isTable () const { return type () == LUA_TTABLE; }
	inline bool isFunction () const { return type () == LUA_TFUNCTION; }
	inline bool isUserdata () const { return type () == LUA_TUSERDATA; }
	inline bool isThread () const { return type () == LUA_TTHREAD; }
	inline bool isLightUserdata () const { return type () == LUA_TLIGHTUSERDATA; }
	/** @} */
	
	//----------------------------------------------------------------------------
	/**
	 Perform an explicit conversion.
  */
	template <class T>
	T cast () const
	{
		StackPop p (m_L, 1);
		push (m_L);
		
		// lua_gettop is used because Userdata::getClass() doesn't handle
		// negative stack indexes.
		//
		return Stack <T>::get (m_L, lua_gettop (m_L));
	}
	
	//----------------------------------------------------------------------------
	/**
	 Universal implicit conversion operator.
	 
	 NOTE: Visual Studio 2010 and 2012 have a bug where this function
	 is not used. See:
	 
	 http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/e30b2664-a92d-445c-9db2-e8e0fbde2014
	 https://connect.microsoft.com/VisualStudio/feedback/details/771509/correct-code-doesnt-compile
	 
	 // This code snippet fails to compile in vs2010,vs2012
	 struct S {
	 template <class T> inline operator T () const { return T (); }
	 };
	 int main () {
	 S () || false;
	 return 0;
	 }
  */
	template <class T>
	inline operator T () const
	{
		return cast <T> ();
	}
	
	//----------------------------------------------------------------------------
	/**
	 Universal comparison operators.
  */
	/** @{ */
	template <class T>
	bool operator== (T rhs) const
	{
		StackPop p (m_L, 2);
		push (m_L);
		Stack <T>::push (m_L, rhs);
		return lua_compare (m_L, -2, -1, LUA_OPEQ) == 1;
	}
	
	template <class T>
	bool operator< (T rhs) const
	{
		StackPop p (m_L, 2);
		push (m_L);
		Stack <T>::push (m_L, rhs);
		return lua_compare (m_L, -2, -1, LUA_OPLT) == 1;
	}
	
	template <class T>
	bool operator<= (T rhs) const
	{
		StackPop p (m_L, 2);
		push (m_L);
		Stack <T>::push (m_L, rhs);
		return lua_compare (m_L, -2, -1, LUA_OPLE) == 1;
	}
	
	template <class T>
	bool operator> (T rhs) const
	{
		StackPop p (m_L, 2);
		push (m_L);
		Stack <T>::push (m_L, rhs);
		return lua_compare (m_L, -1, -2, LUA_OPLT) == 1;
	}
	
	template <class T>
	bool operator>= (T rhs) const
	{
		StackPop p (m_L, 2);
		push (m_L);
		Stack <T>::push (m_L, rhs);
		return lua_compare (m_L, -1, -2, LUA_OPLE) == 1;
	}
	
	template <class T>
	bool rawequal (T rhs) const
	{
		StackPop p (m_L, 2);
		push (m_L);
		Stack <T>::push (m_L, rhs);
		return lua_rawequal (m_L, -1, -2) == 1;
	}
	/** @} */
	
	//----------------------------------------------------------------------------
	/**
	 Append a value to the table.
	 
	 If the table is a sequence this will add another element to it.
  */
	template <class T>
	void append (T v) const
	{
		push (m_L);
		Stack <T>::push (m_L, v);
		luaL_ref (m_L, -2);
		lua_pop (m_L, 1);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Call the length operator.
	 
	 This is identical to applying the Lua # operator.
  */
	int length () const
	{
		StackPop p (m_L, 1);
		push (m_L);
		return get_length (m_L, -1);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Access a table value using a key.
	 
	 This invokes metamethods.
  */
	template <class T>
	Proxy operator[] (T key) const
	{
		Stack <T>::push (m_L, key);
		return Proxy (m_L, m_ref);
	}
	
	//----------------------------------------------------------------------------
	/**
	 Call Lua code.
	 
	 These overloads allow Lua code to be called with up to 8 parameters.
	 The return value is provided as a CallbackRef (which may be LUA_REFNIL).
	 If an error occurs, a LuaException is thrown.
  */
	/** @{ */
	CallbackRef const operator() () const
	{
		push (m_L);
		LuaException::pcall (m_L, 0, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1>
	CallbackRef const operator() (P1 p1) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		LuaException::pcall (m_L, 1, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2>
	CallbackRef const operator() (P1 p1, P2 p2) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		LuaException::pcall (m_L, 2, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		LuaException::pcall (m_L, 3, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3, class P4>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		Stack <P4>::push (m_L, p4);
		LuaException::pcall (m_L, 4, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3, class P4, class P5>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		Stack <P4>::push (m_L, p4);
		Stack <P5>::push (m_L, p5);
		LuaException::pcall (m_L, 5, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3, class P4, class P5, class P6>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		Stack <P4>::push (m_L, p4);
		Stack <P5>::push (m_L, p5);
		Stack <P6>::push (m_L, p6);
		LuaException::pcall (m_L, 6, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		Stack <P4>::push (m_L, p4);
		Stack <P5>::push (m_L, p5);
		Stack <P6>::push (m_L, p6);
		Stack <P7>::push (m_L, p7);
		LuaException::pcall (m_L, 7, 1);
		return CallbackRef (m_L, FromStack ());
	}
	
	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	CallbackRef const operator() (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const
	{
		push (m_L);
		Stack <P1>::push (m_L, p1);
		Stack <P2>::push (m_L, p2);
		Stack <P3>::push (m_L, p3);
		Stack <P4>::push (m_L, p4);
		Stack <P5>::push (m_L, p5);
		Stack <P6>::push (m_L, p6);
		Stack <P7>::push (m_L, p7);
		Stack <P8>::push (m_L, p8);
		LuaException::pcall (m_L, 8, 1);
		return CallbackRef (m_L, FromStack ());
	}
	/** @} */
	
	//============================================================================
	
private:
	lua_State* m_L;
	int m_ref;
};

//------------------------------------------------------------------------------
/**
 Stack specialization for CallbackRef.
 */
template <>
struct Stack <CallbackRef>
{
public:
	// The value is const& to prevent a copy construction.
	//
	static inline void push (lua_State* L, CallbackRef const& v)
	{
		v.push (L);
	}
	
	static inline CallbackRef get (lua_State* L, int index)
	{
		return CallbackRef (L, index, CallbackRef::FromStack ());
	}
};

//------------------------------------------------------------------------------
/**
 Stack specialization for Proxy.
 */
template <>
struct Stack <CallbackRef::Proxy>
{
public:
	// The value is const& to prevent a copy construction.
	//
	static inline void push (lua_State* L, CallbackRef::Proxy const& v)
	{
		v.push (L);
	}
};

}

#endif
