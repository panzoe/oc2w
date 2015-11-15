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

#ifndef _OBJC_RUNTIME_H
#   define _OBJC_RUNTIME_H

#   include <objc/objc.h>
#   include <stdarg.h>
#   include <stdint.h>
#   include <stddef.h>
#   include <Availability.h>
#   include <TargetConditionals.h>

#   if TARGET_OS_MAC
#       include <sys/types.h>
#   endif


    /* Types */

#   if !OBJC_TYPES_DEFINED

        /// 定义新类型代表 在类中定义的方法。
        typedef struct objc_method *Method;

        /// 定义新类型代表 实例变量。
        typedef struct objc_ivar *Ivar;

        /// 定义新类型代表 分类(category)。
        typedef struct objc_category *Category;

        /// 定义新类型代表 Objective-C 声明的属性(property)。
        typedef struct objc_property *objc_property_t;

        /** 根据此声明，凡是首地址是 Class 的 struct 都可被认为是 Objective-C 中的对象 **/
        struct objc_class {
            Class isa  OBJC_ISA_AVAILABILITY;

//#           if !__OBJC2__
//                Class super_class                                        OBJC2_UNAVAILABLE;
//                const char *name                                         OBJC2_UNAVAILABLE;
//                long version                                             OBJC2_UNAVAILABLE;
//                long info                                                OBJC2_UNAVAILABLE;
//                long instance_size                                       OBJC2_UNAVAILABLE;
//                struct objc_ivar_list *ivars                             OBJC2_UNAVAILABLE;
//                struct objc_method_list **methodLists                    OBJC2_UNAVAILABLE;
//                struct objc_cache *cache                                 OBJC2_UNAVAILABLE;
//                struct objc_protocol_list *protocols                     OBJC2_UNAVAILABLE;
//#           endif

        } OBJC2_UNAVAILABLE;
        /* 使用 `Class` 代替 `struct objc_class *` */

#   endif

#   ifdef __OBJC__
        @class Protocol;
#   else
        typedef struct objc_object Protocol;
#   endif

    /// Defines a method
    struct objc_method_description {
        SEL name;               /**< The name of the method */
        char *types;            /**< The types of the method arguments */
    };

    /// Defines a property attribute
    typedef struct {
        const char *name;           /**< The name of the attribute */
        const char *value;          /**< The value of the attribute (usually empty) */
    } objc_property_attribute_t;


    /* 函式列表 */

    /* 与操作实例相关的函式 */

    /** 
     * 返回指定对象的一个复本
     * 
     * @param obj Objective-C 对象
     * @param size 对象 obj 的大小
     * 
     * @return obj 的复本
     */
    OBJC_EXPORT id object_copy(id obj, size_t size)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
        OBJC_ARC_UNAVAILABLE;

    /** 
     * 释放给定对象占据的记性体
     * 
     * @param obj Objective-C 对象
     * 
     * @return nil
     */
    OBJC_EXPORT id object_dispose(id obj)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
        OBJC_ARC_UNAVAILABLE;

    /** 
     * 获取某对象的类
     * 
     * @param obj 欲检查的对象
     * 
     * @return 返回该对象的类对象，若对象为 nil 则返回 Nil
     */
    OBJC_EXPORT Class object_getClass(id obj);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 设定指定对象的属类
     * 
     * @param obj 要修改的对象
     * @param cls 类对象
     * 
     * @return 该对象修改前的类对象，若对象为 nil 则返回 Nil
     */
    OBJC_EXPORT Class object_setClass(id obj, Class cls);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


    /** 
     * 判断某对象是否是类对象
     * 
     * @param obj Objective-C 对象
     * 
     * @return 如果该对象是 “类” 或者 “元类” 则返回 true ，否则返回 false
     */
    OBJC_EXPORT BOOL object_isClass(id obj);
