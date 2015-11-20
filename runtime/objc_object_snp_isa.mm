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

#ifndef objc_object_snp_isa_mm
#define objc_object_snp_isa_mm

#       if SUPPORT_NONPOINTER_ISA

#           if !SUPPORT_TAGGED_POINTERS
#               error sorry
#           endif


inline Class
objc_object::ISA()
{
    assert(!isTaggedPointer());
    return (Class)(isa.bits & ISA_MASK);
}


inline bool
objc_object::hasIndexedIsa()
{
    return isa.indexed;
}

inline Class
objc_object::getIsa()
{
    if (isTaggedPointer()) {
        uintptr_t slot = ((uintptr_t)this >> TAG_SLOT_SHIFT) & TAG_SLOT_MASK;
        return objc_tag_classes[slot];
    }
    return ISA();
}


inline void
objc_object::initIsa(Class cls)
{
    initIsa(cls, false, false);
}

inline void
objc_object::initClassIsa(Class cls)
{
    if (DisableIndexedIsa) {
        initIsa(cls, false, false);
    } else {
        initIsa(cls, true, false);
    }
}

inline void
objc_object::initProtocolIsa(Class cls)
{
    return initClassIsa(cls);
}

inline void
objc_object::initInstanceIsa(Class cls, bool hasCxxDtor)
{
    assert(!UseGC);
    assert(!cls->requiresRawIsa());
    assert(hasCxxDtor == cls->hasCxxDtor());
    
    initIsa(cls, true, hasCxxDtor);
}

inline void
objc_object::initIsa(Class cls, bool indexed, bool hasCxxDtor)
{
    assert(!isTaggedPointer());
    
    if (!indexed) {
        isa.cls = cls;
    } else {
        assert(!DisableIndexedIsa);
        isa.bits = ISA_MAGIC_VALUE;
        // isa.magic is part of ISA_MAGIC_VALUE
        // isa.indexed is part of ISA_MAGIC_VALUE
        isa.has_cxx_dtor = hasCxxDtor;
        isa.shiftcls = (uintptr_t)cls >> 3;
    }
}


inline Class
objc_object::changeIsa(Class newCls)
{
    assert(!isTaggedPointer());
    
    isa_t oldisa;
    isa_t newisa;
    
    bool sideTableLocked = false;
    bool transcribeToSideTable = false;
    
    do {
        transcribeToSideTable = false;
        oldisa = LoadExclusive(&isa.bits);
        if ((oldisa.bits == 0  ||  oldisa.indexed)  &&
            newCls->canAllocIndexed())
        {
            // 0 -> indexed
            // indexed -> indexed
            if (oldisa.bits == 0) newisa.bits = ISA_MAGIC_VALUE;
            else newisa = oldisa;
            // isa.magic is part of ISA_MAGIC_VALUE
            // isa.indexed is part of ISA_MAGIC_VALUE
            newisa.has_cxx_dtor = newCls->hasCxxDtor();
            newisa.shiftcls = (uintptr_t)newCls >> 3;
        }
        else if (oldisa.indexed) {
            // indexed -> not indexed
            // Need to copy retain count et al to side table.
            // Acquire side table lock before setting isa to
            // prevent races such as concurrent -release.
            if (!sideTableLocked) sidetable_lock();
            sideTableLocked = true;
            transcribeToSideTable = true;
            newisa.cls = newCls;
        }
        else {
            // not indexed -> not indexed
            newisa.cls = newCls;
        }
    } while (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits));
    
    if (transcribeToSideTable) {
        // Copy oldisa's retain count et al to side table.
        // oldisa.weakly_referenced: nothing to do
        // oldisa.has_assoc: nothing to do
        // oldisa.has_cxx_dtor: nothing to do
        sidetable_moveExtraRC_nolock(oldisa.extra_rc,
                                     oldisa.deallocating,
                                     oldisa.weakly_referenced);
    }
    
    if (sideTableLocked) sidetable_unlock();
    
    Class oldCls;
    if (oldisa.indexed) oldCls = (Class)((uintptr_t)oldisa.shiftcls << 3);
    else oldCls = oldisa.cls;
    
    return oldCls;
}


inline bool
objc_object::isTaggedPointer()
{
    return ((uintptr_t)this & TAG_MASK);
}


inline bool
objc_object::hasAssociatedObjects()
{
    if (isTaggedPointer()) return true;
    if (isa.indexed) return isa.has_assoc;
    return true;
}


inline void
objc_object::setHasAssociatedObjects()
{
    if (isTaggedPointer()) return;
    
retry:
    isa_t oldisa = LoadExclusive(&isa.bits);
    isa_t newisa = oldisa;
    if (!newisa.indexed) return;
    if (newisa.has_assoc) return;
    newisa.has_assoc = true;
    if (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits)) goto retry;
}


