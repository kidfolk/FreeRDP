/**
 * FreeRDP: A Remote Desktop Protocol Client
 * Windows Client
 *
 * Copyright 2009-2011 Jay Sorg
 * Copyright 2010-2011 Vic Lee
 * Copyright 2010-2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __WFREERDP_H
#define __WFREERDP_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>

#include <freerdp/freerdp.h>
#include <freerdp/gdi/gdi.h>
#include <freerdp/gdi/dc.h>
#include <freerdp/gdi/region.h>
#include <freerdp/cache/cache.h>
#include <freerdp/codec/color.h>
#include <freerdp/utils/debug.h>
#include <freerdp/chanman/chanman.h>

#include "wf_event.h"

#define SET_WFI(_instance, _wfi) (_instance)->client = _wfi
#define GET_WFI(_instance) ((wfInfo*) ((_instance)->client))

#define SET_CHANMAN(_instance, _chanman) (_instance)->chanman = _chanman
#define GET_CHANMAN(_instance) ((rdpChanMan*) ((_instance)->chanman))

struct wf_bitmap
{
	rdpBitmap _p;
	HDC hdc;
	HBITMAP bitmap;
	HBITMAP org_bitmap;
};
typedef struct wf_bitmap wfBitmap;

struct wf_info
{
	int fs_toggle;
	int fullscreen;
	int percentscreen;
	char window_title[64];

	HWND hwnd;
	HGDI_DC hdc;
	uint16 srcBpp;
	uint16 dstBpp;
	freerdp* instance;
	wfBitmap* primary;
	wfBitmap* drawing;
	HCLRCONV clrconv;
	HCURSOR cursor;
	HBRUSH brush;
	HBRUSH org_brush;

	boolean sw_gdi;
	rdpCache* cache;
};
typedef struct wf_info wfInfo;

#endif