//        __OSX_AVAILABLE_STARTING(__MAC_10_10, __IPHONE_8_0);


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
    OBJC_EXPORT void *object_getIndexedIvars(id obj)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
        OBJC_ARC_UNAVAILABLE;

    /** 
     * 读取对象的实例变量值
     * 
     * @param obj 包含该实例变量的对象
     * @param ivar 实例变量描述符
     * 
     * @return 目标实例变量值，若对象为 nil 则返回 nil
     * 
     * @note 在实例变量已知的情况下应使用 object_getIvar ，其处理速度要快于 object_getInstanceVariable
     */
    OBJC_EXPORT id object_getIvar(id obj, Ivar ivar);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 设置对象的实例变量
     * 
     * @param obj 包含该实例变量的对象
     * @param ivar 要设置的实例变量的描述符
     * @param value 实例变量的新值
     * 
     * @note 在实例变量已知的情况下应使用 object_setIvar ，其处理速度要快于 object_setInstanceVariable     
     */
    OBJC_EXPORT void object_setIvar(id obj, Ivar ivar, id value);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 修改类实例的实例变量值
     * 
     * @param obj 指向类实例的指针，该实例应包含你想要修改值的实例变量
     * @param name 一个 C 语言风格的字元串，用于描述你想要修改的实例变量的名称
     * @param value 该实例变量的新值
     * 
     * @return 一个指针 Ivar 数据结构的指针，该数据结构定义了 name 对应的实例变量的名称与类型
     */
    OBJC_EXPORT Ivar object_setInstanceVariable(id obj, const char *name, void *value)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
        OBJC_ARC_UNAVAILABLE;

    /** 
     * 获取类实例的实例变量值
     * 
     * @param obj 指向类实例的指针，该实例应包含你想要获取值的实例变量
     * @param name 一个 C 语言风格的字元串，用于描述你想要获取值的实例变量的名称
     * @param outValue 函数返回时，用于存放一个指向实例变量值的指针
     * 
     * @return 一个指针 Ivar 数据结构的指针，该数据结构定义了 name 对应的实例变量的名称与类型
     */
    OBJC_EXPORT Ivar object_getInstanceVariable(id obj, const char *name, void **outValue)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
        OBJC_ARC_UNAVAILABLE;


    /* 与获取 “类定义” 相关的函式 */

    /** 
     * 获取指定名的类
     * 
     * @param name 要查找的类的名称
     * 
     * @return 指名类的类对象，若该类并未注册于 Objective-C 运行时则返回 nil
     * 
     * @note objc_getClass 与 objc_lookUpClass 的工作机制是不同的，当在 “类” 未注册的情况下，
     *       objc_getClass 会请求 “类处理器回调”，然后二次检测类是否已注册，objc_lookUpClass 则不会这么做。
     * 
     * @warning 此函数的早期实现在类不存在时会终止整个应用程式
     */
    OBJC_EXPORT Class objc_getClass(const char *name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 获取指定类的 “元类”
     * 
     * @param name 要查找 “元类” 的 “类” 的名称
     * 
     * @return 指名类的 “元类” 类对象，若该类未注册于当时 Objective-C 运行时，则返回 nil
     * 
     * @note 如果该指名类定义并未注册，此函式会请求 “类处理器回调”，然后二次检测类是否已注册。
     *       其实所有类肯定都拥有一个有效的元类，总是函式会返回一个元类而无论其是否有效。
     */
    OBJC_EXPORT Class objc_getMetaClass(const char *name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 获取指定名的类
     * 
     * @param name 要查找的类的名称
     * 
     * @return 指名类的类对象，若该类并未注册于 Objective-C 运行时则返回 nil
     * 
     * @note objc_getClass 与 objc_lookUpClass 的工作机制是不同的，当在 “类” 未注册的情况下，
     *       objc_getClass 会请求 “类处理器回调”，然后二次检测类是否已注册，本函式则不会这么做。
     */
    OBJC_EXPORT Class objc_lookUpClass(const char *name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 获取指名类的类定义
     * 
     * @param name 要查找的类的名称
     * 
     * @return 指名类的类对象
     * 
     * @note 此函式与 objc_getClass 功能相同，不同之处在于当类不存在时将会 “杀死” 进程
     * @note This function is used by ZeroLink, where failing to find a class would be a compile-time link error without ZeroLink.
     */
    OBJC_EXPORT Class objc_getRequiredClass(const char *name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 获取已注册的 “类定义” 列表
     * 
     * @param buffer 一个 “类” 阵列。函式返回时阵列中每个成员都指向一个 “类定义”，
     *        阵列成员数量以 bufferCount 与 已注册 “类” 总数两者中较少者为准，
     *        如果只想单纯查询已注册 “类” 总数，传递 NULL 作为参数即可，函式将不会返回实际的 “类定义”
     * @param bufferCount 整数值。用于指示函式变量 buffer 指向的记性体内容可存放的指针数量
     *        函式返回时，只会向阵列中装入这么多数量的元素，如果该数值小于 “已注册类” 总数，
     *        那些函式返回的则会是 “已注册类” 的任意子集
     * 
     * @return 一个用于指示 “已注册类” 总数的整型值
     * 
     * @note Objective-C 运行时库会自行注册你源码中定义的所有 “类”，
     *       通过 objc_addClass 函式可以在运行时态创建出 “类定义” 并注册之
     * 
     * @warning 需注册的是，不可假设从此函式获取到的 “类对象” 都继承自 NSObject，
     *          所以在检测待调用方法已被实现前，任何方法调用都可能是不安全的
     */
    OBJC_EXPORT int objc_getClassList(Class *buffer, int bufferCount);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 创建并返回一个指向所有 “已注册类” 的 “类定义” 列表的指针
     * 
     * @param outCount 一个用于存放此函式返回的指针列表长度的整形指针，不可为 nil
     * 
     * @return 一个由 nil 结尾的类阵列。必须使用 c 标准库函式 free() 进行释放
     * 
     * @see objc_getClassList
     */
    OBJC_EXPORT Class *objc_copyClassList(unsigned int *outCount);
//         __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_3_1);


    /* 操作 “类” 相关的函式 */

    /** 
     * 返回指定 “类” 的名称
     * 
     * @param cls “类对象”
     * 
     * @return 类的名称，若 cls 为 Nil 则返回空字元串
     */
    OBJC_EXPORT const char *class_getName(Class cls);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 判断给定 “类对象” 是否是一个 “元类”
     * 
     * @param cls “类对象”
     * 
     * @return 如 cls 是一个 “元类” 则返回 YES ， 若 cls 不是 “元类”，或者为 Nil 则返回 NO
     */
    OBJC_EXPORT BOOL class_isMetaClass(Class cls);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 返回指定 “类” 的 “超类”
     * 
     * @param cls “类对象”
     * 
     * @return 指定 “类” 的 “超类”，如 cls 是 “根类” 或者为 Nil 则返回 Nil
     *
     * @note 通常情况下，应该使用 NSObject 的 superclass 方法来代替对此函式的调用
     */
    OBJC_EXPORT Class class_getSuperclass(Class cls);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 设置给定 “类” 的 “超类”
     * 
     * @param cls 欲更换 “超类” 的 “类”
     * @param newSuper cls 的新 “超类”
     * 
     * @return cls 的旧 “超类”
     * 
     * @warning 通常情况下，不应使用此函式
     */
    OBJC_EXPORT Class class_setSuperclass(Class cls, Class newSuper);
//         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);

    /** 
     * 返回某个 “类定义” 的版本号
     * 
     * @param cls 一个指向 Class 数据结构的指针。 用于传递想要获取版本号的 “类定义”
     * 
     * @return 用于指示 “类定义” 版本号的整数
     *
     * @see class_setVersion
     */
    OBJC_EXPORT int class_getVersion(Class cls);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 设定 “类定义” 的版本号
     * 
     * @param cls 一个指向 Class 数据结构的指针。 用于传递想要设定版本号的 “类定义”
     * @param version “类定义” 的新版本号
     *
     * @note 通过 “类定义” 的版本号，可以对你提供给 “其它类” 的 “类” 提供版本化的接口
     *       这在 “对象序列化” 时犹为有用（即，将对象以平面化格式进行归档），
     *       通过 “类定义” 版本号的不同，可意识到其实例变量的布局已发生了变更
     * @note 派生自 Foundation.framework 中 NSObject 的类
     *       可通过 setVersion: 类级方法设置 “类定义” 的版本号，该方法也是通过 class_setVersion 函式实现的
     */
    OBJC_EXPORT void class_setVersion(Class cls, int version);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 返回指定 “类” 的实例大小
     * 
     * @param cls “类对象“
     * 
     * @return 大小单位为 字节 ，若 cls 为 Nil 则返回 0
     */
    OBJC_EXPORT size_t class_getInstanceSize(Class cls);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 返回给定 ”类“ 的指定实例变量
     * 
     * @param cls 要获取实例变量的类
     * @param name 要获取实例变量的名称
     * 
     * @return 一个指向 Ivar 数据结构的指针，该结构包含了指名实例变量的信息
     */
    OBJC_EXPORT Ivar class_getInstanceVariable(Class cls, const char *name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 返回给定 ”类“ 的指定实例变量
     * 
     * @param cls 要获取实例变量的类
     * @param name 要获取实例变量的名称
     * 
     * @return 一个指向 Ivar 数据结构的指针，该结构包含了指名实例变量的信息
     */
    OBJC_EXPORT Ivar class_getClassVariable(Class cls, const char *name);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 描述某个 ”类“ 定义的所有实例变量
     * 
     * @param cls 欲检阅的 ”类“
     * @param 函式返回时 outCount 用于存放返回的阵列容量大小，如 outCount 为 NULL 则不返回阵列长度
     * 
     * @return 一个 Ivar 类型的指针阵列，该阵列描述了该 ”类“ 声明的所有实例变量，
     *         其中并不包含由 ”超类“ 声明的实例变量，阵列由一个 NULL 作为结尾，
     *         必须使用 free() 函式来手工释放阵列占用记性体
     * 
     *  如该 ”类“ 并未声明任何实例变量，或 cls 为 Nil，则函式返回值为 NULL 且 *outCount 的值为 0
     */
    OBJC_EXPORT Ivar *class_copyIvarList(Class cls, unsigned int *outCount);
//         __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 返回给定 ”类“ 的指定实例方法
     * 
     * @param cls 欲检阅的 ”类“
     * @param name 欲取回的方法的选择器
     * 
     * @return 在 ”类“ cls 中实现的匹配此选择器的方法，若该 ”类“ 或其 ”超类“ 均不存在匹配此选择器的方法则返回 NULL
     *
     * @note 此方法会尝试搜索其 ”超类“ 实现，而 class_copyMethodList 则不会
     */
    OBJC_EXPORT Method class_getInstanceMethod(Class cls, SEL name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 返回一个指向描述给定 ”类方法“ 的数据结构的指针
     * 
     * @param cls 一个指向 ”类定义“ 的指针。传递一个包含欲取回方法的 ”类“
     * @param name 一个 SEL 类型的指针。传递一个欲取回方法的选择器
     * 
     * @return 一个指向该 ”类“ 中与此选择器匹配的方法的 Method 数据结构指针，
     *         若该 ”类“ 或其 ”超类“ 均不存在匹配此选择器的方法则返回 NULL
     *
     * @note 请注意，此方法会尝试搜索其 ”超类“ 实现，而 class_copyMethodList 则不会
     */
    OBJC_EXPORT Method class_getClassMethod(Class cls, SEL name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * 返回一个向 ”类“ 实例发送消息时用于响应调用的函式指针
     * 
     * @param cls 欲检阅的 ”类“
     * @param name 选择器
     * 
     * @return 返回一个向 ”类“ 实例发送消息时用于响应调用的函式指针
     *         通常是用于响应对某个 ”类“ 的实例发送消息 [object name] 的请求，若 cls 为 NULL 则返回 Nil
     *
     * @note class_getMethodImplementation 的执行效率应该是快于 
     *       method_getImplementation(class_getInstanceMethod(cls, name)) 的形式的
     * @note 返回的函式指针有可能会是运行时内部函式，而不是一个实际的方法实现
     *       例如，当该 ”类“ 实例无法响应该选择器，返回的函式指针将会是 ”运行时消息转发机制“ 的一部分
     */
    OBJC_EXPORT IMP class_getMethodImplementation(Class cls, SEL name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 返回一个向 ”类“ 实例发送消息时用于响应调用的函式指针
     * 
     * @param cls 欲检阅的 ”类“
     * @param name 选择器
     * 
     * @return 返回一个向 ”类“ 实例发送消息时用于响应调用的函式指针
     *         通常是用于响应对某个 ”类“ 的实例发送消息 [object name] 的请求，若 cls 为 NULL 则返回 Nil
     */
    OBJC_EXPORT IMP class_getMethodImplementation_stret(Class cls, SEL name) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0)
         OBJC_ARM64_UNAVAILABLE;

    /** 
     * 判断指定的 ”类“ 实例是否能够响应特定的选择器
     * 
     * @param cls 欲检阅的 ”类“
     * @param sel 选择器
     * 
     * @return 若 ”类“ 实例能响应该选择器则返回 YES ，否则返回 NO
     * 
     * @note 通常情况下，应该优先使用 NSObject 的 respondsToSelector: 或 instancesRespondToSelector: 方法
     */
    OBJC_EXPORT BOOL class_respondsToSelector(Class cls, SEL sel);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 描述某个 ”类“ 所有的 已实现的实例方法
     * 
     * @param cls 欲检阅的 ”类“
     * @param outCount 函式返回时 outCount 用于存放返回的阵列容量大小，如 outCount 为 NULL 则不返回阵列长度
     * 
     * @return 一个指向由该 ”类“ 实现的所有实例方法指针阵列
     *         任何由 ”超类“ 声明的实例方法均未包含在其中，阵列由一个 NULL 作为结尾，
     *         必须使用 free() 函式来手工释放阵列占用记性体
     * 
     *  如该 ”类“ 并未声明任何实例方法，或 cls 为 Nil，则函式返回值为 NULL 且 *outCount 的值为 0
     * 
     * @note 如欲获取 ”类“ 方法，须使用 class_copyMethodList(object_getClass(cls), &count) 函式
     * @note 如需获取由 ”超类“ 已实现的 ”方法实现“ 则应使用 class_getInstanceMethod 或 class_getClassMethod 函式
     */
    OBJC_EXPORT Method *class_copyMethodList(Class cls, unsigned int *outCount);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 判断某个 ”类“ 是否符合给定的 ”协议“
     * 
     * @param cls 欲检阅的 ”类“
     * @param protocol ”协议“
     *
     * @return 则符合则返回 YES 否则返回 NO
     *
     * @note 通常应优先使用 NSObject 的 conformsToProtocol: 方法
     */
    OBJC_EXPORT BOOL class_conformsToProtocol(Class cls, Protocol *protocol);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 描述某个 ”类“ 采用的 ”协议“
     * 
     * @param cls 欲检阅的 ”类“
     * @param outCount 函式返回时 outCount 用于存放返回的阵列容量大小，如 outCount 为 NULL 则不返回阵列长度
     * 
     * @return 一个指向描述该类采用的所有 ”协议“ 的 Protocol* 型指针列表
     *         任何由 ”超类“ 采用或间接采用的 ”协议“ 均未包含在其中
     *         阵列由一个 NULL 作为结尾，必须使用 free() 函式来手工释放阵列占用记性体
     * 
     *  若 cls 为 Nil 或未采用任何 ”协议“，则返回 NULL 且 *outCount 值为 0
     */
    OBJC_EXPORT Protocol * __unsafe_unretained *class_copyProtocolList(Class cls, unsigned int *outCount);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 返回给定 ”类“ 的指名 ”属性“
     * 
     * @param cls 欲检阅的 ”类“
     * @param name 要获取的 ”属性“ 的名称
     * 
     * @return 一个用于描述 ”属性“ 的 objc_property_t 类型指针，
     *         若 cls 为 Nil ，或者该 ”类“ 未声明该指名 ”属性“ 则返回 NULL
     */
    OBJC_EXPORT objc_property_t class_getProperty(Class cls, const char *name);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 描述指定 ”类“ 声明的所有 ”属性“
     * 
     * @param cls 欲检阅的 ”类“
     * @param outCount 函式返回时 outCount 用于存放返回的阵列容量大小，如 outCount 为 NULL 则不返回阵列长度
     * 
     * @return 一个指向描述该 ”类“ 声明的所有 ”属性“ 的 objc_property_t 类型指针列表
     *         任何由 ”超类“ 声明的 ”属性“ 均未包含在其中，阵列由一个 NULL 作为结尾，
     *         必须使用 free() 函式来手工释放阵列占用记性体
     * 
     *  若 cls 为 Nil 或未声明任何 ”属性“，则返回 NULL 且 *outCount 值为 0
     */
    OBJC_EXPORT objc_property_t *class_copyPropertyList(Class cls, unsigned int *outCount);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 返回给定 ”类“ 的实例变量 布局描述
     * 
     * @param cls 欲检阅的 ”类“
     * 
     * @return ”类“ 的实例变量（Ivars） 布局描述
     */
    OBJC_EXPORT const uint8_t *class_getIvarLayout(Class cls);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 返回给定 ”类“ 的 ”弱实例变量“ 布局描述
     * 
     * @param cls 欲检阅的 ”类“
     * 
     * @return ”类“ 的 ”弱实例变量“（weak Ivars） 布局描述
     */
    OBJC_EXPORT const uint8_t *class_getWeakIvarLayout(Class cls);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 为 ”类“ 添加一个新的指名方法实现
     * 
     * @param cls 欲添加方法的 ”类“
     * @param name 要添加方法的指名选择器
     * @param imp 新方法的实现。此函式必须至少接受两个参数：self 及 _cmd
     * @param types 一个用于描述方法参数类型的字元阵列
     * 
     * @return 添加成功返回 YES，否则返回 NO （例如，该 ”类“ 已包含了指名相同的方法实现）
     *
     * @note class_addMethod 可以添加一个针对 ”超类“ 实现的重写，而不会替换此 ”类“ 当前已存在的实现
     *       如需改变现有的实现，请使用 method_setImplementation 函式
     */
    OBJC_EXPORT BOOL class_addMethod(Class cls, SEL name, IMP imp, const char *types);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 为 ”类“ 替换一个指名方法的实现
     * 
     * @param cls 欲替换方法的 ”类“
     * @param name 要替换其实现的指名方法选择器
     * @param imp 要替换方法的新实现
     * @param types 一个用于描述方法参数类型的字元阵列
     *  由于函式必须至少接受两个参数：self 及 _cmd，所以第二三位字元必然是 “@:” （第一个字元表示返回值类型）
     * 
     * @return 此指名方法的前一个实现
     * 
     * @note 引函式行为上有两点不同：
     *  - 如果该方法指名尚不存在，则会调用 class_addMethod 添加该方法实现，
     *    types 提供的类型编码会如期待地被使用
     *  - 如果该方法指名尚已存在，则会调用 method_setImplementation 替换方法实现，
     *    types 提供的类型编码会被忽略
     */
    OBJC_EXPORT IMP class_replaceMethod(Class cls, SEL name, IMP imp, const char *types);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 向 ”类“ 添加一个新的实例变量
     * 
     * @return 添加成功返回 YES，否则返回 NO （例如，”类“ 中已存在了同名的实例变量）
     *
     * @note 此函式只能在 后于 objc_allocateClassPair 且先于 objc_registerClassPair 的时期被调用
     *       向现有的 ”类“ 添加实例变量的作法是不支援的
     * @note 这个 ”类“ 不能是 ”元类“，向 ”元类“ 添加实例变量的行为也是不被支援的
     * @note The instance variable's minimum alignment in bytes is 1<<align. The minimum alignment of an instance 
     *       variable depends on the ivar's type and the machine architecture. 
     *       For variables of any pointer type, pass log2(sizeof(pointer_type)).
     */
    OBJC_EXPORT BOOL class_addIvar(Class cls, const char *name, size_t size, uint8_t alignment, const char *types);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 请求 ”类“ 追加采用一个新 ”协议“
     * 
     * @param cls 欲修改的 ”类“
     * @param protocol 要添加到 cls 的 ”协议“
     * 
     * @return 添加成功返回 YES，否则返回 NO （例如，该 ”类“ 已经采用了这个 ”协议“）
     */
    OBJC_EXPORT BOOL class_addProtocol(Class cls, Protocol *protocol);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 向类添加一个 ”属性“
     * 
     * @param cls 欲修改的 ”类“
     * @param name ”属性“ 的名称
     * @param attributes 该 ”属性“ 的所有特性列表
     * @param attributeCount 特性列表的总数
     * 
     * @return \c YES if the property was added successfully, otherwise \c NO
     *  (for example, the class already has that property).
     */
    OBJC_EXPORT BOOL class_addProperty(Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount);
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * 替换 ”类“ 的一个 ”属性“
     * 
     * @param cls 欲修改的 ”类“
     * @param name ”属性“ 的名称
     * @param attributes 该 ”属性“ 的所有特性列表
     * @param attributeCount 特性列表的总数
     */
    OBJC_EXPORT void class_replaceProperty(Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount);
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * 设置给定 ”类“ 的 ”实例变量布局“ (Ivar layout)
     * 
     * @param cls 欲修改的 ”类“
     * @param layout 目标布局值
     */
    OBJC_EXPORT void class_setIvarLayout(Class cls, const uint8_t *layout);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * 设置给定 ”类“ 的 ”弱实例变量布局“ (layout forweak Ivars)
     * 
     * @param cls 欲修改的 ”类“
     * @param layout 目标布局值
     */
    OBJC_EXPORT void class_setWeakIvarLayout(Class cls, const uint8_t *layout);
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /**
     * Return 返回指名 ”类“
     * 
     * @return The id of the named class, or an uninitialized class
     *  structure that will be used for the class when and if it does 
     *  get loaded.
     *
     * @note 此函式提供给 CoreFoundation 用来完成无缝桥接工作
     * 
     * @warning 请勿自行调用此函式
     */
    OBJC_EXPORT Class objc_getFutureClass(const char *name) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0)
         OBJC_ARC_UNAVAILABLE;

    /** 
     * 此函式提供给 CoreFoundation 用来完成无缝桥接工作
     * 
     * @warning 请勿自行调用此函式
     */
    OBJC_EXPORT void objc_setFutureClass(Class cls, const char *name) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0)
         OBJC_ARC_UNAVAILABLE;


    /* 实例化 ”类“ 相关的函式 */

    /** 
     * Creates an instance of a class, allocating memory for the class in the 
     * default malloc memory zone.
     * 
     * @param cls The class that you wish to allocate an instance of.
     * @param extraBytes An integer indicating the number of extra bytes to allocate. 
     *  The additional bytes can be used to store additional instance variables beyond 
     *  those defined in the class definition.
     * 
     * @return An instance of the class \e cls.
     */
    OBJC_EXPORT id class_createInstance(Class cls, size_t extraBytes)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
        OBJC_ARC_UNAVAILABLE;

    /** 
     * Creates an instance of a class at the specific location provided.
     * 
     * @param cls The class that you wish to allocate an instance of.
     * @param bytes The location at which to allocate an instance of \e cls.
     *  Must point to at least \c class_getInstanceSize(cls) bytes of well-aligned,
     *  zero-filled memory.
     *
     * @return \e bytes on success, \c nil otherwise. (For example, \e cls or \e bytes
     *  might be \c nil)
     *
     * @see class_createInstance
     */
    OBJC_EXPORT id objc_constructInstance(Class cls, void *bytes) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_0)
        OBJC_ARC_UNAVAILABLE;

    /** 
     * Destroys an instance of a class without freeing memory and removes any
     * associated references this instance might have had.
     * 
     * @param obj The class instance to destroy.
     * 
     * @return \e obj. Does nothing if \e obj is nil.
     * 
     * @warning GC does not call this. If you edit this, also edit finalize.
     *
     * @note CF and other clients do call this under GC.
     */
    OBJC_EXPORT void *objc_destructInstance(id obj) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_0)
        OBJC_ARC_UNAVAILABLE;


    /* Adding Classes */

    /** 
     * Creates a new class and metaclass.
     * 
     * @param superclass The class to use as the new class's superclass, or \c Nil to create a new root class.
     * @param name The string to use as the new class's name. The string will be copied.
     * @param extraBytes The number of bytes to allocate for indexed ivars at the end of 
     *  the class and metaclass objects. This should usually be \c 0.
     * 
     * @return The new class, or Nil if the class could not be created (for example, the desired name is already in use).
     * 
     * @note You can get a pointer to the new metaclass by calling \c object_getClass(newClass).
     * @note To create a new class, start by calling \c objc_allocateClassPair. 
     *  Then set the class's attributes with functions like \c class_addMethod and \c class_addIvar.
     *  When you are done building the class, call \c objc_registerClassPair. The new class is now ready for use.
     * @note Instance methods and instance variables should be added to the class itself. 
     *  Class methods should be added to the metaclass.
     */
    OBJC_EXPORT Class objc_allocateClassPair(Class superclass, const char *name, 
                                             size_t extraBytes) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Registers a class that was allocated using \c objc_allocateClassPair.
     * 
     * @param cls The class you want to register.
     */
    OBJC_EXPORT void objc_registerClassPair(Class cls) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Used by Foundation's Key-Value Observing.
     * 
     * @warning Do not call this function yourself.
     */
    OBJC_EXPORT Class objc_duplicateClass(Class original, const char *name, size_t extraBytes)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Destroy a class and its associated metaclass. 
     * 
     * @param cls The class to be destroyed. It must have been allocated with 
     *  \c objc_allocateClassPair
     * 
     * @warning Do not call if instances of this class or a subclass exist.
     */
    OBJC_EXPORT void objc_disposeClassPair(Class cls) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


    /* Working with Methods */

    /** 
     * Returns the name of a method.
     * 
     * @param m The method to inspect.
     * 
     * @return A pointer of type SEL.
     * 
     * @note To get the method name as a C string, call \c sel_getName(method_getName(method)).
     */
    OBJC_EXPORT SEL method_getName(Method m) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the implementation of a method.
     * 
     * @param m The method to inspect.
     * 
     * @return A function pointer of type IMP.
     */
    OBJC_EXPORT IMP method_getImplementation(Method m) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns a string describing a method's parameter and return types.
     * 
     * @param m The method to inspect.
     * 
     * @return A C string. The string may be \c NULL.
     */
    OBJC_EXPORT const char *method_getTypeEncoding(Method m) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the number of arguments accepted by a method.
     * 
     * @param m A pointer to a \c Method data structure. Pass the method in question.
     * 
     * @return An integer containing the number of arguments accepted by the given method.
     */
    OBJC_EXPORT unsigned int method_getNumberOfArguments(Method m)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * Returns a string describing a method's return type.
     * 
     * @param m The method to inspect.
     * 
     * @return A C string describing the return type. You must free the string with \c free().
     */
    OBJC_EXPORT char *method_copyReturnType(Method m) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns a string describing a single parameter type of a method.
     * 
     * @param m The method to inspect.
     * @param index The index of the parameter to inspect.
     * 
     * @return A C string describing the type of the parameter at index \e index, or \c NULL
     *  if method has no parameter index \e index. You must free the string with \c free().
     */
    OBJC_EXPORT char *method_copyArgumentType(Method m, unsigned int index) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns by reference a string describing a method's return type.
     * 
     * @param m The method you want to inquire about. 
     * @param dst The reference string to store the description.
     * @param dst_len The maximum number of characters that can be stored in \e dst.
     *
     * @note The method's return type string is copied to \e dst.
     *  \e dst is filled as if \c strncpy(dst, parameter_type, dst_len) were called.
     */
    OBJC_EXPORT void method_getReturnType(Method m, char *dst, size_t dst_len) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns by reference a string describing a single parameter type of a method.
     * 
     * @param m The method you want to inquire about. 
     * @param index The index of the parameter you want to inquire about.
     * @param dst The reference string to store the description.
     * @param dst_len The maximum number of characters that can be stored in \e dst.
     * 
     * @note The parameter type string is copied to \e dst. \e dst is filled as if \c strncpy(dst, parameter_type, dst_len) 
     *  were called. If the method contains no parameter with that index, \e dst is filled as
     *  if \c strncpy(dst, "", dst_len) were called.
     */
    OBJC_EXPORT void method_getArgumentType(Method m, unsigned int index, 
                                            char *dst, size_t dst_len) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);
    OBJC_EXPORT struct objc_method_description *method_getDescription(Method m) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Sets the implementation of a method.
     * 
     * @param m The method for which to set an implementation.
     * @param imp The implemention to set to this method.
     * 
     * @return The previous implementation of the method.
     */
    OBJC_EXPORT IMP method_setImplementation(Method m, IMP imp) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Exchanges the implementations of two methods.
     * 
     * @param m1 Method to exchange with second method.
     * @param m2 Method to exchange with first method.
     * 
     * @note This is an atomic version of the following:
     *  \code 
     *  IMP imp1 = method_getImplementation(m1);
     *  IMP imp2 = method_getImplementation(m2);
     *  method_setImplementation(m1, imp2);
     *  method_setImplementation(m2, imp1);
     *  \endcode
     */
    OBJC_EXPORT void method_exchangeImplementations(Method m1, Method m2) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


    /* Working with Instance Variables */

    /** 
     * Returns the name of an instance variable.
     * 
     * @param v The instance variable you want to enquire about.
     * 
     * @return A C string containing the instance variable's name.
     */
    OBJC_EXPORT const char *ivar_getName(Ivar v) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the type string of an instance variable.
     * 
     * @param v The instance variable you want to enquire about.
     * 
     * @return A C string containing the instance variable's type encoding.
     *
     * @note For possible values, see Objective-C Runtime Programming Guide > Type Encodings.
     */
    OBJC_EXPORT const char *ivar_getTypeEncoding(Ivar v) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the offset of an instance variable.
     * 
     * @param v The instance variable you want to enquire about.
     * 
     * @return The offset of \e v.
     * 
     * @note For instance variables of type \c id or other object types, call \c object_getIvar
     *  and \c object_setIvar instead of using this offset to access the instance variable data directly.
     */
    OBJC_EXPORT ptrdiff_t ivar_getOffset(Ivar v) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


    /* Working with Properties */

    /** 
     * Returns the name of a property.
     * 
     * @param property The property you want to inquire about.
     * 
     * @return A C string containing the property's name.
     */
    OBJC_EXPORT const char *property_getName(objc_property_t property) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the attribute string of a property.
     * 
     * @param property A property.
     *
     * @return A C string containing the property's attributes.
     * 
     * @note The format of the attribute string is described in Declared Properties in Objective-C Runtime Programming Guide.
     */
    OBJC_EXPORT const char *property_getAttributes(objc_property_t property) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns an array of property attributes for a property. 
     * 
     * @param property The property whose attributes you want copied.
     * @param outCount The number of attributes returned in the array.
     * 
     * @return An array of property attributes; must be free'd() by the caller. 
     */
    OBJC_EXPORT objc_property_attribute_t *property_copyAttributeList(objc_property_t property, unsigned int *outCount)
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * Returns the value of a property attribute given the attribute name.
     * 
     * @param property The property whose attribute value you are interested in.
     * @param attributeName C string representing the attribute name.
     *
     * @return The value string of the attribute \e attributeName if it exists in
     *  \e property, \c nil otherwise. 
     */
    OBJC_EXPORT char *property_copyAttributeValue(objc_property_t property, const char *attributeName)
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);


    /* Working with Protocols */

    /** 
     * Returns a specified protocol.
     * 
     * @param name The name of a protocol.
     * 
     * @return The protocol named \e name, or \c NULL if no protocol named \e name could be found.
     * 
     * @note This function acquires the runtime lock.
     */
    OBJC_EXPORT Protocol *objc_getProtocol(const char *name)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns an array of all the protocols known to the runtime.
     * 
     * @param outCount Upon return, contains the number of protocols in the returned array.
     * 
     * @return A C array of all the protocols known to the runtime. The array contains \c *outCount
     *  pointers followed by a \c NULL terminator. You must free the list with \c free().
     * 
     * @note This function acquires the runtime lock.
     */
    OBJC_EXPORT Protocol * __unsafe_unretained *objc_copyProtocolList(unsigned int *outCount)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns a Boolean value that indicates whether one protocol conforms to another protocol.
     * 
     * @param proto A protocol.
     * @param other A protocol.
     * 
     * @return \c YES if \e proto conforms to \e other, otherwise \c NO.
     * 
     * @note One protocol can incorporate other protocols using the same syntax 
     *  that classes use to adopt a protocol:
     *  \code
     *  @protocol ProtocolName < protocol list >
     *  \endcode
     *  All the protocols listed between angle brackets are considered part of the ProtocolName protocol.
     */
    OBJC_EXPORT BOOL protocol_conformsToProtocol(Protocol *proto, Protocol *other)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns a Boolean value that indicates whether two protocols are equal.
     * 
     * @param proto A protocol.
     * @param other A protocol.
     * 
     * @return \c YES if \e proto is the same as \e other, otherwise \c NO.
     */
    OBJC_EXPORT BOOL protocol_isEqual(Protocol *proto, Protocol *other)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the name of a protocol.
     * 
     * @param p A protocol.
     * 
     * @return The name of the protocol \e p as a C string.
     */
    OBJC_EXPORT const char *protocol_getName(Protocol *p)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns a method description structure for a specified method of a given protocol.
     * 
     * @param p A protocol.
     * @param aSel A selector.
     * @param isRequiredMethod A Boolean value that indicates whether aSel is a required method.
     * @param isInstanceMethod A Boolean value that indicates whether aSel is an instance method.
     * 
     * @return An \c objc_method_description structure that describes the method specified by \e aSel,
     *  \e isRequiredMethod, and \e isInstanceMethod for the protocol \e p.
     *  If the protocol does not contain the specified method, returns an \c objc_method_description structure
     *  with the value \c {NULL, \c NULL}.
     * 
     * @note This function recursively searches any protocols that this protocol conforms to.
     */
    OBJC_EXPORT struct objc_method_description protocol_getMethodDescription(Protocol *p, SEL aSel, BOOL isRequiredMethod, BOOL isInstanceMethod)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns an array of method descriptions of methods meeting a given specification for a given protocol.
     * 
     * @param p A protocol.
     * @param isRequiredMethod A Boolean value that indicates whether returned methods should
     *  be required methods (pass YES to specify required methods).
     * @param isInstanceMethod A Boolean value that indicates whether returned methods should
     *  be instance methods (pass YES to specify instance methods).
     * @param outCount Upon return, contains the number of method description structures in the returned array.
     * 
     * @return A C array of \c objc_method_description structures containing the names and types of \e p's methods 
     *  specified by \e isRequiredMethod and \e isInstanceMethod. The array contains \c *outCount pointers followed
     *  by a \c NULL terminator. You must free the list with \c free().
     *  If the protocol declares no methods that meet the specification, \c NULL is returned and \c *outCount is 0.
     * 
     * @note Methods in other protocols adopted by this protocol are not included.
     */
    OBJC_EXPORT struct objc_method_description *protocol_copyMethodDescriptionList(Protocol *p, BOOL isRequiredMethod, BOOL isInstanceMethod, unsigned int *outCount)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the specified property of a given protocol.
     * 
     * @param proto A protocol.
     * @param name The name of a property.
     * @param isRequiredProperty A Boolean value that indicates whether name is a required property.
     * @param isInstanceProperty A Boolean value that indicates whether name is a required property.
     * 
     * @return The property specified by \e name, \e isRequiredProperty, and \e isInstanceProperty for \e proto,
     *  or \c NULL if none of \e proto's properties meets the specification.
     */
    OBJC_EXPORT objc_property_t protocol_getProperty(Protocol *proto, const char *name, BOOL isRequiredProperty, BOOL isInstanceProperty)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns an array of the properties declared by a protocol.
     * 
     * @param proto A protocol.
     * @param outCount Upon return, contains the number of elements in the returned array.
     * 
     * @return A C array of pointers of type \c objc_property_t describing the properties declared by \e proto.
     *  Any properties declared by other protocols adopted by this protocol are not included. The array contains
     *  \c *outCount pointers followed by a \c NULL terminator. You must free the array with \c free().
     *  If the protocol declares no properties, \c NULL is returned and \c *outCount is \c 0.
     */
    OBJC_EXPORT objc_property_t *protocol_copyPropertyList(Protocol *proto, unsigned int *outCount)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns an array of the protocols adopted by a protocol.
     * 
     * @param proto A protocol.
     * @param outCount Upon return, contains the number of elements in the returned array.
     * 
     * @return A C array of protocols adopted by \e proto. The array contains \e *outCount pointers
     *  followed by a \c NULL terminator. You must free the array with \c free().
     *  If the protocol declares no properties, \c NULL is returned and \c *outCount is \c 0.
     */
    OBJC_EXPORT Protocol * __unsafe_unretained *protocol_copyProtocolList(Protocol *proto, unsigned int *outCount)
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Creates a new protocol instance that cannot be used until registered with
     * \c objc_registerProtocol()
     * 
     * @param name The name of the protocol to create.
     *
     * @return The Protocol instance on success, \c nil if a protocol
     *  with the same name already exists. 
     * @note There is no dispose method for this. 
     */
    OBJC_EXPORT Protocol *objc_allocateProtocol(const char *name) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * Registers a newly constructed protocol with the runtime. The protocol
     * will be ready for use and is immutable after this.
     * 
     * @param proto The protocol you want to register.
     */
    OBJC_EXPORT void objc_registerProtocol(Protocol *proto) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * Adds a method to a protocol. The protocol must be under construction.
     * 
     * @param proto The protocol to add a method to.
     * @param name The name of the method to add.
     * @param types A C string that represents the method signature.
     * @param isRequiredMethod YES if the method is not an optional method.
     * @param isInstanceMethod YES if the method is an instance method. 
     */
    OBJC_EXPORT void protocol_addMethodDescription(Protocol *proto, SEL name, const char *types, BOOL isRequiredMethod, BOOL isInstanceMethod) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * Adds an incorporated protocol to another protocol. The protocol being
     * added to must still be under construction, while the additional protocol
     * must be already constructed.
     * 
     * @param proto The protocol you want to add to, it must be under construction.
     * @param addition The protocol you want to incorporate into \e proto, it must be registered.
     */
    OBJC_EXPORT void protocol_addProtocol(Protocol *proto, Protocol *addition) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * Adds a property to a protocol. The protocol must be under construction. 
     * 
     * @param proto The protocol to add a property to.
     * @param name The name of the property.
     * @param attributes An array of property attributes.
     * @param attributeCount The number of attributes in \e attributes.
     * @param isRequiredProperty YES if the property (accessor methods) is not optional. 
     * @param isInstanceProperty YES if the property (accessor methods) are instance methods. 
     *  This is the only case allowed fo a property, as a result, setting this to NO will 
     *  not add the property to the protocol at all. 
     */
    OBJC_EXPORT void protocol_addProperty(Protocol *proto, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount, BOOL isRequiredProperty, BOOL isInstanceProperty)
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);


    /* Working with Libraries */

    /** 
     * Returns the names of all the loaded Objective-C frameworks and dynamic
     * libraries.
     * 
     * @param outCount The number of names returned.
     * 
     * @return An array of C strings of names. Must be free()'d by caller.
     */
    OBJC_EXPORT const char **objc_copyImageNames(unsigned int *outCount) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the dynamic library name a class originated from.
     * 
     * @param cls The class you are inquiring about.
     * 
     * @return The name of the library containing this class.
     */
    OBJC_EXPORT const char *class_getImageName(Class cls) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Returns the names of all the classes within a library.
     * 
     * @param image The library or framework you are inquiring about.
     * @param outCount The number of class names returned.
     * 
     * @return An array of C strings representing the class names.
     */
    OBJC_EXPORT const char **objc_copyClassNamesForImage(const char *image, 
                                                         unsigned int *outCount) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


    /* Working with Selectors */

    /** 
     * Returns the name of the method specified by a given selector.
     * 
     * @param sel A pointer of type \c SEL. Pass the selector whose name you wish to determine.
     * 
     * @return A C string indicating the name of the selector.
     */
    OBJC_EXPORT const char *sel_getName(SEL sel)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * Registers a method name with the Objective-C runtime system.
     * 
     * @param str A pointer to a C string. Pass the name of the method you wish to register.
     * 
     * @return A pointer of type SEL specifying the selector for the named method.
     * 
     * @note The implementation of this method is identical to the implementation of \c sel_registerName.
     * @note Prior to OS X version 10.0, this method tried to find the selector mapped to the given name
     *  and returned \c NULL if the selector was not found. This was changed for safety, because it was
     *  observed that many of the callers of this function did not check the return value for \c NULL.
     */
    OBJC_EXPORT SEL sel_getUid(const char *str)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * Registers a method with the Objective-C runtime system, maps the method 
     * name to a selector, and returns the selector value.
     * 
     * @param str A pointer to a C string. Pass the name of the method you wish to register.
     * 
     * @return A pointer of type SEL specifying the selector for the named method.
     * 
     * @note You must register a method name with the Objective-C runtime system to obtain the
     *  method’s selector before you can add the method to a class definition. If the method name
     *  has already been registered, this function simply returns the selector.
     */
    OBJC_EXPORT SEL sel_registerName(const char *str)
