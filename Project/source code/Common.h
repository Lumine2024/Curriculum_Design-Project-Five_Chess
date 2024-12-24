#pragma once
#ifndef _MSC_VER
	#error This project can only be compiled in Visual Studio.
#endif
#include <yvals_core.h> // for _HAS_CXX17
#if !_HAS_CXX17 && __cplusplus < 201703L
	#error C++17 is required.
#endif
#pragma push_macro("msvc")
#pragma push_macro("noinline")
#pragma push_macro("always_inline")
#undef msvc
#undef noinline
#undef always_inline
// 在release模式下，将部分函数声明为inline，以减少函数调用开销
#ifndef RELEASE_INLINE
	#ifdef _DEBUG
		#define RELEASE_INLINE [[msvc::noinline]]
	#else
		#define RELEASE_INLINE [[msvc::always_inline]]
	#endif
#endif
#pragma pop_macro("msvc")
#pragma pop_macro("noinline")
#pragma pop_macro("always_inline")
#include <string>
#ifdef _UNICODE
	typedef wchar_t TCHAR;
#else
	typedef char TCHAR;
#endif
namespace std {
using tstring = std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>>;
}