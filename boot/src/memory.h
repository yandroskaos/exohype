#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <efi.h>

#include "rtl.h"

typedef struct _MEMORY_MAP
{
    EFI_MEMORY_DESCRIPTOR* descriptors;
    UINTN                  size;
    UINTN                  mapKey;
    UINTN                  descriptorSize;
    UINT32                 descriptorVersion;
} MEMORY_MAP, *PMEMORY_MAP;

NTAPI
EFI_STATUS
MemoryInit(
	EFI_SYSTEM_TABLE* systemTable_,
	PMEMORY_MAP       mmap_);

#endif