//        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

    /** 
     * Returns a Boolean value that indicates whether two selectors are equal.
     * 
     * @param lhs The selector to compare with rhs.
     * @param rhs The selector to compare with lhs.
     * 
     * @return \c YES if \e rhs and \e rhs are equal, otherwise \c NO.
     * 
     * @note sel_isEqual is equivalent to ==.
     */
    OBJC_EXPORT BOOL sel_isEqual(SEL lhs, SEL rhs) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


    /* Objective-C Language Features */

    /** 
     * This function is inserted by the compiler when a mutation
     * is detected during a foreach iteration. It gets called 
     * when a mutation occurs, and the enumerationMutationHandler
     * is enacted if it is set up. A fatal error occurs if a handler is not set up.
     *
     * @param obj The object being mutated.
     * 
     */
    OBJC_EXPORT void objc_enumerationMutation(id obj) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Sets the current mutation handler. 
     * 
     * @param handler Function pointer to the new mutation handler.
     */
    OBJC_EXPORT void objc_setEnumerationMutationHandler(void (*handler)(id)) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Set the function to be called by objc_msgForward.
     * 
     * @param fwd Function to be jumped to by objc_msgForward.
     * @param fwd_stret Function to be jumped to by objc_msgForward_stret.
     * 
     * @see message.h::_objc_msgForward
     */
    OBJC_EXPORT void objc_setForwardHandler(void *fwd, void *fwd_stret) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

    /** 
     * Creates a pointer to a function that will call the block
     * when the method is called.
     * 
     * @param block The block that implements this method. Its signature should
     *  be: method_return_type ^(id self, method_args...). 
     *  The selector is not available as a parameter to this block.
     *  The block is copied with \c Block_copy().
     * 
     * @return The IMP that calls this block. Must be disposed of with
     *  \c imp_removeBlock.
     */
    OBJC_EXPORT IMP imp_implementationWithBlock(id block)
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * Return the block associated with an IMP that was created using
     * \c imp_implementationWithBlock.
     * 
     * @param anImp The IMP that calls this block.
     * 
     * @return The block called by \e anImp.
     */
    OBJC_EXPORT id imp_getBlock(IMP anImp)
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * Disassociates a block from an IMP that was created using
     * \c imp_implementationWithBlock and releases the copy of the 
     * block that was created.
     * 
     * @param anImp An IMP that was created using \c imp_implementationWithBlock.
     * 
     * @return YES if the block was released successfully, NO otherwise. 
     *  (For example, the block might not have been used to create an IMP previously).
     */
    OBJC_EXPORT BOOL imp_removeBlock(IMP anImp)
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

    /** 
     * This loads the object referenced by a weak pointer and returns it, after
     * retaining and autoreleasing the object to ensure that it stays alive
     * long enough for the caller to use it. This function would be used
     * anywhere a __weak variable is used in an expression.
     * 
     * @param location The weak pointer address
     * 
     * @return The object pointed to by \e location, or \c nil if \e location is \c nil.
     */
    OBJC_EXPORT id objc_loadWeak(id *location)
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_5_0);

    /** 
     * This function stores a new value into a __weak variable. It would
     * be used anywhere a __weak variable is the target of an assignment.
     * 
     * @param location The address of the weak pointer itself
     * @param obj The new object this weak ptr should now point to
     * 
     * @return The value stored into \e location, i.e. \e obj
     */
    OBJC_EXPORT id objc_storeWeak(id *location, id obj) 
