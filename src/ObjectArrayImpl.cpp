#include "Common.h"
#include "ObjectTypeArray.h"
#include "Type_Abstract.h"
#include "Type_Object.h"
#include "Type_Anim.h"
#include "Type_Techno.h"
#include "Physics.h"

using namespace std;


template <typename T>
T *ObjectTypeArray<T>::Find(const char *ID) {LOGFUNC;
	auto i = Array.find(ID);
	if (i == Array.end()) return nullptr;
	else return (*i).second;
}

template <typename T>
T *ObjectTypeArray<T>::Find(string &ID) {LOGFUNC;
	auto i = Array.find(ID);
	if (i == Array.end()) return nullptr;
	else return (*i).second;
}

template <typename T>
void ObjectTypeArray<T>::Push(const char *ID, T *src) {LOGFUNC;
	Array.insert(make_pair(ID, src));
}

template <typename T>
void ObjectTypeArray<T>::Push(const string &ID, T *src) {LOGFUNC;
	Array.insert(make_pair(ID, src));
}

template <typename T>
T *ObjectTypeArray<T>::Allocate(const char *ID) {LOGFUNC;
	auto ret = new T(ID);
	Array.insert(make_pair(ID, ret));
	return ret;
}

template <typename T>
T *ObjectTypeArray<T>::Allocate(string &ID) {LOGFUNC;
	auto ret = new T(ID.c_str());
	Array.insert(make_pair(ID, ret));
	return ret;
}

template <typename T>
T *ObjectTypeArray<T>::FindOrAllocate(const char *ID) {LOGFUNC;
	if (auto ret = Find(ID)) return ret;
	else return Allocate(ID);
}

template <typename T>
T *ObjectTypeArray<T>::FindOrLoad(const char *ID, ConfigManger &manger) {LOGFUNC;
	if (auto ret = Find(ID)) return ret;
	else {
		ret = Allocate(ID);
		ret->LoadFromConfig(manger);
		return ret;
	}
}

template <typename T>
T *ObjectTypeArray<T>::FindOrLoad(string &ID, ConfigManger &manger) {LOGFUNC;
	if (auto ret = Find(ID)) return ret;
	else {
		ret = Allocate(ID);
		ret->LoadFromConfig(manger);
		return ret;
	}
}

template class ObjectTypeArray<Type_Abstract>;
template class ObjectTypeArray<Type_Object>;
template class ObjectTypeArray<Type_Anim>;
template class ObjectTypeArray<Type_Techno>;

template class ObjectArray<PhysicsObject>;