inline bool
objc_object::isWeaklyReferenced()
{
    assert(!isTaggedPointer());
    if (isa.indexed) return isa.weakly_referenced;
    else return sidetable_isWeaklyReferenced();
}


inline void
objc_object::setWeaklyReferenced_nolock()
{
retry:
    isa_t oldisa = LoadExclusive(&isa.bits);
    isa_t newisa = oldisa;
    if (!newisa.indexed) return sidetable_setWeaklyReferenced_nolock();
    if (newisa.weakly_referenced) return;
    newisa.weakly_referenced = true;
    if (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits)) goto retry;
}


inline bool
objc_object::hasCxxDtor()
{
    assert(!isTaggedPointer());
    if (isa.indexed) return isa.has_cxx_dtor;
    else return isa.cls->hasCxxDtor();
}



inline bool
objc_object::rootIsDeallocating()
{
    assert(!UseGC);
    
    if (isTaggedPointer()) return false;
    if (isa.indexed) return isa.deallocating;
    return sidetable_isDeallocating();
}


inline void
objc_object::clearDeallocating()
{
    if (!isa.indexed) {
        sidetable_clearDeallocating();
    }
    else if (isa.weakly_referenced) {
        clearDeallocating_weak();
    }
    
    assert(!sidetable_present());
}


inline void
objc_object::rootDealloc()
{
    assert(!UseGC);
    if (isTaggedPointer()) return;
    
    if (isa.indexed  &&
        !isa.weakly_referenced  &&
        !isa.has_assoc  &&
        !isa.has_cxx_dtor)
    {
        assert(!sidetable_present());
        free(this);
    }
    else {
        object_dispose((id)this);
    }
}


// Equivalent to calling [this retain], with shortcuts if there is no override
inline id
objc_object::retain()
{
    // UseGC is allowed here, but requires hasCustomRR.
    assert(!UseGC  ||  ISA()->hasCustomRR());
    assert(!isTaggedPointer());
    
    if (! ISA()->hasCustomRR()) {
        return rootRetain();
    }
    
    return ((id(*)(objc_object *, SEL))objc_msgSend)(this, SEL_retain);
}


// Base retain implementation, ignoring overrides.
// This does not check isa.fast_rr; if there is an RR override then
// it was already called and it chose to call [super retain].
//
// tryRetain=true is the -_tryRetain path.
// handleOverflow=false is the frameless fast path.
// handleOverflow=true is the framed slow path including overflow to side table
// The code is structured this way to prevent duplication.

ALWAYS_INLINE id
objc_object::rootRetain()
{
    return rootRetain(false, false);
}

ALWAYS_INLINE bool
objc_object::rootTryRetain()
{
    return rootRetain(true, false) ? true : false;
}

ALWAYS_INLINE id
objc_object::rootRetain(bool tryRetain, bool handleOverflow)
{
    assert(!UseGC);
    if (isTaggedPointer()) return (id)this;
    
    bool sideTableLocked = false;
    bool transcribeToSideTable = false;
    
    isa_t oldisa;
    isa_t newisa;
    
    do {
        transcribeToSideTable = false;
        oldisa = LoadExclusive(&isa.bits);
        newisa = oldisa;
        if (!newisa.indexed) goto unindexed;
        // don't check newisa.fast_rr; we already called any RR overrides
        if (tryRetain && newisa.deallocating) goto tryfail;
        uintptr_t carry;
        newisa.bits = addc(newisa.bits, RC_ONE, 0, &carry);  // extra_rc++
        
        if (carry) {
            // newisa.extra_rc++ overflowed
            if (!handleOverflow) return rootRetain_overflow(tryRetain);
            // Leave half of the retain counts inline and
            // prepare to copy the other half to the side table.
            if (!tryRetain && !sideTableLocked) sidetable_lock();
            sideTableLocked = true;
            transcribeToSideTable = true;
            newisa.extra_rc = RC_HALF;
            newisa.has_sidetable_rc = true;
        }
    } while (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits));
    
    if (transcribeToSideTable) {
        // Copy the other half of the retain counts to the side table.
        sidetable_addExtraRC_nolock(RC_HALF);
    }
    
    if (!tryRetain && sideTableLocked) sidetable_unlock();
    return (id)this;
    
tryfail:
    if (!tryRetain && sideTableLocked) sidetable_unlock();
    return nil;
    
unindexed:
    if (!tryRetain && sideTableLocked) sidetable_unlock();
    if (tryRetain) return sidetable_tryRetain() ? (id)this : nil;
    else return sidetable_retain();
}


