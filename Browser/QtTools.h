#pragma once

#if !defined(WIN32)
#define QTTOOLS_API
#else
#ifdef QTTOOLS_EXPORTS
#define QTTOOLS_API __declspec(dllexport)
#else
#define QTTOOLS_API __declspec(dllimport)
#endif
#endif // WIN32