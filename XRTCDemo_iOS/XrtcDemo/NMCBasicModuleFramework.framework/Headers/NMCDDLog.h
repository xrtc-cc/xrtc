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

#import <Foundation/Foundation.h>

// Enable 1.9.x legacy macros if imported directly
#ifndef NMC_DD_LEGACY_MACROS
    #define NMC_DD_LEGACY_MACROS 0
#endif
#if NMC_DD_LEGACY_MACROS
    #import "DDLegacyMacros.h"
#endif

#if OS_OBJECT_USE_OBJC
    #define DISPATCH_QUEUE_REFERENCE_TYPE strong
#else
    #define DISPATCH_QUEUE_REFERENCE_TYPE assign
#endif

@class NMCDDLogMessage;
@protocol NMCDDLogger;
@protocol NMCDDLogFormatter;

/**
 * Define the standard options.
 *
 * We default to only 4 levels because it makes it easier for beginners
 * to make the transition to a logging framework.
 *
 * More advanced users may choose to completely customize the levels (and level names) to suite their needs.
 * For more information on this see the "Custom Log Levels" page:
 * Documentation/CustomLogLevels.md
 *
 * Advanced users may also notice that we're using a bitmask.
 * This is to allow for custom fine grained logging:
 * Documentation/FineGrainedLogging.md
 *
 * -- Flags --
 *
 * Typically you will use the LOG_LEVELS (see below), but the flags may be used directly in certain situations.
 * For example, say you have a lot of warning log messages, and you wanted to disable them.
 * However, you still needed to see your error and info log messages.
 * You could accomplish that with the following:
 *
 * static const NRTCDDLogLevel nrtcDDLogLevel = NRTCDDLogFlagError | NRTCDDLogFlagInfo;
 *
 * When NRTC_LOG_LEVEL_DEF is defined as nrtcDDLogLevel.
 *
 * Flags may also be consulted when writing custom log formatters,
 * as the NRTCDDLogMessage class captures the individual flag that caused the log message to fire.
 *
 * -- Levels --
 *
 * Log levels are simply the proper bitmask of the flags.
 *
 * -- Booleans --
 *
 * The booleans may be used when your logging code involves more than one line.
 * For example:
 *
 * if (LOG_VERBOSE) {
 *     for (id sprocket in sprockets)
 *         NRTCDDLogVerbose(@"sprocket: %@", [sprocket description])
 * }
 *
 * -- Async --
 *
 * Defines the default asynchronous options.
 * The default philosophy for asynchronous logging is very simple:
 *
 * Log messages with errors should be executed synchronously.
 *     After all, an error just occurred. The application could be unstable.
 *
 * All other log messages, such as debug output, are executed asynchronously.
 *     After all, if it wasn't an error, then it was just informational output,
 *     or something the application was easily able to recover from.
 *
 * -- Changes --
 *
 * You are strongly discouraged from modifying this file.
 * If you do, you make it more difficult on yourself to merge future bug fixes and improvements from the project.
 * Instead, create your own MyLogging.h or ApplicationNameLogging.h or CompanyLogging.h
 *
 * For an example of customizing your logging experience, see the "Custom Log Levels" page:
 * Documentation/CustomLogLevels.md
 **/

typedef NS_OPTIONS(NSUInteger, NMCDDLogFlag) {
    NMCDDLogFlagError      = (1 << 0), // 0...00001
    NMCDDLogFlagWarning    = (1 << 1), // 0...00010
    NMCDDLogFlagInfo       = (1 << 2), // 0...00100
    NMCDDLogFlagDebug      = (1 << 3), // 0...01000
    NMCDDLogFlagVerbose    = (1 << 4)  // 0...10000
};

