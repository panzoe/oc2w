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

#ifndef objc_object_autorelease_retval
#define objc_object_autorelease_retval

#if SUPPORT_RETURN_AUTORELEASE

    /***********************************************************************
     Fast handling of returned autoreleased values.
     The caller and callee cooperate to keep the returned object
     out of the autorelease pool.
     
     Caller:
     ret = callee();
     objc_retainAutoreleasedReturnValue(ret);
     // use ret here
     
     Callee:
     // compute ret
     [ret retain];
     return objc_autoreleaseReturnValue(ret);
     
     objc_autoreleaseReturnValue() examines the caller's instructions following
     the return. If the caller's instructions immediately call
     objc_autoreleaseReturnValue, then the callee omits the -autorelease and saves
     the result in thread-local storage. If the caller does not look like it
     cooperates, then the callee calls -autorelease as usual.
     
     objc_autoreleaseReturnValue checks if the returned value is the same as the
     one in thread-local storage. If it is, the value is used directly. If not,
     the value is assumed to be truly autoreleased and is retained again.  In
     either case, the caller now has a retained reference to the value.
     
     Tagged pointer objects do participate in the fast autorelease scheme,
     because it saves message sends. They are not entered in the autorelease
     pool in the slow case.
     **********************************************************************/

#   if __x86_64__

    static ALWAYS_INLINE bool
    callerAcceptsFastAutorelease(const void * const ra0)
    {
        const uint8_t *ra1 = (const uint8_t *)ra0;
        const uint16_t *ra2;
        const uint32_t *ra4 = (const uint32_t *)ra1;
        const void **sym;
    
#       define PREFER_GOTPCREL 0
#       if PREFER_GOTPCREL
        // 48 89 c7    movq  %rax,%rdi
        // ff 15       callq *symbol@GOTPCREL(%rip)
        if (*ra4 != 0xffc78948) {
            return false;
        }
        if (ra1[4] != 0x15) {
            return false;
        }
        ra1 += 3;
#       else
        // 48 89 c7    movq  %rax,%rdi
        // e8          callq symbol
        if (*ra4 != 0xe8c78948) {
            return false;
        }
        ra1 += (long)*(const int32_t *)(ra1 + 4) + 8l;
        ra2 = (const uint16_t *)ra1;
        // ff 25       jmpq *symbol@DYLDMAGIC(%rip)
        if (*ra2 != 0x25ff) {
            return false;
        }
#       endif
        
        ra1 += 6l + (long)*(const int32_t *)(ra1 + 2);
        sym = (const void **)ra1;
        if (*sym != objc_retainAutoreleasedReturnValue)
        {
            return false;
        }
        
        return true;
    }

    // __x86_64__ ===>>>end
#   elif __i386__  &&  TARGET_IPHONE_SIMULATOR

    static inline bool
    callerAcceptsFastAutorelease(const void *ra)
    {
        return false;
    }

    // __i386__  &&  TARGET_IPHONE_SIMULATOR ===>>>end
#   else

#   warning unknown architecture

    static ALWAYS_INLINE bool
    callerAcceptsFastAutorelease(const void *ra)
    {
        return false;
    }

    // unknown architecture ===>>>end
#   endif


    static ALWAYS_INLINE
    bool fastAutoreleaseForReturn(id obj)
    {
        assert(tls_get_direct(AUTORELEASE_POOL_RECLAIM_KEY) == nil);
        
        if (callerAcceptsFastAutorelease(__builtin_return_address(0))) {
            tls_set_direct(AUTORELEASE_POOL_RECLAIM_KEY, obj);
            return true;
        }
        
        return false;
    }


    static ALWAYS_INLINE
    bool fastRetainFromReturn(id obj)
    {
        if (obj == tls_get_direct(AUTORELEASE_POOL_RECLAIM_KEY)) {
            tls_set_direct(AUTORELEASE_POOL_RECLAIM_KEY, 0);
            return true;
        }
        
        return false;
    }


    // SUPPORT_RETURN_AUTORELEASE ===>>>end
#   else
    // not SUPPORT_RETURN_AUTORELEASE

    static ALWAYS_INLINE
    bool fastAutoreleaseForReturn(id obj)
    {
        return false;
    }


    static ALWAYS_INLINE
    bool fastRetainFromReturn(id obj)
    {
        return false;
    }

    // not SUPPORT_RETURN_AUTORELEASE ===>>>end
#   endif

#endif /* objc_object_autorelease_retval */
