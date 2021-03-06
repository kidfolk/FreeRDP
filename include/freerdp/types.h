/**
 * FreeRDP: A Remote Desktop Protocol Client
 * Type Definitions
 *
 * Copyright 2009-2011 Jay Sorg
 * Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
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

#ifndef __RDP_TYPES_H
#define __RDP_TYPES_H

#ifdef _WIN32
#include <WinDef.h>
#endif

/* Base Types */

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned int uint32;
typedef signed int sint32;
#ifdef _WIN32
typedef unsigned __int64 uint64;
typedef signed __int64 sint64;
#else
typedef unsigned long long uint64;
typedef signed long long sint64;
#endif

#ifndef True
#define True  (1)
#endif

#ifndef False
#define False (0)
#endif

#ifndef _WIN32
typedef int boolean;
#else
#define boolean BOOLEAN
#endif

#ifndef MIN
#define MIN(x,y)	(((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y)	(((x) > (y)) ? (x) : (y))
#endif

#include <freerdp/settings.h>

struct _RDP_PALETTEENTRY
{
	uint8 red;
	uint8 green;
	uint8 blue;
};
typedef struct _RDP_PALETTEENTRY RDP_PALETTEENTRY;

struct _RDP_PALETTE
{
	uint16 count;
	RDP_PALETTEENTRY* entries;
};
typedef struct _RDP_PALETTE RDP_PALETTE;

struct _RDP_PLUGIN_DATA
{
	uint16 size;
	void* data[4];
};
typedef struct _RDP_PLUGIN_DATA RDP_PLUGIN_DATA;

struct _RDP_RECT
{
	sint16 x;
	sint16 y;
	sint16 width;
	sint16 height;
};
typedef struct _RDP_RECT RDP_RECT;

/* Plugin events */
typedef struct _RDP_EVENT RDP_EVENT;

typedef void (*RDP_EVENT_CALLBACK) (RDP_EVENT* event);

struct _RDP_EVENT
{
	uint16 event_class;
	uint16 event_type;
	RDP_EVENT_CALLBACK on_event_free_callback;
	void* user_data;
};

enum RDP_EVENT_CLASS
{
	RDP_EVENT_CLASS_DEBUG = 0,
	RDP_EVENT_CLASS_CLIPRDR,
	RDP_EVENT_CLASS_TSMF,
	RDP_EVENT_CLASS_RAIL
};

#endif /* __RDP_TYPES_H */
