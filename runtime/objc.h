/*
 * Copyright (c) 1999-2007 Apple Inc.  All Rights Reserved.
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
/*
 *	objc.h
 *	Copyright 1988-1996, NeXT Software, Inc.
 */

#ifndef _OBJC_OBJC_H_
#   define _OBJC_OBJC_H_

#   include <sys/types.h>      // for __DARWIN_NULL
#   include <Availability.h>
#   include <objc/objc-api.h>
#   include <stdbool.h>

#   if !OBJC_TYPES_DEFINED
        /// 用于表示一个 Objective-C 类.
        typedef struct objc_class *Class;

        /// 类的实例
        struct objc_object {
            Class isa  OBJC_ISA_AVAILABILITY;
        };

        /// 指向类实例的指针
        typedef struct objc_object *id;
#   endif

    /// 表示为一个方法选择器类型
    typedef struct objc_selector *SEL;

    /// 指向方法实现体函式的指针
#   if !OBJC_OLD_DISPATCH_PROTOTYPES
        typedef void (*IMP)(void /* id, SEL, ... */ ); 
#   else
        typedef id (*IMP)(id, SEL, ...);
#   endif

#   define OBJC_BOOL_DEFINED

    /// Type to represent a boolean value.
#   if !defined(OBJC_HIDE_64) && TARGET_OS_IPHONE && __LP64__
        typedef bool BOOL;
#   else
        typedef signed char BOOL;
    // BOOL is explicitly signed so @encode(BOOL) == "c" rather than "C" 
    // even if -funsigned-char is used.
#   endif

#   if __has_feature(objc_bool)
#       define YES __objc_yes
#       define NO  __objc_no
#   else
#       define YES ((BOOL)1)
#       define NO  ((BOOL)0)
#   endif

#   ifndef Nil
#       if __has_feature(cxx_nullptr)
#           define Nil nullptr
#       else
#           define Nil __DARWIN_NULL
#       endif
#   endif

#   ifndef nil
#       if __has_feature(cxx_nullptr)
#           define nil nullptr
#       else
#           define nil __DARWIN_NULL
#       endif
#   endif

#   if ! (defined(__OBJC_GC__)  ||  __has_feature(objc_arc))
#       define __strong             /* empty */
#   endif

#   if !__has_feature(objc_arc)
#       define __unsafe_unretained  /* empty */
#       define __autoreleasing      /* empty */
#   endif


    /**
     * 返回给定 “方法选择器” 的名称
     *
     * @param sel SEL 类型指针，传递要检测的选择器
     *
     * @return 用于表示选择器名称的 C 语言风格字元串
     */
    OBJC_EXPORT const char *sel_getName(SEL sel);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /**
     * 向 Objective-C “运行时系统” 注册一个方法
     *
     * @param str 要注册的方法命名的 C 语言风格字元串指针
     *
     * @return 一个指向命名方法的选择器的 SEL 类型指针
     *
     * @note 方法经函式注册后，其名称会映射到对应的选择器，并作为函式返回值
     *
     * @note 在向 “类定义” 添加方法前，选择器必须已经用其名称向 Objective-C “运行时系统” 注册过了
     *       如果方法名已被注册，此函式会直接返回该选择器
     */
    OBJC_EXPORT SEL sel_registerName(const char *str);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /**
     * 获取对象的类名
     *
     * @param obj Objective-C 对象
     *
     * @return 实例对象的类名称
     */
    OBJC_EXPORT const char *object_getClassName(id obj);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * Returns a pointer to any extra bytes allocated with an instance given object.
     * 
     * @param obj An Objective-C object.
     * 
     * @return A pointer to any extra bytes allocated with \e obj. If \e obj was
     *   not allocated with any extra bytes, then dereferencing the returned pointer is undefined.
     * 
     * @note This function returns a pointer to any extra bytes allocated with the instance
     *  (as specified by \c class_createInstance with extraBytes>0). This memory follows the
     *  object's ordinary ivars, but may not be adjacent to the last ivar.
     * @note The returned pointer is guaranteed to be pointer-size aligned, even if the area following
     *  the object's last ivar is less aligned than that. Alignment greater than pointer-size is never
     *  guaranteed, even if the area following the object's last ivar is more aligned than that.
     * @note In a garbage-collected environment, the memory is scanned conservatively.
     */
    OBJC_EXPORT void *object_getIndexedIvars(id obj);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 检测选择器有效性
     * 
     * @param sel 要检测的选择器
     * 
     * @return 若选择器有效会拥有一个函式实现则返回 YES，否则返回 NO
     * 
     * @warning 在部分平台上，一个无效引用（引用了无效的记忆体地址）会引发程式崩溃
     */
    OBJC_EXPORT BOOL sel_isMapped(SEL sel);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /**
     * 向 Objective-C “运行时系统” 注册一个方法名
     *
     * @param str 一个 C 语言风格字元串指针，用于传递要注册的方法名称
     *
     * @return  一个指向命名方法选择器的 SEL 类型指针
     *
     * @note 该方法实现已与函式名表意不同
     * @note 此方法实现与 sel_registerName 函式实现是相同的
     * @note 在 OS X 10.0 系统之前，此方法会尝试寻找此名称映射的选择器，如未找到则返回 NULL
     *       这一转变是安全的，因为事实上多数调用的方法并不会对返回值进行 NULL检查
     */
    OBJC_EXPORT SEL sel_getUid(const char *str);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);


    // 过时的 ARC 转换方法，即将弃用
    // 使用 CFBridgingRetain，CFBridgingRelease 和 __bridge 作为替换方案

    typedef const void* objc_objectptr_t;

#   if __has_feature(objc_arc)
#       define objc_retainedObject(o) ((__bridge_transfer id)(objc_objectptr_t)(o))
#       define objc_unretainedObject(o) ((__bridge id)(objc_objectptr_t)(o))
#       define objc_unretainedPointer(o) ((__bridge objc_objectptr_t)(id)(o))
#   else
#       define objc_retainedObject(o) ((id)(objc_objectptr_t)(o))
#       define objc_unretainedObject(o) ((id)(objc_objectptr_t)(o))
#       define objc_unretainedPointer(o) ((objc_objectptr_t)(id)(o))
#   endif


//    #if !__OBJC2__
//
//        // The following declarations are provided here for source compatibility.
//
//        #if defined(__LP64__)
//            typedef long arith_t;
//            typedef unsigned long uarith_t;
//        #   define ARITH_SHIFT 32
//        #else
//            typedef int arith_t;
//            typedef unsigned uarith_t;
//        #   define ARITH_SHIFT 16
//        #endif
//
//        typedef char *STR;
//
//        #define ISSELECTOR(sel) sel_isMapped(sel)
//        #define SELNAME(sel)	sel_getName(sel)
//        #define SELUID(str)	sel_getUid(str)
//        #define NAMEOF(obj)     object_getClassName(obj)
//        #define IV(obj)         object_getIndexedIvars(obj)
//
//    #endif

#endif  /* _OBJC_OBJC_H_ */
