
#include "others\log.h"

GlutConsole * Debug::_glConsole = NULL;

bool Debug::console_log = true;

bool Debug::log_restart() {

	remove(LOG_FILE);


	FILE * file;
	errno_t err;
	if ((err = fopen_s(&file, LOG_FILE, "a")) != 0) {

		fprintf(stderr, "ERROR: could not open LOG_FILE %s file for appending\n", LOG_FILE);
		return false;
	}



	time_t now = time(NULL);
	char date[64];
	if ((err = ctime_s(date, 64, &now)) != 0)
		fprintf(file, "RAY PROGRAM LOG.\n");
	else
		fprintf(file, "RAY PROGRAM LOG.\n%s\n", date);

	fclose(file);
	return true;
}

bool Debug::log(const char * message, ...) {

	va_list argptr;
	FILE * file;
	errno_t err;
	if ((err = fopen_s(&file, LOG_FILE, "a")) != 0) {

		fprintf(stderr, "ERROR: could not open LOG_FILE %s file for appending\n",
			LOG_FILE);
		return false;
	}
	//
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	//}

	if (console_log) {
		va_start(argptr, message);
		vprintf(message, argptr);
		va_end(argptr);
	}

	if (_glConsole) {
		char buffer[1024];
		va_start(argptr, message);
		vsprintf(buffer, message, argptr);

		va_end(argptr);
		_glConsole->appendLine(buffer);
	}

	fclose(file);
	return true;

}

bool Debug::log_string(const std::string & message) {

	va_list argptr;
	FILE * file;
	errno_t err;
	if ((err = fopen_s(&file, LOG_FILE, "a")) != 0) {

		fprintf(stderr, "ERROR: could not open LOG_FILE %s file for appending\n",
			LOG_FILE);
		return false;
	}

	fputs(message.c_str(), file);
	//
	/*va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	//}*/

	if (console_log) {
		printf(message.c_str());
	}

	if (_glConsole) {
		_glConsole->appendLine(message);
	}

	fclose(file);
	return true;

}