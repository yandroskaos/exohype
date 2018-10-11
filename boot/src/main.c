#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

#include <inttypes.h>

#include "rtl.h"
#include "memory.h"
#include "video.h"

MEMORY_MAP gMemoryMap = { 0 };
VIDEO      gVideo = { 0 };

EFI_STATUS
efi_main (
	EFI_HANDLE        imageHandle_,
	EFI_SYSTEM_TABLE* systemTable_)
{
	EFI_STATUS status = EFI_SUCCESS;

	InitializeLib(imageHandle_, systemTable_);

	Print(L"Starting...\n");

	Print(L"Detecting Memory Map...\n");
	status = MemoryInit(systemTable_, &gMemoryMap);
	if(EFI_ERROR(status))
	{
		Print(L"Failed to load memory map\n");
		Print(L"\n\n\nHit any key to exit this image\n");
		WaitForSingleEvent(ST->ConIn->WaitForKey, 0);
		return status;
        }
	
	for(UINTN i = 0; i < gMemoryMap.size; ++i)
	{
		Print(L"[%d]---------------------\n", i);
		Print(L"Type          = 0x%08X\n",         gMemoryMap.descriptors[i].Type);
		Print(L"PhysicalStart = 0x%" PRIx64 L"\n", gMemoryMap.descriptors[i].PhysicalStart);
		Print(L"VirtualStart  = 0x%" PRIx64 L"\n", gMemoryMap.descriptors[i].VirtualStart);
		Print(L"NumberOfPages = %" PRIu64 L"\n",   gMemoryMap.descriptors[i].NumberOfPages);
		Print(L"Attribute     = 0x%" PRIx64 L"\n", gMemoryMap.descriptors[i].Attribute);
	}

	Print(L"\n\n\nHit any key to exit this image\n");
	WaitForSingleEvent(ST->ConIn->WaitForKey, 0);


	Print(L"Setting Video Mode...\n");
	status = VideoInit(systemTable_, &gVideo);
	if(EFI_ERROR(status))
	{
		Print(L"Failed to init video\n");
		Print(L"\n\n\nHit any key to exit this image\n");
		WaitForSingleEvent(ST->ConIn->WaitForKey, 0);
		return status;
        }

	VideoFillScreen(&gVideo, 0x00FF0000);
	

	return EFI_SUCCESS;
}