typedef NS_ENUM(NSUInteger, NMCDDLogLevel) {
    NMCDDLogLevelOff       = 0,
    NMCDDLogLevelError     = (NMCDDLogFlagError),                       // 0...00001
    NMCDDLogLevelWarning   = (NMCDDLogLevelError   | NMCDDLogFlagWarning), // 0...00011
    NMCDDLogLevelInfo      = (NMCDDLogLevelWarning | NMCDDLogFlagInfo),    // 0...00111
    NMCDDLogLevelDebug     = (NMCDDLogLevelInfo    | NMCDDLogFlagDebug),   // 0...01111
    NMCDDLogLevelVerbose   = (NMCDDLogLevelDebug   | NMCDDLogFlagVerbose), // 0...11111
    NMCDDLogLevelAll       = NSUIntegerMax                           // 1111....11111 (NRTCDDLogLevelVerbose plus any other flags)
};

/**
 * The THIS_FILE macro gives you an NSString of the file name.
 * For simplicity and clarity, the file name does not include the full path or file extension.
 *
 * For example: NRTCDDLogWarn(@"%@: Unable to find thingy", THIS_FILE) -> @"MyViewController: Unable to find thingy"
 **/

NSString * NMCDDExtractFileNameWithoutExtension(const char *filePath, BOOL copy);

#define THIS_FILE         (NMCDDExtractFileNameWithoutExtension(__FILE__, NO))

/**
 * The THIS_METHOD macro gives you the name of the current objective-c method.
 *
 * For example: NRTCDDLogWarn(@"%@ - Requires non-nil strings", THIS_METHOD) -> @"setMake:model: requires non-nil strings"
 *
 * Note: This does NOT work in straight C functions (non objective-c).
 * Instead you should use the predefined __FUNCTION__ macro.
 **/

#define THIS_METHOD       NSStringFromSelector(_cmd)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NMCDDLog : NSObject

/**
 * Provides access to the underlying logging queue.
 * This may be helpful to Logger classes for things like thread synchronization.
 **/

+ (dispatch_queue_t)loggingQueue;

/**
 * Logging Primitive.
 *
 * This method is used by the macros above.
 * It is suggested you stick with the macros as they're easier to use.
 **/

+ (void)log:(BOOL)synchronous
      level:(NMCDDLogLevel)level
       flag:(NMCDDLogFlag)flag
    context:(NSInteger)context
       file:(const char *)file
   function:(const char *)function
       line:(NSUInteger)line
        tag:(id)tag
     format:(NSString *)format, ... NS_FORMAT_FUNCTION(9,10);

/**
 * Logging Primitive.
 *
 * This method can be used if you have a prepared va_list.
 **/

+ (void)log:(BOOL)asynchronous
      level:(NMCDDLogLevel)level
       flag:(NMCDDLogFlag)flag
    context:(NSInteger)context
       file:(const char *)file
   function:(const char *)function
       line:(NSUInteger)line
        tag:(id)tag
     format:(NSString *)format
       args:(va_list)argList;

/**
 * Logging Primitive.
 **/
+ (void)log:(BOOL)asynchronous
    message:(NSString *)message
      level:(NMCDDLogLevel)level
       flag:(NMCDDLogFlag)flag
    context:(NSInteger)context
       file:(const char *)file
   function:(const char *)function
       line:(NSUInteger)line
        tag:(id)tag;

/**
 * Logging Primitive.
 *
 * This method can be used if you manualy prepared NRTCDDLogMessage.
 **/

+ (void)log:(BOOL)asynchronous
    message:(NMCDDLogMessage *)logMessage;

/**
 * Since logging can be asynchronous, there may be times when you want to flush the logs.
 * The framework invokes this automatically when the application quits.
 **/

+ (void)flushLog;

/**
 * Loggers
 *
 * In order for your log statements to go somewhere, you should create and add a logger.
 *
 * You can add multiple loggers in order to direct your log statements to multiple places.
 * And each logger can be configured separately.
 * So you could have, for example, verbose logging to the console, but a concise log file with only warnings & errors.
 **/

/**
 * Adds the logger to the system.
 *
 * This is equivalent to invoking [NRTCDDLog addLogger:logger withLogLevel:NRTCDDLogLevelAll].
 **/
