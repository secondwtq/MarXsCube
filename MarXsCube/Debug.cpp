#include "Debug.h"

using namespace std;

namespace Debug {

	ofstream logstream;

	void createLogFile(const char *filename) {
		logstream.open(filename);
	}

	void closeLogFile() {
		logstream.close();
	}
}