//        __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_5_0);


    /* Associative References */

    /**
     * Policies related to associative references.
     * These are options to objc_setAssociatedObject()
     */
    enum {
        OBJC_ASSOCIATION_ASSIGN = 0,           /**< Specifies a weak reference to the associated object. */
        OBJC_ASSOCIATION_RETAIN_NONATOMIC = 1, /**< Specifies a strong reference to the associated object. 
                                                *   The association is not made atomically. */
        OBJC_ASSOCIATION_COPY_NONATOMIC = 3,   /**< Specifies that the associated object is copied. 
                                                *   The association is not made atomically. */
        OBJC_ASSOCIATION_RETAIN = 01401,       /**< Specifies a strong reference to the associated object.
                                                *   The association is made atomically. */
        OBJC_ASSOCIATION_COPY = 01403          /**< Specifies that the associated object is copied.
                                                *   The association is made atomically. */
    };

    /// Type to specify the behavior of an association.
    typedef uintptr_t objc_AssociationPolicy;

    /** 
     * Sets an associated value for a given object using a given key and association policy.
     * 
     * @param object The source object for the association.
     * @param key The key for the association.
     * @param value The value to associate with the key key for object. Pass nil to clear an existing association.
     * @param policy The policy for the association. For possible values, see “Associative Object Behaviors.”
     * 
     * @see objc_setAssociatedObject
     * @see objc_removeAssociatedObjects
     */
    OBJC_EXPORT void objc_setAssociatedObject(id object, const void *key, id value, objc_AssociationPolicy policy)
