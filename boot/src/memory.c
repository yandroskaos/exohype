#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

#include "memory.h"

NTAPI
EFI_STATUS
MemoryInit(
	EFI_SYSTEM_TABLE* systemTable_,
	PMEMORY_MAP       mmap_)
{
	EFI_STATUS status = EFI_SUCCESS;
	
	RtlMemSet(mmap_, 0, sizeof(MEMORY_MAP));

	status = systemTable_->BootServices->GetMemoryMap(&mmap_->size, mmap_->descriptors, &mmap_->mapKey, &mmap_->descriptorSize, &mmap_->descriptorVersion);

	mmap_->size += 2 * sizeof(EFI_MEMORY_DESCRIPTOR);

	status = systemTable_->BootServices->AllocatePool(EfiLoaderData, mmap_->size, &mmap_->descriptors);
	if(EFI_ERROR(status))
	{
		mmap_->descriptors = 0;
		return status;
        }

	status = systemTable_->BootServices->GetMemoryMap(&mmap_->size, mmap_->descriptors, &mmap_->mapKey, &mmap_->descriptorSize, &mmap_->descriptorVersion);
	if(EFI_ERROR(status))
		return status;

	mmap_->size /= mmap_->descriptorSize;

	return status;
}

