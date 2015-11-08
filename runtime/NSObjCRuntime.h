/*	NSObjCRuntime.h
	Copyright (c) 1994-2012, Apple Inc. All rights reserved.
*/

#ifndef _OBJC_NSOBJCRUNTIME_H_
    #   define _OBJC_NSOBJCRUNTIME_H_

    #   include <TargetConditionals.h>
    #   include <objc/objc.h>

    //#if __LP64__ || (TARGET_OS_EMBEDDED && !TARGET_OS_IPHONE) || TARGET_OS_WIN32 || NS_BUILD_32_LIKE_64
    //typedef long NSInteger;
    //typedef unsigned long NSUInteger;
    //#else
    //typedef int NSInteger;
    //typedef unsigned int NSUInteger;
    //#endif

        typedef long NSInteger;
        typedef unsigned long NSUInteger;

    #   define NSIntegerMax    LONG_MAX
    #   define NSIntegerMin    LONG_MIN
    #   define NSUIntegerMax   ULONG_MAX

    #   define NSINTEGER_DEFINED 1

#endif
