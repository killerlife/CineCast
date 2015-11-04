//////////////////////////////////////////////////////////////////////////
//
#ifndef __DCPWRAPLIB_H_
#define __DCPWRAPLIB_H_

#ifdef _WIN32
#ifdef DCPWRAPLIB_EXPORTS
#define DCPWRAPLIB_API __declspec(dllexport)
#else
#define DCPWRAPLIB_API __declspec(dllimport)
#endif
#else
#define DCPWRAPLIB_API
#endif

#endif //__DCPWRAPLIB_H_
