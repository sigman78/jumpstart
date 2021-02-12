//
//

#pragma once

#include <iosfwd>
#include <type_traits>
#include <utility>


namespace Platform {

#if (defined(__GNUC__) && !defined(__clang__)) || (defined(__clang__) && ((defined(__apple_build_version__) && __clang_major__ >= 12) || (!defined(__apple_build_version__) && __clang_major__ >= 9))) || (defined(_MSC_VER) && _MSC_VER >= 1926)
#define PLATFORM_DEBUG_HAS_SOURCE_LOCATION
namespace Impl { struct SourceLocation; }
#endif

class Debug {
public:

	using Op = *void(Debug&);

	static void nosp(Debug& debug) {
		debug._nosp = true;
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

	static std::stream* systemOutput();
	static std::stream* output();

	Debug();
	Debug(std::stream* output);
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

private:
	template<typename T> Debug& print(const T& value);

	std::ostream* _output;
	std::ostream* _previousGlobalOutput;

	void cleanup();

#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
	friend Impl::SourceLocation;
    const char* _sourceLocationFile{};
    int _sourceLocationLine{};
#endif
	bool _nosp{};
};

#ifdef PLATFORM_DEBUG_HAS_SOURCE_LOCATION
namespace Impl {
	struct SourceLocation {
        #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
		SourceLocation(Debug&& debug, const char* file = __builtin_FILE(), int line = __builtin_LINE());
		#else
		#error Unsupported compiler
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

// Main workhorse
template<typename T> inline Debug& operator<<(Debug&& debug, const T& value) {
    return debug << value;
}

// Iterable impl
// TODO:

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
};

}