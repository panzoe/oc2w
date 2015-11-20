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

#ifndef objc_object_h
#define objc_object_h

#   import <cstddef>  // for nullptr_t

    union isa_t
    {
        isa_t() { }
        isa_t(uintptr_t value) : bits(value) { }
        
        Class cls;
        uintptr_t bits;
    
#       if SUPPORT_NONPOINTER_ISA
    
        // extra_rc must be the MSB-most field (so it matches carry/overflow flags)
        // indexed must be the LSB (fixme or get rid of it)
        // shiftcls must occupy the same bits that a real class pointer would
        // bits + RC_ONE is equivalent to extra_rc + 1
        // RC_HALF is the high bit of extra_rc (i.e. half of its range)
        
        // future expansion:
        // uintptr_t fast_rr : 1;     // no r/r overrides
        // uintptr_t lock : 2;        // lock for atomic property, @synch
        // uintptr_t extraBytes : 1;  // allocated with extra bytes

#           if __x86_64__
#               define ISA_MASK        0x00007ffffffffff8ULL
#               define ISA_MAGIC_MASK  0x0000000000000001ULL
#               define ISA_MAGIC_VALUE 0x0000000000000001ULL
        struct {
            uintptr_t indexed           : 1;
            uintptr_t has_assoc         : 1;
            uintptr_t has_cxx_dtor      : 1;
            uintptr_t shiftcls          : 44; // MACH_VM_MAX_ADDRESS 0x7fffffe00000
            uintptr_t weakly_referenced : 1;
            uintptr_t deallocating      : 1;
            uintptr_t has_sidetable_rc  : 1;
            uintptr_t extra_rc          : 14;
#               define RC_ONE   (1ULL<<50)
#               define RC_HALF  (1ULL<<13)
        };
    
#           else
                // Available bits in isa field are architecture-specific.
#               error unknown architecture
#           endif
    
            // SUPPORT_NONPOINTER_ISA
#       endif
    
    };

    struct objc_object {
        private:
            isa_t isa;
    
        public:
    
            // ISA() assumes this is NOT a tagged pointer object
            Class ISA();
            
            // getIsa() allows this to be a tagged pointer object
            Class getIsa();
            
            // initIsa() should be used to init the isa of new objects only.
            // If this object already has an isa, use changeIsa() for correctness.
            // initInstanceIsa(): objects with no custom RR/AWZ
            // initClassIsa(): class objects
            // initProtocolIsa(): protocol objects
            // initIsa(): other objects
            void initIsa(Class cls /*indexed=false*/);
            void initClassIsa(Class cls /*indexed=maybe*/);
            void initProtocolIsa(Class cls /*indexed=maybe*/);
            void initInstanceIsa(Class cls, bool hasCxxDtor);
            
            // changeIsa() should be used to change the isa of existing objects.
            // If this is a new object, use initIsa() for performance.
            Class changeIsa(Class newCls);
            
            bool hasIndexedIsa();
            bool isTaggedPointer();
            bool isClass();
            
            // object may have associated objects?
            bool hasAssociatedObjects();
            void setHasAssociatedObjects();
            
            // object may be weakly referenced?
            bool isWeaklyReferenced();
            void setWeaklyReferenced_nolock();
            
            // object may have -.cxx_destruct implementation?
            bool hasCxxDtor();
            
            // Optimized calls to retain/release methods
            id retain();
            void release();
            id autorelease();
            
            // Implementations of retain/release methods
            id rootRetain();
            bool rootRelease();
            id rootAutorelease();
            bool rootTryRetain();
            bool rootReleaseShouldDealloc();
            uintptr_t rootRetainCount();
            
            // Implementation of dealloc methods
            bool rootIsDeallocating();
            void clearDeallocating();
            void rootDealloc();
    
        private:
            void initIsa(Class newCls, bool indexed, bool hasCxxDtor);
            
            // Slow paths for inline control
            id rootAutorelease2();
            bool overrelease_error();
    
#           if SUPPORT_NONPOINTER_ISA
        
            // Unified retain count manipulation for nonpointer isa
            id rootRetain(bool tryRetain, bool handleOverflow);
            bool rootRelease(bool performDealloc, bool handleUnderflow);
            id rootRetain_overflow(bool tryRetain);
            bool rootRelease_underflow(bool performDealloc);
            
            void clearDeallocating_weak();
            
            // Side table retain count overflow for nonpointer isa
            void sidetable_lock();
            void sidetable_unlock();
            
            void sidetable_moveExtraRC_nolock(size_t extra_rc, bool isDeallocating, bool weaklyReferenced);
            bool sidetable_addExtraRC_nolock(size_t delta_rc);
            bool sidetable_subExtraRC_nolock(size_t delta_rc);
            size_t sidetable_getExtraRC_nolock();
    
#           endif
    
            // Side-table-only retain count
            bool sidetable_isDeallocating();
            void sidetable_clearDeallocating();
            
            bool sidetable_isWeaklyReferenced();
            void sidetable_setWeaklyReferenced_nolock();
            
            id sidetable_retain();
            id sidetable_retain_slow(SideTable *table);
            
            bool sidetable_release(bool performDealloc = true);
            bool sidetable_release_slow(SideTable *table, bool performDealloc = true);
            
            bool sidetable_tryRetain();
            
            uintptr_t sidetable_retainCount();
    
#           if !NDEBUG
        
            bool sidetable_present();
        
#           endif
    };

#   if SUPPORT_TAGGED_POINTERS

#       define TAG_COUNT 8
#       define TAG_SLOT_MASK 0xf

#       if SUPPORT_MSB_TAGGED_POINTERS
#           define TAG_MASK (1ULL<<63)
#           define TAG_SLOT_SHIFT 60
#           define TAG_PAYLOAD_LSHIFT 4
#           define TAG_PAYLOAD_RSHIFT 4
#       else
#           define TAG_MASK 1
#           define TAG_SLOT_SHIFT 0
#           define TAG_PAYLOAD_LSHIFT 0
#           define TAG_PAYLOAD_RSHIFT 4
#       endif

        extern "C" {
            extern Class objc_debug_taggedpointer_classes[TAG_COUNT*2];
        }

#       define objc_tag_classes objc_debug_taggedpointer_classes

#       endif

    static ALWAYS_INLINE bool fastAutoreleaseForReturn(id obj);
    static ALWAYS_INLINE bool fastRetainFromReturn(id obj);

#endif /* objc_object_h */
