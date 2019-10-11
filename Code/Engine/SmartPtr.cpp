#include "stdafx.h"
#include "SmartPtr.h"

//-----------------------------------------------------------------------------
RefCounted::~RefCounted()
{
	if (refCount)
	{
		SE_ASSERT(refCount->refs == 0);
		if (refCount->weakRefs == 0)
			SafeDelete(refCount);
		else
			refCount->expired = true;
	}
}
//-----------------------------------------------------------------------------
void RefCounted::AddRef()
{
	if (!refCount)
		refCount = new RefCount();

	++(refCount->refs);
}
//-----------------------------------------------------------------------------
void RefCounted::ReleaseRef()
{
	SE_ASSERT(refCount && refCount->refs > 0);
	--(refCount->refs);
	if (refCount->refs == 0)
		delete this;
}
//-----------------------------------------------------------------------------
RefCount* RefCounted::RefCountPtr()
{
	if (!refCount)
		refCount = new RefCount();

	return refCount;
}
//-----------------------------------------------------------------------------