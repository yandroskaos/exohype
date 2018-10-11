#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "rtl.h"

//#define VIDEO_PREFERRED_WIDTH  1920
//#define VIDEO_PREFERRED_HEIGHT 1080

#define VIDEO_PREFERRED_WIDTH  1024
#define VIDEO_PREFERRED_HEIGHT 768

typedef struct _VIDEO
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL*        protocol;

    UINT32                               modeId;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION modeInfo;

    UINT32*                              bufferBase;
    UINT64                               bufferSize;
} VIDEO, *PVIDEO;

NTAPI
EFI_STATUS
VideoInit(
	EFI_SYSTEM_TABLE* systemTable_,
	PVIDEO            video_);

NTAPI
VOID
VideoSetPixel(
	PVIDEO video_,
	UINTN  x_,
	UINTN  y_,
	UINT32 rgb_);

NTAPI
VOID
VideoFillScreen(
	PVIDEO video_,
	UINT32 rgb_);

#endif