//        __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_1);

    /** 
     * Returns the value associated with a given object for a given key.
     * 
     * @param object The source object for the association.
     * @param key The key for the association.
     * 
     * @return The value associated with the key \e key for \e object.
     * 
     * @see objc_setAssociatedObject
     */
    OBJC_EXPORT id objc_getAssociatedObject(id object, const void *key)
//        __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_1);

    /** 
     * Removes all associations for a given object.
     * 
     * @param object An object that maintains associated objects.
     * 
     * @note The main purpose of this function is to make it easy to return an object 
     *  to a "pristine state”. You should not use this function for general removal of
     *  associations from objects, since it also removes associations that other clients
     *  may have added to the object. Typically you should use \c objc_setAssociatedObject 
     *  with a nil value to clear an association.
     * 
     * @see objc_setAssociatedObject
     * @see objc_getAssociatedObject
     */
    OBJC_EXPORT void objc_removeAssociatedObjects(id object)
//        __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_1);


#   define _C_ID       '@'
#   define _C_CLASS    '#'
#   define _C_SEL      ':'
#   define _C_CHR      'c'
#   define _C_UCHR     'C'
#   define _C_SHT      's'
#   define _C_USHT     'S'
#   define _C_INT      'i'
#   define _C_UINT     'I'
#   define _C_LNG      'l'
#   define _C_ULNG     'L'
#   define _C_LNG_LNG  'q'
#   define _C_ULNG_LNG 'Q'
#   define _C_FLT      'f'
#   define _C_DBL      'd'
#   define _C_BFLD     'b'
#   define _C_BOOL     'B'
#   define _C_VOID     'v'
#   define _C_UNDEF    '?'
#   define _C_PTR      '^'
#   define _C_CHARPTR  '*'
#   define _C_ATOM     '%'
#   define _C_ARY_B    '['
#   define _C_ARY_E    ']'
#   define _C_UNION_B  '('
#   define _C_UNION_E  ')'
#   define _C_STRUCT_B '{'
#   define _C_STRUCT_E '}'
#   define _C_VECTOR   '!'
#   define _C_CONST    'r'


    /* 过时的类型 */

