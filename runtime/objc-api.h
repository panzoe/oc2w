/*
 * Copyright (c) 1999-2006 Apple Inc.  All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
// Copyright 1988-1996 NeXT Software, Inc.

#ifndef _OBJC_OBJC_API_H_
#   define _OBJC_OBJC_API_H_

    #include <Availability.h>
    #include <AvailabilityMacros.h>
    #include <TargetConditionals.h>

    /* __has_feature __has_extension __has_attribute 均是 Clang 编译支援的函式风宏 */
#   ifndef __has_feature
#       define __has_feature(x) 0               // 兼容非 Clang 编译器环境
#   endif
    
#   ifndef __has_extension
#       define __has_extension __has_feature    // 兼容 pre-3.0 编译器环境
#   endif
    
#   ifndef __has_attribute
#       define __has_attribute(x) 0             // 兼容非 Clang 编译器环境
#   endif


    /*
     * OBJC_API_VERSION 未定义或值为 0 : 仅支援 Tiger 系统和早期 API
     * OBJC_API_VERSION 值为 2: 支援 Leopard 及后续系统以及新版 API
     */
    //#if !defined(OBJC_API_VERSION)
    //    // 根据指定的最小 Mac OS X 系统版本号是否小于 Mac OS X 10.5 决定是否启用 2.0 特性
    //#   if defined(__MAC_OS_X_VERSION_MIN_REQUIRED)  &&  __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_5
    //#       define OBJC_API_VERSION 0
    //#   else
    //#       define OBJC_API_VERSION 2
    //#   endif
    //#endif

    /** 强制使用 2.0 特性 **/
#   define OBJC_API_VERSION 2


    /* OBJC_OLD_DISPATCH_PROTOTYPES == 0 enforces the rule that the dispatch 
     * functions must be cast to an appropriate function pointer type. */
    /** OBJC_OLD_DISPATCH_PROTOTYPES 值为 0 时，则遵循派发函式时必须将其包装为适当的函式指针类型的强制规则 **/
#   if !defined(OBJC_OLD_DISPATCH_PROTOTYPES)
#       define OBJC_OLD_DISPATCH_PROTOTYPES 1
#   endif


    /* OBJC_ISA_AVAILABILITY: `isa` 未来将不可用或被标记为不推荐访问，直接访问该变量的代码编译时将会报警，但不影响继续编译 */
#   if !defined(OBJC_ISA_AVAILABILITY)
//    #   if __OBJC2__
#           define OBJC_ISA_AVAILABILITY  __attribute__((deprecated))
//    #   else
//    #       define OBJC_ISA_AVAILABILITY  /* still available */
//    #   endif
#   endif


    /* OBJC2_UNAVAILABLE: 在 Objective-C 2.0 中不可用, 于 Leopard 系统中开始标记为不推荐，访问该属性的代码将无法继续编译 */
#   if !defined(OBJC2_UNAVAILABLE)
//    #   if __OBJC2__
#           define OBJC2_UNAVAILABLE UNAVAILABLE_ATTRIBUTE
//    #   else
//            /* plain C code also falls here, but this is close enough */
//    #       define OBJC2_UNAVAILABLE __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0)
//    #   endif
#   endif

    /* OBJC_ARC_UNAVAILABLE: ARC 模式下不可用 （即使用编译选项 -fobjc-arc 时） */
    #if !defined(OBJC_ARC_UNAVAILABLE)
    #   if __has_feature(objc_arc)
    #       if __has_extension(attribute_unavailable_with_message)
    #           define OBJC_ARC_UNAVAILABLE __attribute__((unavailable("not available in automatic reference counting mode")))
    #       else
    #           define OBJC_ARC_UNAVAILABLE __attribute__((unavailable))
    #       endif
    #   else
    #       define OBJC_ARC_UNAVAILABLE
    #   endif
    #endif

    #if !defined(OBJC_HIDE_64)
    /* OBJC_ARM64_UNAVAILABLE: arm64 框架下不可用 (i.e. stret dispatch) */
    #   if !defined(OBJC_ARM64_UNAVAILABLE)
    #       if defined(__arm64__)
    #           define OBJC_ARM64_UNAVAILABLE __attribute__((unavailable("not available in arm64")))
    #       else
    #           define OBJC_ARM64_UNAVAILABLE
    #       endif
    #   endif
    #endif

    /* OBJC_GC_UNAVAILABLE: unavailable with -fobjc-gc or -fobjc-gc-only */
    #if !defined(OBJC_GC_UNAVAILABLE)
    #   if __OBJC_GC__
    #       if __has_extension(attribute_unavailable_with_message)
    #           define OBJC_GC_UNAVAILABLE __attribute__((unavailable("not available in garbage collecting mode")))
    #       else
    #           define OBJC_GC_UNAVAILABLE __attribute__((unavailable))
    #       endif
    #   else
    #       define OBJC_GC_UNAVAILABLE
    #   endif
    #endif

    /** C 函式外部引用声明 **/
    #if !defined(OBJC_EXTERN)
    #   if defined(__cplusplus)
    #       define OBJC_EXTERN extern "C" 
    #   else
    #       define OBJC_EXTERN extern
    #   endif
    #endif

    /** 根据编译目标使用不同的关键词修饰使用该宏定义的函式声明，以帮助编译器生成导入或导出风格 **/
    #if !defined(OBJC_VISIBLE)
    #   if TARGET_OS_WIN32
    #       if defined(BUILDING_OBJC)
    #           define OBJC_VISIBLE __declspec(dllexport)
    #       else
    #           define OBJC_VISIBLE __declspec(dllimport)
    #       endif
    #   else
    #       define OBJC_VISIBLE  __attribute__((visibility("default")))
    #   endif
    #endif

    #if !defined(OBJC_EXPORT)
    #   define OBJC_EXPORT  OBJC_EXTERN OBJC_VISIBLE
    #endif

    #if !defined(OBJC_IMPORT)
    #   define OBJC_IMPORT extern
    #endif

    /** 根类标志宏，用于指名以下声明类的是一个根类 **/
    #if !defined(OBJC_ROOT_CLASS)
    #   if __has_attribute(objc_root_class)
    #       define OBJC_ROOT_CLASS __attribute__((objc_root_class))
    #   else
    #       define OBJC_ROOT_CLASS
    #   endif
    #endif

    #   ifndef __DARWIN_NULL
    #       define __DARWIN_NULL NULL
    #   endif

    #   if !defined(OBJC_INLINE)
    #       define OBJC_INLINE __inline
    #   endif

#endif
