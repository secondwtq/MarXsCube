#ifndef CUBE_E_DEBUG_H
#define CUBE_E_DEBUG_H

#include <fstream>
#include <iostream>

namespace Debug {
	extern std::ofstream logstream;

	void createLogFile(const char *filename);
	void closeLogFile();
}

#define _LOGFUNC(X1, X2, X3)  (Debug::logstream << (X1) << " " << (X2) << " " << (X3) << std::endl)

//  TODO: Modified in MarXsCube
//#define LOGFUNC _LOGFUNC(__FILE__, __LINE__, __func__)

#define LOGFUNC

#endif