//#   if !__OBJC2__
//
//#       define CLS_GETINFO(cls,infomask)        ((cls)->info & (infomask))
//#       define CLS_SETINFO(cls,infomask)        ((cls)->info |= (infomask))
//
//        // class is not a metaclass
//#       define CLS_CLASS               0x1
//        // class is a metaclass
//#       define CLS_META                0x2
//        // class's +initialize method has completed
//#       define CLS_INITIALIZED         0x4
//        // class is posing
//#       define CLS_POSING              0x8
//        // unused
//#       define CLS_MAPPED              0x10
//        // class and subclasses need cache flush during image loading
//#       define CLS_FLUSH_CACHE         0x20
//        // method cache should grow when full
//#       define CLS_GROW_CACHE          0x40
//        // unused
//#       define CLS_NEED_BIND           0x80
//        // methodLists is array of method lists
//#       define CLS_METHOD_ARRAY        0x100
//        // the JavaBridge constructs classes with these markers
//#       define CLS_JAVA_HYBRID         0x200
//#       define CLS_JAVA_CLASS          0x400
//        // thread-safe +initialize
//#       define CLS_INITIALIZING        0x800
//        // bundle unloading
//#       define CLS_FROM_BUNDLE         0x1000
//        // C++ ivar support
//#       define CLS_HAS_CXX_STRUCTORS   0x2000
//        // Lazy method list arrays
//#       define CLS_NO_METHOD_ARRAY     0x4000
//        // +load implementation
//#       define CLS_HAS_LOAD_METHOD     0x8000
//        // objc_allocateClassPair API
//#       define CLS_CONSTRUCTING        0x10000
//        // class compiled with bigger class structure
//#       define CLS_EXT                 0x20000
//
//
//        struct objc_method_description_list {
//                int count;
//                struct objc_method_description list[1];
//        };
//
//
//        struct objc_protocol_list {
//            struct objc_protocol_list *next;
//            long count;
//            Protocol *list[1];
//        };
//
//
//        struct objc_category {
//            char *category_name                                      OBJC2_UNAVAILABLE;
//            char *class_name                                         OBJC2_UNAVAILABLE;
//            struct objc_method_list *instance_methods                OBJC2_UNAVAILABLE;
//            struct objc_method_list *class_methods                   OBJC2_UNAVAILABLE;
//            struct objc_protocol_list *protocols                     OBJC2_UNAVAILABLE;
//        }                                                            OBJC2_UNAVAILABLE;
//
//
//        struct objc_ivar {
//            char *ivar_name                                          OBJC2_UNAVAILABLE;
//            char *ivar_type                                          OBJC2_UNAVAILABLE;
//            int ivar_offset                                          OBJC2_UNAVAILABLE;
//#           ifdef __LP64__
//                int space                                            OBJC2_UNAVAILABLE;
//#           endif
//        }                                                            OBJC2_UNAVAILABLE;
//
//        struct objc_ivar_list {
//            int ivar_count                                           OBJC2_UNAVAILABLE;
//#           ifdef __LP64__
//                int space                                            OBJC2_UNAVAILABLE;
//#           endif
//            /* variable length structure */
//            struct objc_ivar ivar_list[1]                            OBJC2_UNAVAILABLE;
//        }                                                            OBJC2_UNAVAILABLE;
//
//
//        struct objc_method {
//            SEL method_name                                          OBJC2_UNAVAILABLE;
//            char *method_types                                       OBJC2_UNAVAILABLE;
//            IMP method_imp                                           OBJC2_UNAVAILABLE;
//        }                                                            OBJC2_UNAVAILABLE;
//
//        struct objc_method_list {
//            struct objc_method_list *obsolete                        OBJC2_UNAVAILABLE;
//
//            int method_count                                         OBJC2_UNAVAILABLE;
//#           ifdef __LP64__
//                int space                                            OBJC2_UNAVAILABLE;
//#           endif
//            /* variable length structure */
//            struct objc_method method_list[1]                        OBJC2_UNAVAILABLE;
//        }                                                            OBJC2_UNAVAILABLE;
//
//
//        typedef struct objc_symtab *Symtab                           OBJC2_UNAVAILABLE;
//
//        struct objc_symtab {
//            unsigned long sel_ref_cnt                                OBJC2_UNAVAILABLE;
//            SEL *refs                                                OBJC2_UNAVAILABLE;
//            unsigned short cls_def_cnt                               OBJC2_UNAVAILABLE;
//            unsigned short cat_def_cnt                               OBJC2_UNAVAILABLE;
//            void *defs[1] /* variable size */                        OBJC2_UNAVAILABLE;
//        }                                                            OBJC2_UNAVAILABLE;
//
//
//        typedef struct objc_cache *Cache                             OBJC2_UNAVAILABLE;
//
//#       define CACHE_BUCKET_NAME(B)  ((B)->method_name)
//#       define CACHE_BUCKET_IMP(B)   ((B)->method_imp)
//#       define CACHE_BUCKET_VALID(B) (B)
//#       ifndef __LP64__
//#           define CACHE_HASH(sel, mask) (((uintptr_t)(sel)>>2) & (mask))
//#       else
//#           define CACHE_HASH(sel, mask) (((unsigned int)((uintptr_t)(sel)>>3)) & (mask))
//#       endif
//        struct objc_cache {
//            unsigned int mask /* total = mask + 1 */                 OBJC2_UNAVAILABLE;
//            unsigned int occupied                                    OBJC2_UNAVAILABLE;
//            Method buckets[1]                                        OBJC2_UNAVAILABLE;
//        };
//
//
//        typedef struct objc_module *Module                           OBJC2_UNAVAILABLE;
//
//        struct objc_module {
//            unsigned long version                                    OBJC2_UNAVAILABLE;
//            unsigned long size                                       OBJC2_UNAVAILABLE;
//            const char *name                                         OBJC2_UNAVAILABLE;
//            Symtab symtab                                            OBJC2_UNAVAILABLE;
//        }                                                            OBJC2_UNAVAILABLE;
//
//#   else

        struct objc_method_list;

