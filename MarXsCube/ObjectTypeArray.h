#ifndef CUBE_E_TYPEARRAY_H
#define CUBE_E_TYPEARRAY_H

#include <unordered_map>
#include <string>

class ConfigManger;

template <typename T>
class ObjectTypeArray {
	public:
		typedef std::unordered_map <std::string, T *> ArrayType;
		ArrayType Array;

		T *Find(const char *ID);

		T *Find(std::string &ID);

		T *Allocate(const char *ID);
		T *Allocate(std::string &ID);

		T *FindOrAllocate(const char *ID);

		T *FindOrLoad(const char *ID, ConfigManger &manger);

		T *FindOrLoad(std::string &ID, ConfigManger &manger);

		void Push(const char *ID, T *src);

		void Push(const std::string &ID, T *src);
};

#include "Config.h"

#endif