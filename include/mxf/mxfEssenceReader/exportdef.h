#ifndef MXFAPP_ESSENCEREADEREREXPORT_H
#define MXFAPP_ESSENCEREADEREREXPORT_H

#ifdef _WIN32


#ifdef MXFESSENCEREADERDLL_EXPORTS
#define MXFESSENCEREADERDLL_API __declspec(dllexport)
#else
#define MXFESSENCEREADERDLL_API __declspec(dllimport)
#endif


#else 
#define MXFESSENCEREADERDLL_API
#endif



#endif



