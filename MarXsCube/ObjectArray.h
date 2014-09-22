#ifndef CUBE_E_ARRAY_H
#define CUBE_E_ARRAY_H

#include <vector>
#include <algorithm>

template <typename T>
class ObjectArray {
	public:
		std::vector<T *> Array;
		operator std::vector<T *>() {LOGFUNC; return Array; }
};

template <typename T>
inline bool addObject_noDup(T src, std::vector<T> &vec) {LOGFUNC;
	bool ret = true;
	for (auto i : vec)
		if (i == src) { ret = false; break; }
	if (ret) vec.push_back(src);
	return ret;
}

template <typename T>
inline void delObject_all(T src, std::vector<T> &vec) {LOGFUNC;
	vec.erase(remove(vec.begin(), vec.end(), src), vec.end());
}

#endif