#pragma once

#ifdef SHAREDVAR_EXPORTS
#define SHAREDVAR_API __declspec(dllexport)
#else
#define SHAREDVAR_API __declspec(dllimport)
#endif

extern "C"
{
	SHAREDVAR_API int _x;
	SHAREDVAR_API int _y;
	SHAREDVAR_API int f1;
	SHAREDVAR_API int f2;
}