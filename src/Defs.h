#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdio.h>
#include <string>
#include <list>
#include <map>

#include <platform/Log.h>

#if defined OZW_FATAL_ERROR || defined OZW_ERROR
#   undef OZW_FATAL_ERROR
#   undef OZW_ERROR
#endif

#ifndef OPENZWAVE_DISABLE_EXCEPTIONS
#   define OZW_FATAL_ERROR(exitCode, msg)   	Log::Write(OpenZWave::LogLevel_Error,"Exception: %s:%d - %d - %s", std::string(__MYFUNCTION__).substr(std::string(__MYFUNCTION__).find_last_of("/\\") + 1).c_str(), __LINE__, exitCode, msg); \
                                                throw OZWException(__MYFUNCTION__, __LINE__, exitCode, msg)
#   define OZW_ERROR(exitCode, msg) 			Log::Write(OpenZWave::LogLevel_Warning,"Exception: %s:%d - %d - %s", std::string(__MYFUNCTION__).substr(std::string(__MYFUNCTION__).find_last_of("/\\") + 1).c_str(), __LINE__, exitCode, msg); \
                                                throw OZWException(__MYFUNCTION__, __LINE__, exitCode, msg)
#else
#   define OZW_FATAL_ERROR(exitCode, msg)   	Log::Write(OpenZWave::LogLevel_Error,"Exception: %s:%d - %d - %s", std::string(__MYFUNCTION__).substr(std::string(__MYFUNCTION__).find_last_of("/\\") + 1).c_str(), __LINE__, exitCode, msg); \
                                                std::cerr << "Error: "<< std::string(__MYFUNCTION__).substr(std::string(__MYFUNCTION__).find_last_of("/\\") + 1) << ":" << __LINE__ << " - " << msg << std::endl; exit(exitCode)
#   define OZW_ERROR(exitCode, msg)			    Log::Write(OpenZWave::LogLevel_Warning,"Exception: %s:%d - %d - %s", std::string(__MYFUNCTION__).substr(std::string(__MYFUNCTION__).find_last_of("/\\") + 1).c_str(), __LINE__, exitCode, msg);
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::list;
using std::map;

using OpenZWave::Log;

#endif // DEFS_H_INCLUDED
