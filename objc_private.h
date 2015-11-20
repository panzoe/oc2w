/*
 * Copyright (c) 2012 Apple Inc.  All Rights Reserved.
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

#ifndef objc_private_h
#define objc_private_h

#   import "objc-config.h"

/* Isolate ourselves from the definitions of id and Class in the compiler
 * and public headers.
 */

#   ifdef _OBJC_OBJC_H_
#       error include objc-private.h before other headers
#   endif

#   define OBJC_TYPES_DEFINED 1
#   define OBJC_OLD_DISPATCH_PROTOTYPES 0

#   import <cstddef>  // for nullptr_t
#   import <stdint.h>
#   import <assert.h>

    struct objc_class;
    struct objc_object;

    typedef struct objc_class *Class;
    typedef struct objc_object *id;

    struct method_t;
    struct ivar_t;
    struct category_t;
    struct property_t;

    typedef struct method_t *Method;
    typedef struct ivar_t *Ivar;
    typedef struct category_t *Category;
    typedef struct property_t *objc_property_t;

    namespace {
        class SideTable;
    };

    // Public headers

#   import "objc.h"
#   import "runtime.h"
#   import "objc-os.h"
#   import "objc-abi.h"
#   import "objc-api.h"
#   import "objc-auto.h"
#   import "objc-config.h"
#   import "objc-internal.h"
#   import "maptable.h"
#   import "hashtable2.h"

    /* Do not include message.h here. */
    /* #include "message.h" */

#   define __APPLE_API_PRIVATE
#       include "objc-gdb.h"
#   undef __APPLE_API_PRIVATE

    // Private headers

#   import "objc-runtime-new.h"

#   import "objc-references.h"
#   import "objc-initialize.h"
#   import "objc-loadmethod.h"

    // Inlined parts of objc_object's implementation
#   import "objc-object.h"






#endif /* objc_private_h */