// Equivalent to calling [this release], with shortcuts if there is no override
inline void
objc_object::release()
{
    // UseGC is allowed here, but requires hasCustomRR.
    assert(!UseGC  ||  ISA()->hasCustomRR());
    assert(!isTaggedPointer());
    
    if (! ISA()->hasCustomRR()) {
        rootRelease();
        return;
    }
    
    ((void(*)(objc_object *, SEL))objc_msgSend)(this, SEL_release);
}


// Base release implementation, ignoring overrides.
// Does not call -dealloc.
// Returns true if the object should now be deallocated.
// This does not check isa.fast_rr; if there is an RR override then
// it was already called and it chose to call [super release].
//
// handleUnderflow=false is the frameless fast path.
// handleUnderflow=true is the framed slow path including side table borrow
// The code is structured this way to prevent duplication.

ALWAYS_INLINE bool
objc_object::rootRelease()
{
    return rootRelease(true, false);
}

ALWAYS_INLINE bool
objc_object::rootReleaseShouldDealloc()
{
    return rootRelease(false, false);
}

ALWAYS_INLINE bool
objc_object::rootRelease(bool performDealloc, bool handleUnderflow)
{
    assert(!UseGC);
    if (isTaggedPointer()) return false;
    
    bool sideTableLocked = false;
    
    isa_t oldisa;
    isa_t newisa;
    
retry:
    do {
        oldisa = LoadExclusive(&isa.bits);
        newisa = oldisa;
        if (!newisa.indexed) goto unindexed;
        // don't check newisa.fast_rr; we already called any RR overrides
        uintptr_t carry;
        newisa.bits = subc(newisa.bits, RC_ONE, 0, &carry);  // extra_rc--
        if (carry) goto underflow;
    } while (!StoreReleaseExclusive(&isa.bits, oldisa.bits, newisa.bits));
    
    if (sideTableLocked) sidetable_unlock();
    return false;
    
underflow:
    // newisa.extra_rc-- underflowed: borrow from side table or deallocate
    
    // abandon newisa to undo the decrement
    newisa = oldisa;
    
    if (newisa.has_sidetable_rc) {
        if (!handleUnderflow) {
            return rootRelease_underflow(performDealloc);
        }
        // Add some retain counts inline and prepare
        // to remove them from the side table.
        if (!sideTableLocked) sidetable_lock();
        sideTableLocked = true;
        newisa.extra_rc = RC_HALF - 1;  // redo the decrement
        if (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits)) goto retry;
        
        // Remove the retain counts from the side table.
        bool zeroed = sidetable_subExtraRC_nolock(RC_HALF);
        if (zeroed) {
            // Side table count is now zero. Clear the marker bit.
            do {
                oldisa = LoadExclusive(&isa.bits);
                newisa.has_sidetable_rc = false;
            } while (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits));
        }
        
        // Decrement successful after borrowing from side table.
        // This decrement cannot be the deallocating decrement - the side
        // table lock and has_sidetable_rc bit ensure that if everyone
        // else tried to -release while we worked, the last one would block.
        sidetable_unlock();
        return false;
    }
    
    // Really deallocate.
    
    if (sideTableLocked) sidetable_unlock();
    
    if (newisa.deallocating) {
        return overrelease_error();
    }
    newisa.deallocating = true;
    if (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits)) goto retry;
    __sync_synchronize();
    if (performDealloc) {
        ((void(*)(objc_object *, SEL))objc_msgSend)(this, SEL_dealloc);
    }
    return true;
    
unindexed:
    if (sideTableLocked) sidetable_unlock();
    return sidetable_release(performDealloc);
}


// Equivalent to [this autorelease], with shortcuts if there is no override
inline id
objc_object::autorelease()
{
    // UseGC is allowed here, but requires hasCustomRR.
    assert(!UseGC  ||  ISA()->hasCustomRR());
    
    if (isTaggedPointer()) return (id)this;
    if (! ISA()->hasCustomRR()) return rootAutorelease();
    
    return ((id(*)(objc_object *, SEL))objc_msgSend)(this, SEL_autorelease);
}


// Base autorelease implementation, ignoring overrides.
inline id
objc_object::rootAutorelease()
{
    assert(!UseGC);
    
    if (isTaggedPointer()) return (id)this;
    if (fastAutoreleaseForReturn((id)this)) return (id)this;
    
    return rootAutorelease2();
}


inline uintptr_t
objc_object::rootRetainCount()
{
    assert(!UseGC);
    if (isTaggedPointer()) return (uintptr_t)this;
    
    sidetable_lock();
    isa_t bits = LoadExclusive(&isa.bits);
    if (bits.indexed) {
        uintptr_t rc = 1 + bits.extra_rc;
        if (bits.has_sidetable_rc) {
            rc += sidetable_getExtraRC_nolock();
        }
        sidetable_unlock();
        return rc;
    }
    
    sidetable_unlock();
    return sidetable_retainCount();
}

#endif

#endif /* objc_object_snp_isa_mm */
