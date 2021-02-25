//
//

#pragma once

#include "configure.h"
#include "traits.h"
#include <iosfwd>
#include <string>
#include <type_traits>
#include <utility>


namespace Platform {

#if (defined(__GNUC__) && !defined(__clang__)) || (defined(__clang__) && ((defined(__apple_build_version__) && __clang_major__ >= 12) || (!defined(__apple_build_version__) && __clang_major__ >= 9))) || (defined(_MSC_VER) && _MSC_VER >= 1926)
#define PLATFORM_DEBUG_HAS_SOURCE_LOCATION
namespace Impl { struct SourceLocation; }
#endif

class Debug {
public:
    using Op = void(Debug&);

    static void nosp(Debug& debug) {
        debug._nosp = true;
    }

    static void nonl(Debug& debug) {
        debug._nonl = true;
    }

    static void nl(Debug& debug) {
        debug << nosp << "\n" << nosp;
    }

    static void sp(Debug& debug) {
        debug << nosp << " " << nosp;
    }

    Debug& operator<<(Op op) {
        op(*this);
        return *this;
    }

    static std::ostream* systemOutput();
    static std::ostream* output();

    Debug();
    Debug(std::ostream* output);
    Debug(const Debug&) = delete;
    Debug(Debug&&) = delete;
    Debug& operator=(const Debug&) = delete;
    Debug& operator=(Debug&&) = delete;
    ~Debug();

    Debug& operator<<(const char* str);
    Debug& operator<<(const void* ptr);
    Debug& operator<<(bool val);
    Debug& operator<<(char val);
    Debug& operator<<(unsigned char val);
    Debug& operator<<(int val);
    Debug& operator<<(long val);
    Debug& operator<<(long long val);
    Debug& operator<<(unsigned int val);
    Debug& operator<<(unsigned long val);
    Debug& operator<<(unsigned long long val);

    Debug& operator<<(float val);
    Debug& operator<<(double val);

    Debug& operator<<(std::nullptr_t);

    template<typename T> Debug& print(const T& value);
protected:

    std::ostream* _output;
    std::ostream* _previousGlobalOutput;

    void cleanup();

#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
    friend Impl::SourceLocation;
    const char* _sourceLocationFile{};
    int _sourceLocationLine{};
#endif
    bool _nosp{};
    bool _nonl{};
    bool _printed{};
};

#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
namespace Impl {
    struct SourceLocation {
        #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
        SourceLocation(Debug&& debug, const char* file = __builtin_FILE(), int line = __builtin_LINE());
        #else
        #error Inconsistent PLATFORM_DEBUG_HAS_SOURCE_LOCATION
        #endif
        Debug* debug;
    };
}

inline Debug& operator!(Impl::SourceLocation loc) {
    return *loc.debug;
}
#else
inline Debug& operator!(Debug&& debug) { 
    return debug; 
}
#endif

// Warning output
class Warning: public Debug {
public:
    static std::ostream* systemOutput();
    static std::ostream* output();

    Warning();
    Warning(std::ostream* output);
    Warning(const Warning&) = delete;
    Warning(Warning&&) = delete;
    Warning& operator=(const Warning&) = delete;
    Warning& operator=(Warning&&) = delete;
    ~Warning();

private:
    std::ostream* _previousGlobalWarningOutput{};
};


// Error output
class Error: public Debug {
public:
    static std::ostream* systemOutput();
    static std::ostream* output();

    Error();
    Error(std::ostream* output);
    Error(const Error&) = delete;
    Error(Error&&) = delete;
    Error& operator=(const Error&) = delete;
    Error& operator=(Error&&) = delete;
    ~Error();

private:
    std::ostream* _previousGlobalErrorOutput{};
};

namespace Impl {

struct OutputFallback {
    template<class T> OutputFallback(const T& t): applier(&OutputFallback::makeApply<T>), value(&t) {}

    void apply(std::ostream& s) const {
        (this->*applier)(s);
    }

    template<class T> void makeApply(std::ostream& s) const {
        s << *static_cast<const T*>(value);
    }

    using Fn = void(OutputFallback::*)(std::ostream&) const;
    const Fn applier;
    const void* value;
};

} // Impl

Debug& operator<<(Debug& d, Impl::OutputFallback&& val);

template<typename T, typename U> Debug& operator<<(Debug& debug, const std::pair<T,U>& val) {
    debug << "(" << Debug::nosp << val.first << ", " << val.second << ")";
    return debug;
}

template<typename Iterable> Debug& operator<<(typename std::enable_if<IsIterable<Iterable>::value && !IsString<Iterable>::value, Debug&>::type debug, const Iterable& val) {
    // Nested containers may need additional work wrt format flags
    debug << "{" << Debug::nosp;
    for(auto it = val.begin(); it != val.end(); ++it) {
        if (it != val.begin())
            debug << "," << Debug::sp;
        debug << *it;
    }
    debug << "}";
    return debug;
}

template<typename T> inline Debug& operator<<(Debug&& debug, const T& value) {
    return debug << value;
}

} // Platform
