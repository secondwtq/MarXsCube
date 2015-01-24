#include "Debug_old.h"

using namespace std;

namespace Debug_old {

	ofstream logstream;

	void createLogFile(const char *filename) {
		logstream.open(filename);
	}

	void closeLogFile() {
		logstream.close();
	}
}