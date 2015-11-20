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

#ifndef objc_object_normal_isa_mm
#define objc_object_normal_isa_mm

#   if !SUPPORT_NONPOINTER_ISA

inline Class
objc_object::ISA()
{
    assert(!isTaggedPointer());
    return isa.cls;
}


inline bool
objc_object::hasIndexedIsa()
{
    return false;
}


inline Class
objc_object::getIsa()
{
#               if SUPPORT_TAGGED_POINTERS
    if (isTaggedPointer()) {
        uintptr_t slot = ((uintptr_t)this >> TAG_SLOT_SHIFT) & TAG_SLOT_MASK;
        return objc_tag_classes[slot];
    }
#               endif
    return ISA();
}


inline void
objc_object::initIsa(Class cls)
{
    assert(!isTaggedPointer());
    isa = (uintptr_t)cls;
}


inline void
objc_object::initClassIsa(Class cls)
{
    initIsa(cls);
}


inline void
objc_object::initProtocolIsa(Class cls)
{
    initIsa(cls);
}


inline void
objc_object::initInstanceIsa(Class cls, bool)
{
    initIsa(cls);
}


inline void
objc_object::initIsa(Class cls, bool, bool)
{
    initIsa(cls);
}


inline Class
objc_object::changeIsa(Class cls)
{
    assert(!isTaggedPointer());
    
    isa_t oldisa, newisa;
    newisa.cls = cls;
    do {
        oldisa = LoadExclusive(&isa.bits);
    } while (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits));
    
    if (oldisa.cls  &&  oldisa.cls->instancesHaveAssociatedObjects()) {
        cls->setInstancesHaveAssociatedObjects();
    }
    
    return oldisa.cls;
}


inline bool
objc_object::isTaggedPointer()
{
#               if SUPPORT_TAGGED_POINTERS
    return ((uintptr_t)this & TAG_MASK);
#               else
    return false;
#               endif
}


inline bool
objc_object::hasAssociatedObjects()
{
    assert(!UseGC);
    
    return getIsa()->instancesHaveAssociatedObjects();
}


inline void
objc_object::setHasAssociatedObjects()
{
    assert(!UseGC);
    
    getIsa()->setInstancesHaveAssociatedObjects();
}


inline bool
objc_object::isWeaklyReferenced()
{
    assert(!isTaggedPointer());
    assert(!UseGC);
    
    return sidetable_isWeaklyReferenced();
}


inline void
objc_object::setWeaklyReferenced_nolock()
{
    assert(!isTaggedPointer());
    assert(!UseGC);
    
    sidetable_setWeaklyReferenced_nolock();
}


inline bool
objc_object::hasCxxDtor()
{
    assert(!isTaggedPointer());
    return isa.cls->hasCxxDtor();
}


inline bool
objc_object::rootIsDeallocating()
{
    assert(!UseGC);
    
    if (isTaggedPointer()) return false;
    return sidetable_isDeallocating();
}


inline void
objc_object::clearDeallocating()
{
    sidetable_clearDeallocating();
}


inline void
objc_object::rootDealloc()
{
    if (isTaggedPointer()) return;
    object_dispose((id)this);
}


// Equivalent to calling [this retain], with shortcuts if there is no override
inline id
objc_object::retain()
{
    // UseGC is allowed here, but requires hasCustomRR.
    assert(!UseGC  ||  ISA()->hasCustomRR());
    assert(!isTaggedPointer());
    
    if (! ISA()->hasCustomRR()) {
        return sidetable_retain();
    }
    
    return ((id(*)(objc_object *, SEL))objc_msgSend)(this, SEL_retain);
}


// Base retain implementation, ignoring overrides.
// This does not check isa.fast_rr; if there is an RR override then
// it was already called and it chose to call [super retain].
inline id
objc_object::rootRetain()
{
    assert(!UseGC);
    
    if (isTaggedPointer()) return (id)this;
    return sidetable_retain();
}


// Equivalent to calling [this release], with shortcuts if there is no override
inline void
objc_object::release()
{
    // UseGC is allowed here, but requires hasCustomRR.
    assert(!UseGC  ||  ISA()->hasCustomRR());
    assert(!isTaggedPointer());
    
    if (! ISA()->hasCustomRR()) {
        sidetable_release();
        return;
    }
    
    ((void(*)(objc_object *, SEL))objc_msgSend)(this, SEL_release);
}


// Base release implementation, ignoring overrides.
// Does not call -dealloc.
// Returns true if the object should now be deallocated.
// This does not check isa.fast_rr; if there is an RR override then
// it was already called and it chose to call [super release].
inline bool
objc_object::rootRelease()
{
    assert(!UseGC);
    
    if (isTaggedPointer()) return false;
    return sidetable_release(true);
}

inline bool
objc_object::rootReleaseShouldDealloc()
{
    if (isTaggedPointer()) return false;
    return sidetable_release(false);
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


// Base tryRetain implementation, ignoring overrides.
// This does not check isa.fast_rr; if there is an RR override then
// it was already called and it chose to call [super _tryRetain].
inline bool
objc_object::rootTryRetain()
{
    assert(!UseGC);
    
    if (isTaggedPointer()) return true;
    return sidetable_tryRetain();
}


inline uintptr_t
objc_object::rootRetainCount()
{
    assert(!UseGC);
    
    if (isTaggedPointer()) return (uintptr_t)this;
    return sidetable_retainCount();
}

#   endif

#endif /* objc_object_private_mm */