+ (void)addLogger:(id <NMCDDLogger>)logger;

/**
 * Adds the logger to the system.
 *
 * The level that you provide here is a preemptive filter (for performance).
 * That is, the level specified here will be used to filter out logMessages so that
 * the logger is never even invoked for the messages.
 *
 * More information:
 * When you issue a log statement, the logging framework iterates over each logger,
 * and checks to see if it should forward the logMessage to the logger.
 * This check is done using the level parameter passed to this method.
 *
 * For example:
 *
 * [NRTCDDLog addLogger:consoleLogger withLogLevel:NRTCDDLogLevelVerbose];
 * [NRTCDDLog addLogger:fileLogger    withLogLevel:NRTCDDLogLevelWarning];
 *
 * NRTCDDLogError(@"oh no"); => gets forwarded to consoleLogger & fileLogger
 * NRTCDDLogInfo(@"hi");     => gets forwarded to consoleLogger only
 *
 * It is important to remember that Lumberjack uses a BITMASK.
 * Many developers & third party frameworks may define extra log levels & flags.
 * For example:
 *
 * #define SOME_FRAMEWORK_LOG_FLAG_TRACE (1 << 6) // 0...1000000
 *
 * So if you specify NRTCDDLogLevelVerbose to this method, you won't see the framework's trace messages.
 *
 * (SOME_FRAMEWORK_LOG_FLAG_TRACE & NRTCDDLogLevelVerbose) => (01000000 & 00011111) => NO
 *
 * Consider passing NRTCDDLogLevelAll to this method, which has all bits set.
 * You can also use the exclusive-or bitwise operator to get a bitmask that has all flags set,
 * except the ones you explicitly don't want. For example, if you wanted everything except verbose & debug:
 *
 * ((NRTCDDLogLevelAll ^ NRTCDDLogLevelVerbose) | NRTCDDLogLevelInfo)
 **/
+ (void)addLogger:(id <NMCDDLogger>)logger withLevel:(NMCDDLogLevel)level;

+ (void)removeLogger:(id <NMCDDLogger>)logger;
+ (void)removeAllLoggers;

+ (NSArray *)allLoggers;

//taojl
+ (void)changeLogger:(id <NMCDDLogger>)logger withLevel:(NMCDDLogLevel)level;

/**
 * Registered Dynamic Logging
 *
 * These methods allow you to obtain a list of classes that are using registered dynamic logging,
 * and also provides methods to get and set their log level during run time.
 **/

+ (NSArray *)registeredClasses;
+ (NSArray *)registeredClassNames;

+ (NMCDDLogLevel)levelForClass:(Class)aClass;
+ (NMCDDLogLevel)levelForClassWithName:(NSString *)aClassName;

+ (void)setLevel:(NMCDDLogLevel)level forClass:(Class)aClass;
+ (void)setLevel:(NMCDDLogLevel)level forClassWithName:(NSString *)aClassName;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@protocol NMCDDLogger <NSObject>

- (void)logMessage:(NMCDDLogMessage *)logMessage;

/**
 * Formatters may optionally be added to any logger.
 *
 * If no formatter is set, the logger simply logs the message as it is given in logMessage,
 * or it may use its own built in formatting style.
 **/
@property (nonatomic, strong) id <NMCDDLogFormatter> logFormatter;

@optional

/**
 * Since logging is asynchronous, adding and removing loggers is also asynchronous.
 * In other words, the loggers are added and removed at appropriate times with regards to log messages.
 *
 * - Loggers will not receive log messages that were executed prior to when they were added.
 * - Loggers will not receive log messages that were executed after they were removed.
 *
 * These methods are executed in the logging thread/queue.
 * This is the same thread/queue that will execute every logMessage: invocation.
 * Loggers may use these methods for thread synchronization or other setup/teardown tasks.
 **/
- (void)didAddLogger;
- (void)willRemoveLogger;