//#   endif


    /* 过时的函式 */

    OBJC_EXPORT IMP class_lookupMethod(Class cls, SEL sel);
//        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);
    OBJC_EXPORT BOOL class_respondsToMethod(Class cls, SEL sel);
//        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);
    OBJC_EXPORT void _objc_flush_caches(Class cls);
//        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);

    OBJC_EXPORT id object_copyFromZone(id anObject, size_t nBytes, void *z) 
//        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_NA,__IPHONE_NA)
        OBJC_ARC_UNAVAILABLE;
    OBJC_EXPORT id object_realloc(id anObject, size_t nBytes)    OBJC2_UNAVAILABLE;
    OBJC_EXPORT id object_reallocFromZone(id anObject, size_t nBytes, void *z) OBJC2_UNAVAILABLE;

#   define OBSOLETE_OBJC_GETCLASSES 1

    OBJC_EXPORT void *objc_getClasses(void)                      OBJC2_UNAVAILABLE;
    OBJC_EXPORT void objc_addClass(Class myClass)                OBJC2_UNAVAILABLE;
    OBJC_EXPORT void objc_setClassHandler(int (*)(const char *)) OBJC2_UNAVAILABLE;
    OBJC_EXPORT void objc_setMultithreaded (BOOL flag)           OBJC2_UNAVAILABLE;

    OBJC_EXPORT id class_createInstanceFromZone(Class, size_t idxIvars, void *z)  
