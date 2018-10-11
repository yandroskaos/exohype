#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

#include "video.h"

NTAPI
EFI_STATUS
VideoSelectMode(
	PVIDEO   video_)
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info = 0;
	UINTN                                 size = 0;
	EFI_STATUS                            status = EFI_SUCCESS;

	//
	// Initialize info of current mode
	//
	status = video_->protocol->QueryMode(video_->protocol, video_->modeId, &size, &info);
	if(EFI_ERROR(status))
		return status;

	RtlMemCpy(&video_->modeInfo, info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

	//
	// Look for a better mode
	//
	for(UINT32 i = 0; i < video_->protocol->Mode->MaxMode; ++i)
	{
		//
		// Find out the parameters of the mode we’re looking at
		//
		status = video_->protocol->QueryMode(video_->protocol, i, &size, &info);
		if(EFI_ERROR(status))
			return status;

		//
		// We only accept RGB or BGR 8 bit colorspaces.
		//
		if(info->PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
		   info->PixelFormat != PixelBlueGreenRedReserved8BitPerColor)
		{
			continue;
		}

		//
		// Check if it is the desired resolution.
		//
		if(info->VerticalResolution   == VIDEO_PREFERRED_HEIGHT &&
		   info->HorizontalResolution == VIDEO_PREFERRED_WIDTH)
		{
			RtlMemCpy(&video_->modeInfo, info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
			video_->modeId = i;
			break;
		}
	}

	return EFI_SUCCESS;
}

NTAPI
EFI_STATUS
VideoInit(
	EFI_SYSTEM_TABLE* systemTable_,
	PVIDEO            video_)
{
	EFI_GUID   graphicsProtocol = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_STATUS status           = EFI_SUCCESS;

	//
	// Get Video Services
	//
	status = systemTable_->BootServices->LocateProtocol(&graphicsProtocol, NULL, (void **)&video_->protocol);
	if(EFI_ERROR(status))
		return status;

	//
	// Select video mode starting from UEFI proposal
	//
	video_->modeId = video_->protocol->Mode->Mode;
	status = VideoSelectMode(video_);
	if(EFI_ERROR(status))
		return status;

	//
	// Set mode
	//
	status = video_->protocol->SetMode(video_->protocol, video_->modeId);
	if(EFI_ERROR(status))
		return status;

	//
	// Save framebuffer
	//
	video_->bufferBase = (UINT32*)video_->protocol->Mode->FrameBufferBase;
	video_->bufferSize = video_->protocol->Mode->FrameBufferSize;

	return EFI_SUCCESS;
}


NTAPI
VOID
VideoSetPixel(
	PVIDEO video_,
	UINTN  x_,
	UINTN  y_,
	UINT32 rgb_)
{
	video_->bufferBase[x_ + y_ * video_->modeInfo.PixelsPerScanLine] = rgb_;
}

NTAPI
VOID
VideoFillScreen(
	PVIDEO video_,
	UINT32 rgb_)
{
	rgb_ |= 0xff000000;

	for(UINTN x = 0; x < video_->modeInfo.HorizontalResolution; ++x)
	{
		for(UINTN y = 0; y < video_->modeInfo.VerticalResolution; ++y)
		{
			VideoSetPixel(video_, x, y, rgb_);
		}
	}
}

