#ifndef MIVT_CONFIG_H
#define MIVT_CONFIG_H

#ifdef MIVT_EXPORTS
  // building library -> export symbols
  #ifdef WIN32
    #define MIVT_API __declspec(dllexport)
  #else
    #define MIVT_API
  #endif
#else
  // including library -> import symbols
  #ifdef WIN32
    #define MIVT_API __declspec(dllimport)
  #else
    #define MIVT_API
  #endif
#endif

#endif // MIVT_CONFIG_H

