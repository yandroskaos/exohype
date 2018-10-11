#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

#include "rtl.h"

NTAPI
VOID
RtlMemCpy(
	VOID*       dest_, 
	CONST VOID* src_,
	UINTN       size_)
{
	UINT8* d   = (UINT8*)dest_;
	UINT8* s   = (UINT8*)src_;
	UINT8* end = s + size_;

	while(s != end)
		*d++ = *s++;
}

NTAPI
VOID
RtlMemSet(
	VOID* dest_,
	UINT8 v_,
	UINTN size_)
{
	UINT8* d   = (UINT8*)dest_;
	UINT8* end = d + size_;

	while(d != end)
		*d++ = v_;
}

