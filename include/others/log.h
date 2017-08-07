


#ifndef _LOG_H_
#define _LOG_H_


#define LOG_FILE "ray_program.log"

#include "ogl/glutconsole.h"

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <assert.h>
#include <time.h>
#include <string>


class Debug {

private:
	


public:
	static GlutConsole * _glConsole;
	static bool console_log;
	static bool log_restart();

	static bool log(const char * message, ...);
	static bool log_string(const std::string & message);
	

};

#endif