//        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_NA,__IPHONE_NA)
        OBJC_ARC_UNAVAILABLE;

    OBJC_EXPORT void class_addMethods(Class, struct objc_method_list *) OBJC2_UNAVAILABLE;
    OBJC_EXPORT void class_removeMethods(Class, struct objc_method_list *) OBJC2_UNAVAILABLE;
    OBJC_EXPORT void _objc_resolve_categories_for_class(Class cls)  OBJC2_UNAVAILABLE;

    OBJC_EXPORT Class class_poseAs(Class imposter, Class original) OBJC2_UNAVAILABLE;

    OBJC_EXPORT unsigned int method_getSizeOfArguments(Method m) OBJC2_UNAVAILABLE;
    OBJC_EXPORT unsigned method_getArgumentInfo(struct objc_method *m, int arg, const char **type, int *offset) OBJC2_UNAVAILABLE;

    OBJC_EXPORT Class objc_getOrigClass(const char *name)        OBJC2_UNAVAILABLE;

#   define OBJC_NEXT_METHOD_LIST 1

    OBJC_EXPORT struct objc_method_list *class_nextMethodList(Class, void **) OBJC2_UNAVAILABLE;
    // nextMethodList 的用法
    //
    // void *iterator = 0;
    // struct objc_method_list *mlist;
    // while ( mlist = class_nextMethodList( cls, &iterator ) ) {
    //     ;
    // }
     
    OBJC_EXPORT id (*_alloc)(Class, size_t)                      OBJC2_UNAVAILABLE;
    OBJC_EXPORT id (*_copy)(id, size_t)                          OBJC2_UNAVAILABLE;
    OBJC_EXPORT id (*_realloc)(id, size_t)                       OBJC2_UNAVAILABLE;
    OBJC_EXPORT id (*_dealloc)(id)                               OBJC2_UNAVAILABLE;
    OBJC_EXPORT id (*_zoneAlloc)(Class, size_t, void *)          OBJC2_UNAVAILABLE;
    OBJC_EXPORT id (*_zoneRealloc)(id, size_t, void *)           OBJC2_UNAVAILABLE;
    OBJC_EXPORT id (*_zoneCopy)(id, size_t, void *)              OBJC2_UNAVAILABLE;
    OBJC_EXPORT void (*_error)(id, const char *, va_list)        OBJC2_UNAVAILABLE;

#endif
