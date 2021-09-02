// Software License Agreement (BSD License)
//
// Copyright (c) 2010-2015, Deusty, LLC
// All rights reserved.
//
// Redistribution and use of this software in source and binary forms,
// with or without modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Neither the name of Deusty nor the names of its contributors may be used
//   to endorse or promote products derived from this software without specific
//   prior written permission of Deusty, LLC.

// Disable legacy macros
#ifndef NMC_DD_LEGACY_MACROS
    #define NMC_DD_LEGACY_MACROS 0
#endif

#import "NMCDDLog.h"
#import "NMCLogFormatter.h"

/**
 * The constant/variable/method responsible for controlling the current log level.
 **/
//#define NMC_LOG_LEVEL_DEF NMCDDLogLevel
//#ifndef NMC_LOG_LEVEL_DEF
//    #ifdef NMCDDLogLevel
//        #define NMC_LOG_LEVEL_DEF NMCDDLogLevel
//    #else
//        #define NMC_LOG_LEVEL_DEF NMCDDLogLevelVerbose
//    #endif
//#endif

#if DEBUG
//默认关闭HTLog，防止影响用户的日志打印。
#define NMC_LOG_LEVEL_DEF  NMCDDLogLevelDebug
#else
#define NMC_LOG_LEVEL_DEF NMCDDLogLevelDebug
#endif

/**
 * Whether async should be used by log messages, excluding error messages that are always sent sync.
 **/
#ifndef NMC_LOG_ASYNC_ENABLED
    #define NMC_LOG_ASYNC_ENABLED YES
#endif

/**
 * This is the single macro that all other macros below compile into.
 * This big multiline macro makes all the other macros easier to read.
 **/
#define NMC_LOG_MACRO(isAsynchronous, lvl, flg, ctx, atag, fnct, frmt, ...) \
        [NMCDDLog log : isAsynchronous                                     \
             level : lvl                                                \
              flag : flg                                                \
           context : ctx                                                \
              file : __FILE__                                           \
          function : fnct                                               \
              line : __LINE__                                           \
               tag : atag                                               \
            format : (frmt), ## __VA_ARGS__]

/**
 * Define version of the macro that only execute if the log level is above the threshold.
 * The compiled versions essentially look like this:
 *
 * if (logFlagForThisLogMsg & nrtcDDLogLevel) { execute log message }
 *
 * When NRTC_LOG_LEVEL_DEF is defined as nrtcDDLogLevel.
 *
 * As shown further below, Lumberjack actually uses a bitmask as opposed to primitive log levels.
 * This allows for a great amount of flexibility and some pretty advanced fine grained logging techniques.
 *
 * Note that when compiler optimizations are enabled (as they are for your release builds),
 * the log messages above your logging threshold will automatically be compiled out.
 *
 * (If the compiler sees NRTC_LOG_LEVEL_DEF/nrtcDDLogLevel declared as a constant, the compiler simply checks to see
 *  if the 'if' statement would execute, and if not it strips it from the binary.)
 *
 * We also define shorthand versions for asynchronous and synchronous logging.
 **/
#define NMC_LOG_MAYBE(async, lvl, flg, ctx, tag, fnct, frmt, ...) \
        do { if(lvl & flg) NMC_LOG_MACRO(async, lvl, flg, ctx, tag, fnct, frmt, ##__VA_ARGS__); } while(0)

/**
 * Ready to use log macros with no context or tag.
 **/
#define NMCDDLogError(frmt, ...)   NMC_LOG_MAYBE(NO,                NMC_LOG_LEVEL_DEF, NMCDDLogFlagError,   0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogWarn(frmt, ...)    NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagWarning, 0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogInfo(frmt, ...)    NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagInfo,    0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogDebug(frmt, ...)   NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagDebug,   0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogVerbose(frmt, ...) NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagVerbose, 0, nil, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)

/**
 * Ready to use log macros with no context or pull tag.
 * add by hzzhanggenning@corp.netease.com 2017.11.14
 **/
#define NMCDDLogPullError(frmt, ...)   NMC_LOG_MAYBE(NO,                NMC_LOG_LEVEL_DEF, NMCDDLogFlagError,   0, @"pull", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPullWarn(frmt, ...)    NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagWarning, 0, @"pull", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPullInfo(frmt, ...)    NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagInfo,    0, @"pull", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPullDebug(frmt, ...)   NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagDebug,   0, @"pull", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPullVerbose(frmt, ...) NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagVerbose, 0, @"pull", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)

/**
 * Ready to use log macros with no context or push tag.
 * add by hzzhanggenning@corp.netease.com 2017.11.14
 **/
#define NMCDDLogPushError(frmt, ...)   NMC_LOG_MAYBE(NO,                NMC_LOG_LEVEL_DEF, NMCDDLogFlagError,   0, @"push", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPushWarn(frmt, ...)    NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagWarning, 0, @"push", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPushInfo(frmt, ...)    NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagInfo,    0, @"push", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPushDebug(frmt, ...)   NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagDebug,   0, @"push", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define NMCDDLogPushVerbose(frmt, ...) NMC_LOG_MAYBE(NMC_LOG_ASYNC_ENABLED, NMC_LOG_LEVEL_DEF, NMCDDLogFlagVerbose, 0, @"push", __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)

