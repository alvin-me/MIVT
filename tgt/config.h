#pragma once

#ifdef TGT_EXPORTS
  // building library -> export symbols
  #ifdef WIN32
    #define TGT_API __declspec(dllexport)
  #else
    #define TGT_API
  #endif
#else
  // including library -> import symbols
  #ifdef WIN32
    #define TGT_API __declspec(dllimport)
  #else
    #define TGT_API
  #endif
#endif

