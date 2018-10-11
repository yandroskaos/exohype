#ifndef __RTL_H__
#define __RTL_H__

#define NTAPI  __attribute__((ms_abi))
#define NAKED  __attribute__((naked))
#define INLINE __attribute__((always_inline))
#define PACKED __attribute__((packed))

NTAPI VOID   RtlMemCpy(VOID* dest_, CONST VOID* src_, UINTN size_);
NTAPI VOID   RtlMemSet(VOID* dest_, UINT8 v_,         UINTN size_);

#endif