/**
 * Some loggers may buffer IO for optimization purposes.
 * For example, a database logger may only save occasionaly as the disk IO is slow.
 * In such loggers, this method should be implemented to flush any pending IO.
 *
 * This allows invocations of DDLog's flushLog method to be propogated to loggers that need it.
 *
 * Note that DDLog's flushLog method is invoked automatically when the application quits,
 * and it may be also invoked manually by the developer prior to application crashes, or other such reasons.
 **/
- (void)flush;

/**
 * Each logger is executed concurrently with respect to the other loggers.
 * Thus, a dedicated dispatch queue is used for each logger.
 * Logger implementations may optionally choose to provide their own dispatch queue.
 **/
@property (nonatomic, DISPATCH_QUEUE_REFERENCE_TYPE, readonly) dispatch_queue_t loggerQueue;

/**
 * If the logger implementation does not choose to provide its own queue,
 * one will automatically be created for it.
 * The created queue will receive its name from this method.
 * This may be helpful for debugging or profiling reasons.
 **/
@property (nonatomic, readonly) NSString *loggerName;

/**
 Use for identify current sdk logger when multiple sdk loggers exist
 -- add by zgn 2018.12.24
 */
@property (nonatomic, readonly) NSString *sdkIdentify;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@protocol NMCDDLogFormatter <NSObject>
@required

/**
 * Formatters may optionally be added to any logger.
 * This allows for increased flexibility in the logging environment.
 * For example, log messages for log files may be formatted differently than log messages for the console.
 *
 * For more information about formatters, see the "Custom Formatters" page:
 * Documentation/CustomFormatters.md
 *
 * The formatter may also optionally filter the log message by returning nil,
 * in which case the logger will not log the message.
 **/
- (NSString *)formatLogMessage:(NMCDDLogMessage *)logMessage;

@optional

/**
 * A single formatter instance can be added to multiple loggers.
 * These methods provides hooks to notify the formatter of when it's added/removed.
 *
 * This is primarily for thread-safety.
 * If a formatter is explicitly not thread-safe, it may wish to throw an exception if added to multiple loggers.
 * Or if a formatter has potentially thread-unsafe code (e.g. NSDateFormatter),
 * it could possibly use these hooks to switch to thread-safe versions of the code.
 **/
- (void)didAddToLogger:(id <NMCDDLogger>)logger;
- (void)willRemoveFromLogger:(id <NMCDDLogger>)logger;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@protocol NMCDDRegisteredDynamicLogging

/**
 * Implement these methods to allow a file's log level to be managed from a central location.
 *
 * This is useful if you'd like to be able to change log levels for various parts
 * of your code from within the running application.
 *
 * Imagine pulling up the settings for your application,
 * and being able to configure the logging level on a per file basis.
 *
 * The implementation can be very straight-forward:
 *
 * + (int)nrtcDDLogLevel
 * {
 *     return nrtcDDLogLevel;
 * }
 *
 * + (void)ddSetLogLevel:(NRTCDDLogLevel)level
 * {
 *     nrtcDDLogLevel = level;
 * }
 **/

+ (NMCDDLogLevel)NMCDDLogLevel;
+ (void)ddSetLogLevel:(NMCDDLogLevel)level;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NS_DESIGNATED_INITIALIZER
    #define NS_DESIGNATED_INITIALIZER
#endif

/**
 * The NRTCDDLogMessage class encapsulates information about the log message.
 * If you write custom loggers or formatters, you will be dealing with objects of this class.
 **/

typedef NS_OPTIONS(NSInteger, NMCDDLogMessageOptions) {
    NMCDDLogMessageCopyFile     = 1 << 0,
    NMCDDLogMessageCopyFunction = 1 << 1
};

