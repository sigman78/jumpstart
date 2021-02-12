//
//

#include "debug.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>

/* For isatty() on Unix-like systems */
#ifdef CORRADE_TARGET_UNIX
#include <unistd.h>

/* Node.js alternative to isatty() on Emscripten */
#elif defined(CORRADE_TARGET_EMSCRIPTEN)
#include <emscripten.h>

#elif defined(CORRADE_TARGET_WINDOWS)
#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN

/* For isatty() on Windows */
#ifdef CORRADE_UTILITY_USE_ANSI_COLORS
#include <io.h>

/* WINAPI-based colored output on Windows */
#else
#include <windows.h>
#include <wincon.h>
#endif
#endif

namespace Platform {

namespace {

struct DebugGlobals {
    std::ostream *output, *warningOutput, *errorOutput;
};

PLATFORM_THREAD_LOCAL
DebugGlobals debugGlobals{
	&std::cout, &std::cerr, &std::cerr
};

}

////

std::ostream* Debug::defaultOutput() { return &std::cout; }
std::ostream* Warning::defaultOutput() { return &std::cerr; }
//std::ostream* Error::defaultOutput() { return &std::cerr; }

std::ostream* Debug::output() { return debugGlobals.output; }
std::ostream* Warning::output() { return debugGlobals.warningOutput; }
//std::ostream* Error::output() { return debugGlobals.errorOutput; }


#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
Impl::SourceLocation(Debug&& debug, const char* file, int line): debug(&debug) {
	debug._sourceLocationFile = file;
	debug._sourceLocationLine = line;
}
#endif

Debug::Debug(std::ostream* output) {
	_previousGlobalOutput = debugGlobals.output;
	debugGlobals.output = _output = output;	
}

Debug::Debug(): Debug(debugGlobals.output) {}

Warning::Warning(std::ostream* output) {
	_previousGlobalOutput = debugGlobals.warningOutput;
	debugGlobals.warningOutput = _output = output;
}

Warning::Warning(): Warning(debugGlobals.warningOutput) {}

void Debug::cleanup() {
	#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
	if(_output && _sourceLocationFile) {
		*_output << _sourceLocationFile << ":" << _sourceLocationLine;
	}
	#endif

	if(_output) {
		*_output << std::endl;
	}

	debugGlobals.output = _previousGlobalOutput;
}

Debug::~Debug() {
	cleanup();
}

namespace {
	template<class T> inline void serialize(std::ostream& s, const T& value) {
		s << value;
	}
}

template<typename T>
Debug& Debug::print(const T& val) {
	if (!_output) return *this;

	#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
	if(_sourceLocationFile) {
		*_output << _sourceLocationFile << ":" << _sourceLocationLine;
		_sourceLocationFile = nullptr;
	}
	#endif

	if (!_nosp) {
		*_output << ' ';
		_nosp = false;
	}

	serialize(*_output, val);

	return *this;
}

Debug& Debug::operator<<(const char* str) {
	return print(str);
}

Debug& Debug::operator<<(const void* ptr) {
    std::ostringstream o;
    o << "0x" << std::hex << reinterpret_cast<std::uintptr_t>(value);
    return print(o.str());
}

Debug& Debug::operator<<(bool val) {
	return print(val ? "true" : "false");
}

Debug& Debug::operator<<(char val) {
	return print(int(val));
}

Debug& Debug::operator<<(unsigned char val) {
	return print(int(val));
}

Debug& Debug::operator<<(int val) {
	return print(val);
}

Debug& Debug::operator<<(long val) {
	return print(val);
}

Debug& Debug::operator<<(long long val) {
	return print(val);
}

Debug& Debug::operator<<(unsigned int val) {
	return print(val);
}

Debug& Debug::operator<<(unsigned long val) {
	return print(val);
}

Debug& Debug::operator<<(unsigned long long val) {
	return print(val);
}

Debug& Debug::operator<<(float val) {
	if (!_output) {
		return *this;
	}
	*_output << std::setprecision(6);
	return print(val);
}

Debug& Debug::operator<<(double val) {
	if (!_output) {
		return *this;
	}
	*_output << std::setprecision(15);
	return print(val);
}

Debug& Debug::operator<<(std::nullptr_t) {
	return print("null");
}

//Debug& operator<<(Debug& debug, Implementation::DebugOstreamFallback&& value) {
//    return debug.print(value);
//}


} // Platform