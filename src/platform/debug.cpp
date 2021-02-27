//
//

#include "debug.h"
#include "macros.h"

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>

#if defined(PLATFORM_TARGET_WINDOWS)
#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN
#include <windows.h>
#endif

namespace Platform {

namespace {

// #define PLATFORM_WINDOWED_APPLICATION

#if defined(PLATFORM_TARGET_WINDOWS) && defined(PLATFORM_WINDOWED_APPLICATION)
#define OUTPUT_TO_DEBUGGER

// Courtesy of: https://gist.github.com/comargo/f7fe244e539325cef0f66704b58dc35a
template <class CharT, class TraitsT = std::char_traits<CharT>>
class basic_debugbuf : public std::basic_stringbuf<CharT, TraitsT>
{
public:
    virtual ~basic_debugbuf()
    {
        sync();
    }

protected:
    int sync()
    {
        output_debug_string(str().c_str());
        str({});    // Clear the string buffer

        return 0;
    }

    void output_debug_string(const CharT *text) {}
};

template<>
void basic_debugbuf<char>::output_debug_string(const char *text)
{
    ::OutputDebugStringA(text);
}

template<>
void basic_debugbuf<wchar_t>::output_debug_string(const wchar_t *text)
{
    ::OutputDebugStringW(text);
}

template<class CharT, class TraitsT = std::char_traits<CharT> >
class basic_dostream : public std::basic_ostream<CharT, TraitsT>
{
public:

    basic_dostream() : std::basic_ostream<CharT, TraitsT>
                (new basic_debugbuf<CharT, TraitsT>()) {}
    ~basic_dostream() 
    {
        delete rdbuf(); 
    }
};

typedef basic_dostream<char>    dostream;
typedef basic_dostream<wchar_t> wdostream;

#endif

struct DebugGlobals {
    std::ostream *output, *warningOutput, *errorOutput;
};

#if defined(OUTPUT_TO_DEBUGGER)

dostream cdebug;

PLATFORM_THREAD_LOCAL
DebugGlobals debugGlobals{
    &cdebug, &cdebug, &cdebug
};

#else

PLATFORM_THREAD_LOCAL
DebugGlobals debugGlobals{
    &std::cout, &std::cerr, &std::cerr
};

#endif

}

////

std::ostream* Debug::systemOutput() { return &std::cout; }
std::ostream* Warning::systemOutput() { return &std::cerr; }
std::ostream* Error::systemOutput() { return &std::cerr; }

std::ostream* Debug::output() { return debugGlobals.output; }
std::ostream* Warning::output() { return debugGlobals.warningOutput; }
std::ostream* Error::output() { return debugGlobals.errorOutput; }


#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
Impl::SourceLocation::SourceLocation(Debug&& debug, const char* file, int line): debug(&debug) {
    debug._sourceLocationFile = file;
    debug._sourceLocationLine = line;
}
#endif

Debug::Debug(std::ostream* output) {
    _previousGlobalOutput = debugGlobals.output;
    debugGlobals.output = _output = output;	
}

Debug::Debug(): Debug(debugGlobals.output) {}

Warning::Warning(std::ostream* output): Debug{} {
    _previousGlobalWarningOutput = debugGlobals.warningOutput;
    debugGlobals.warningOutput = _output = output;
}

Warning::Warning(): Warning(debugGlobals.warningOutput) {}

Error::Error(std::ostream* output): Debug{} {
    _previousGlobalErrorOutput = debugGlobals.errorOutput;
    debugGlobals.errorOutput = _output = output;
}

Error::Error(): Error(debugGlobals.errorOutput) {}

void Debug::cleanup() {
    #ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
    if(_output && _sourceLocationFile) {
        *_output << _sourceLocationFile << " (" << _sourceLocationLine << ") :";
        _printed = true;
    }
    #endif

    if(_output && not _nonl && _printed) {
        *_output << std::endl;
    }

    debugGlobals.output = _previousGlobalOutput;
}

Debug::~Debug() {
    cleanup();
}

Warning::~Warning() {
    debugGlobals.warningOutput = _previousGlobalWarningOutput;
}

Error::~Error() {
    debugGlobals.errorOutput = _previousGlobalErrorOutput;
}

namespace {
    template<class T> inline void serialize(std::ostream& s, const T& value) {
        s << value;
    }

    template<> inline void serialize<Impl::OutputFallback>(std::ostream& s, const Impl::OutputFallback& val) {
        val.apply(s);
    }
}

template<typename T>
Debug& Debug::print(const T& val) {
    if (!_output) return *this;

    #ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
    if(_sourceLocationFile) {
        *_output << _sourceLocationFile << " (" << _sourceLocationLine << ") :";
        _sourceLocationFile = nullptr;
        _printed = true;
    }
    #endif

    if (not _nosp && _printed) {
        *_output << ' ';
        _nosp = false;
    }

    serialize(*_output, val);
    _printed = true;

    return *this;
}

Debug& Debug::operator<<(const char* str) {
    return print(str);
}

Debug& Debug::operator<<(const void* ptr) {
    std::ostringstream out;
    out << "0x" << std::hex << reinterpret_cast<std::uintptr_t>(ptr);
    return print(out.str());
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

Debug& operator<<(Debug& debug, Impl::OutputFallback&& value) {
    return debug.print(value);
}

} // Platform