@interface NMCDDLogMessage : NSObject <NSCopying>
{
    // Direct accessors to be used only for performance
    @public
    NSString *_message;
    NMCDDLogLevel _level;
    NMCDDLogFlag _flag;
    NSInteger _context;
    NSString *_file;
    NSString *_fileName;
    NSString *_function;
    NSUInteger _line;
    id _tag;
    NMCDDLogMessageOptions _options;
    NSDate *_timestamp;
    NSString *_threadID;
    NSString *_threadName;
    NSString *_queueLabel;
}

/**
 * Standard init method for a log message object.
 * Used by the logging primitives. (And the macros use the logging primitives.)
 *
 * If you find need to manually create logMessage objects, there is one thing you should be aware of:
 *
 * If no flags are passed, the method expects the file and function parameters to be string literals.
 * That is, it expects the given strings to exist for the duration of the object's lifetime,
 * and it expects the given strings to be immutable.
 * In other words, it does not copy these strings, it simply points to them.
 * This is due to the fact that __FILE__ and __FUNCTION__ are usually used to specify these parameters,
 * so it makes sense to optimize and skip the unnecessary allocations.
 * However, if you need them to be copied you may use the options parameter to specify this.
 * Options is a bitmask which supports NRTCDDLogMessageCopyFile and NRTCDDLogMessageCopyFunction.
 **/
- (instancetype)initWithMessage:(NSString *)message
                          level:(NMCDDLogLevel)level
                           flag:(NMCDDLogFlag)flag
                        context:(NSInteger)context
                           file:(NSString *)file
                       function:(NSString *)function
                           line:(NSUInteger)line
                            tag:(id)tag
                        options:(NMCDDLogMessageOptions)options
                      timestamp:(NSDate *)timestamp;

/**
 * Read-only properties
 **/
@property (readonly, nonatomic) NSString *message;
@property (readonly, nonatomic) NMCDDLogLevel level;
@property (readonly, nonatomic) NMCDDLogFlag flag;
@property (readonly, nonatomic) NSInteger context;
@property (readonly, nonatomic) NSString *file;
@property (readonly, nonatomic) NSString *fileName;
@property (readonly, nonatomic) NSString *function;
@property (readonly, nonatomic) NSUInteger line;
@property (readonly, nonatomic) id tag;
@property (readonly, nonatomic) NMCDDLogMessageOptions options;
@property (readonly, nonatomic) NSDate *timestamp;
@property (readonly, nonatomic) NSString *threadID; // ID as it appears in NSLog calculated from the machThreadID
@property (readonly, nonatomic) NSString *threadName;
@property (readonly, nonatomic) NSString *queueLabel;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * The NRTCDDLogger protocol specifies that an optional formatter can be added to a logger.
 * Most (but not all) loggers will want to support formatters.
 *
 * However, writting getters and setters in a thread safe manner,
 * while still maintaining maximum speed for the logging process, is a difficult task.
 *
 * To do it right, the implementation of the getter/setter has strict requiremenets:
 * - Must NOT require the logMessage method to acquire a lock.
 * - Must NOT require the logMessage method to access an atomic property (also a lock of sorts).
 *
 * To simplify things, an abstract logger is provided that implements the getter and setter.
 *
 * Logger implementations may simply extend this class,
 * and they can ACCESS THE FORMATTER VARIABLE DIRECTLY from within their logMessage method!
 **/

@interface NMCDDAbstractLogger : NSObject <NMCDDLogger>
{
    // Direct accessors to be used only for performance
    @public
    id <NMCDDLogFormatter> _logFormatter;
    dispatch_queue_t _loggerQueue;
}

@property (nonatomic, strong) id <NMCDDLogFormatter> logFormatter;
@property (nonatomic, DISPATCH_QUEUE_REFERENCE_TYPE) dispatch_queue_t loggerQueue;

// For thread-safety assertions
@property (nonatomic, readonly, getter=isOnGlobalLoggingQueue)  BOOL onGlobalLoggingQueue;
@property (nonatomic, readonly, getter=isOnInternalLoggerQueue) BOOL onInternalLoggerQueue;

@end

