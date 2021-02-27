//
//

#pragma once

#include <type_traits>

#include "defines.h"

namespace Platform {

#define PLATFORM_TYPE_TEST(type, ...) \
	template<typename W> struct type { \
		template<typename T> static char test(T&&, __VA_ARGS__* = nullptr); \
		static short test(...); \
		static constexpr bool value = sizeof(test(std::declval<W>())) == sizeof(char); \
	}; \
	/* */

namespace Impl {
	PLATFORM_TYPE_TEST(HasMemberBegin, decltype(std::declval<T>().begin()));
	PLATFORM_TYPE_TEST(HasMemberEnd, decltype(std::declval<T>().end()));
	PLATFORM_TYPE_TEST(HasBegin, decltype(begin(std::declval<T>())));
	PLATFORM_TYPE_TEST(HasEnd, decltype(end(std::declval<T>())));
	PLATFORM_TYPE_TEST(HasMemberCStr, decltype(std::declval<T>().c_str()));
}

template<typename T>
using IsIterable = std::integral_constant<bool, 
	(Impl::HasMemberBegin<T>::value || Impl::HasBegin<T>::value) &&
	(Impl::HasMemberEnd<T>::value || Impl::HasEnd<T>::value)
	>;

template<typename T>
using IsString = std::integral_constant<bool,
	Impl::HasMemberCStr<T>::value
	>;

}
