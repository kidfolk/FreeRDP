/**
 * FreeRDP: A Remote Desktop Protocol Client
 * GDI Library
 *
 * Copyright 2010-2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <freerdp/api.h>
#include <freerdp/freerdp.h>
#include <freerdp/constants.h>
#include <freerdp/utils/bitmap.h>
#include <freerdp/codec/color.h>
#include <freerdp/codec/bitmap.h>
#include <freerdp/codec/rfx.h>
#include <freerdp/codec/nsc.h>

#include <freerdp/gdi/dc.h>
#include <freerdp/gdi/pen.h>
#include <freerdp/gdi/line.h>
#include <freerdp/gdi/shape.h>
#include <freerdp/gdi/brush.h>
#include <freerdp/gdi/region.h>
#include <freerdp/gdi/bitmap.h>
#include <freerdp/gdi/palette.h>
#include <freerdp/gdi/drawing.h>
#include <freerdp/gdi/clipping.h>

#include <freerdp/gdi/gdi.h>

/* Ternary Raster Operation Table */
const uint32 rop3_code_table[] =
{
	0x00000042, /* 0 */
	0x00010289, /* DPSoon */
	0x00020C89, /* DPSona */
	0x000300AA, /* PSon */
	0x00040C88, /* SDPona */
	0x000500A9, /* DPon */
	0x00060865, /* PDSxnon */
	0x000702C5, /* PDSaon */
	0x00080F08, /* SDPnaa */
	0x00090245, /* PDSxon */
	0x000A0329, /* DPna */
	0x000B0B2A, /* PSDnaon */
	0x000C0324, /* SPna */
	0x000D0B25, /* PDSnaon */
	0x000E08A5, /* PDSonon */
	0x000F0001, /* Pn */
	0x00100C85, /* PDSona */
	0x001100A6, /* DSon */
	0x00120868, /* SDPxnon */
	0x001302C8, /* SDPaon */
	0x00140869, /* DPSxnon */
	0x001502C9, /* DPSaon */
	0x00165CCA, /* PSDPSanaxx */
	0x00171D54, /* SSPxDSxaxn */
	0x00180D59, /* SPxPDxa */
	0x00191CC8, /* SDPSanaxn */
	0x001A06C5, /* PDSPaox */
	0x001B0768, /* SDPSxaxn */
	0x001C06CA, /* PSDPaox */
	0x001D0766, /* DSPDxaxn */
	0x001E01A5, /* PDSox */
	0x001F0385, /* PDSoan */
	0x00200F09, /* DPSnaa */
	0x00210248, /* SDPxon */
	0x00220326, /* DSna */
	0x00230B24, /* SPDnaon */
	0x00240D55, /* SPxDSxa */
	0x00251CC5, /* PDSPanaxn */
	0x002606C8, /* SDPSaox */
	0x00271868, /* SDPSxnox */
	0x00280369, /* DPSxa */
	0x002916CA, /* PSDPSaoxxn */
	0x002A0CC9, /* DPSana */
	0x002B1D58, /* SSPxPDxaxn */
	0x002C0784, /* SPDSoax */
	0x002D060A, /* PSDnox */
	0x002E064A, /* PSDPxox */
	0x002F0E2A, /* PSDnoan */
	0x0030032A, /* PSna */
	0x00310B28, /* SDPnaon */
	0x00320688, /* SDPSoox */
	0x00330008, /* Sn */
	0x003406C4, /* SPDSaox */
	0x00351864, /* SPDSxnox */
	0x003601A8, /* SDPox */
	0x00370388, /* SDPoan */
	0x0038078A, /* PSDPoax */
	0x00390604, /* SPDnox */
	0x003A0644, /* SPDSxox */
	0x003B0E24, /* SPDnoan */
	0x003C004A, /* PSx */
	0x003D18A4, /* SPDSonox */
	0x003E1B24, /* SPDSnaox */
	0x003F00EA, /* PSan */
	0x00400F0A, /* PSDnaa */
	0x00410249, /* DPSxon */
	0x00420D5D, /* SDxPDxa */
	0x00431CC4, /* SPDSanaxn */
	0x00440328, /* SDna */
	0x00450B29, /* DPSnaon */
	0x004606C6, /* DSPDaox */
	0x0047076A, /* PSDPxaxn */
	0x00480368, /* SDPxa */
	0x004916C5, /* PDSPDaoxxn */
	0x004A0789, /* DPSDoax */
	0x004B0605, /* PDSnox */
	0x004C0CC8, /* SDPana */
	0x004D1954, /* SSPxDSxoxn */
	0x004E0645, /* PDSPxox */
	0x004F0E25, /* PDSnoan */
	0x00500325, /* PDna */
	0x00510B26, /* DSPnaon */
	0x005206C9, /* DPSDaox */
	0x00530764, /* SPDSxaxn */
	0x005408A9, /* DPSonon */
	0x00550009, /* Dn */
	0x005601A9, /* DPSox */
	0x00570389, /* DPSoan */
	0x00580785, /* PDSPoax */
	0x00590609, /* DPSnox */
	0x005A0049, /* DPx */
	0x005B18A9, /* DPSDonox */
	0x005C0649, /* DPSDxox */
	0x005D0E29, /* DPSnoan */
	0x005E1B29, /* DPSDnaox */
	0x005F00E9, /* DPan */
	0x00600365, /* PDSxa */
	0x006116C6, /* DSPDSaoxxn */
	0x00620786, /* DSPDoax */
	0x00630608, /* SDPnox */
	0x00640788, /* SDPSoax */
	0x00650606, /* DSPnox */
	0x00660046, /* DSx */
	0x006718A8, /* SDPSonox */
	0x006858A6, /* DSPDSonoxxn */
	0x00690145, /* PDSxxn */
	0x006A01E9, /* DPSax */
	0x006B178A, /* PSDPSoaxxn */
	0x006C01E8, /* SDPax */
	0x006D1785, /* PDSPDoaxxn */
	0x006E1E28, /* SDPSnoax */
	0x006F0C65, /* PDSxnan */
	0x00700CC5, /* PDSana */
	0x00711D5C, /* SSDxPDxaxn */
	0x00720648, /* SDPSxox */
	0x00730E28, /* SDPnoan */
	0x00740646, /* DSPDxox */
	0x00750E26, /* DSPnoan */
	0x00761B28, /* SDPSnaox */
	0x007700E6, /* DSan */
	0x007801E5, /* PDSax */
	0x00791786, /* DSPDSoaxxn */
	0x007A1E29, /* DPSDnoax */
	0x007B0C68, /* SDPxnan */
	0x007C1E24, /* SPDSnoax */
	0x007D0C69, /* DPSxnan */
	0x007E0955, /* SPxDSxo */
	0x007F03C9, /* DPSaan */
	0x008003E9, /* DPSaa */
	0x00810975, /* SPxDSxon */
	0x00820C49, /* DPSxna */
	0x00831E04, /* SPDSnoaxn */
	0x00840C48, /* SDPxna */
	0x00851E05, /* PDSPnoaxn */
	0x008617A6, /* DSPDSoaxx */
	0x008701C5, /* PDSaxn */
	0x008800C6, /* DSa */
	0x00891B08, /* SDPSnaoxn */
	0x008A0E06, /* DSPnoa */
	0x008B0666, /* DSPDxoxn */
	0x008C0E08, /* SDPnoa */
	0x008D0668, /* SDPSxoxn */
	0x008E1D7C, /* SSDxPDxax */
	0x008F0CE5, /* PDSanan */
	0x00900C45, /* PDSxna */
	0x00911E08, /* SDPSnoaxn */
	0x009217A9, /* DPSDPoaxx */
	0x009301C4, /* SPDaxn */
	0x009417AA, /* PSDPSoaxx */
	0x009501C9, /* DPSaxn */
	0x00960169, /* DPSxx */
	0x0097588A, /* PSDPSonoxx */
	0x00981888, /* SDPSonoxn */
	0x00990066, /* DSxn */
	0x009A0709, /* DPSnax */
	0x009B07A8, /* SDPSoaxn */
	0x009C0704, /* SPDnax */
	0x009D07A6, /* DSPDoaxn */
	0x009E16E6, /* DSPDSaoxx */
	0x009F0345, /* PDSxan */
	0x00A000C9, /* DPa */
	0x00A11B05, /* PDSPnaoxn */
	0x00A20E09, /* DPSnoa */
	0x00A30669, /* DPSDxoxn */
	0x00A41885, /* PDSPonoxn */
	0x00A50065, /* PDxn */
	0x00A60706, /* DSPnax */
	0x00A707A5, /* PDSPoaxn */
	0x00A803A9, /* DPSoa */
	0x00A90189, /* DPSoxn */
	0x00AA0029, /* D */
	0x00AB0889, /* DPSono */
	0x00AC0744, /* SPDSxax */
	0x00AD06E9, /* DPSDaoxn */
	0x00AE0B06, /* DSPnao */
	0x00AF0229, /* DPno */
	0x00B00E05, /* PDSnoa */
	0x00B10665, /* PDSPxoxn */
	0x00B21974, /* SSPxDSxox */
	0x00B30CE8, /* SDPanan */
	0x00B4070A, /* PSDnax */
	0x00B507A9, /* DPSDoaxn */
	0x00B616E9, /* DPSDPaoxx */
	0x00B70348, /* SDPxan */
	0x00B8074A, /* PSDPxax */
	0x00B906E6, /* DSPDaoxn */
	0x00BA0B09, /* DPSnao */
	0x00BB0226, /* DSno */
	0x00BC1CE4, /* SPDSanax */
	0x00BD0D7D, /* SDxPDxan */
	0x00BE0269, /* DPSxo */
	0x00BF08C9, /* DPSano */
	0x00C000CA, /* PSa */
	0x00C11B04, /* SPDSnaoxn */
	0x00C21884, /* SPDSonoxn */
	0x00C3006A, /* PSxn */
	0x00C40E04, /* SPDnoa */
	0x00C50664, /* SPDSxoxn */
	0x00C60708, /* SDPnax */
	0x00C707AA, /* PSDPoaxn */
	0x00C803A8, /* SDPoa */
	0x00C90184, /* SPDoxn */
	0x00CA0749, /* DPSDxax */
	0x00CB06E4, /* SPDSaoxn */
	0x00CC0020, /* S */
	0x00CD0888, /* SDPono */
	0x00CE0B08, /* SDPnao */
	0x00CF0224, /* SPno */
	0x00D00E0A, /* PSDnoa */
	0x00D1066A, /* PSDPxoxn */
	0x00D20705, /* PDSnax */
	0x00D307A4, /* SPDSoaxn */
	0x00D41D78, /* SSPxPDxax */
	0x00D50CE9, /* DPSanan */
	0x00D616EA, /* PSDPSaoxx */
	0x00D70349, /* DPSxan */
	0x00D80745, /* PDSPxax */
	0x00D906E8, /* SDPSaoxn */
	0x00DA1CE9, /* DPSDanax */
	0x00DB0D75, /* SPxDSxan */
	0x00DC0B04, /* SPDnao */
	0x00DD0228, /* SDno */
	0x00DE0268, /* SDPxo */
	0x00DF08C8, /* SDPano */
	0x00E003A5, /* PDSoa */
	0x00E10185, /* PDSoxn */
	0x00E20746, /* DSPDxax */
	0x00E306EA, /* PSDPaoxn */
	0x00E40748, /* SDPSxax */
	0x00E506E5, /* PDSPaoxn */
	0x00E61CE8, /* SDPSanax */
	0x00E70D79, /* SPxPDxan */
	0x00E81D74, /* SSPxDSxax */
	0x00E95CE6, /* DSPDSanaxxn */
	0x00EA02E9, /* DPSao */
	0x00EB0849, /* DPSxno */
	0x00EC02E8, /* SDPao */
	0x00ED0848, /* SDPxno */
	0x00EE0086, /* DSo */
	0x00EF0A08, /* SDPnoo */
	0x00F00021, /* P */
	0x00F10885, /* PDSono */
	0x00F20B05, /* PDSnao */
	0x00F3022A, /* PSno */
	0x00F40B0A, /* PSDnao */
	0x00F50225, /* PDno */
	0x00F60265, /* PDSxo */
	0x00F708C5, /* PDSano */
	0x00F802E5, /* PDSao */
	0x00F90845, /* PDSxno */
	0x00FA0089, /* DPo */
	0x00FB0A09, /* DPSnoo */
	0x00FC008A, /* PSo */
	0x00FD0A0A, /* PSDnoo */
	0x00FE02A9, /* DPSoo */
	0x00FF0062  /* 1 */
};

/* GDI Helper Functions */

INLINE uint32 gdi_rop3_code(uint8 code)
{
	return rop3_code_table[code];
}

INLINE uint8* gdi_get_bitmap_pointer(HGDI_DC hdcBmp, int x, int y)
{
	uint8* p;
	HGDI_BITMAP hBmp = (HGDI_BITMAP) hdcBmp->selectedObject;
	
	if (x >= 0 && x < hBmp->width && y >= 0 && y < hBmp->height)
	{
		p = hBmp->data + (y * hBmp->width * hdcBmp->bytesPerPixel) + (x * hdcBmp->bytesPerPixel);
		return p;
	}
	else
	{
		printf("gdi_get_bitmap_pointer: requesting invalid pointer: (%d,%d) in %dx%d\n", x, y, hBmp->width, hBmp->height);
		return 0;
	}
}

INLINE uint8* gdi_get_brush_pointer(HGDI_DC hdcBrush, int x, int y)
{
	uint8 * p;

	if (hdcBrush->brush != NULL)
	{
		if (hdcBrush->brush->style == GDI_BS_PATTERN)
		{
			HGDI_BITMAP hBmpBrush = hdcBrush->brush->pattern;
	
			if (x >= 0 && y >= 0)
			{
				x = x % hBmpBrush->width;
				y = y % hBmpBrush->height;
				p = hBmpBrush->data + (y * hBmpBrush->scanline) + (x * hBmpBrush->bytesPerPixel);
				return p;
			}
		}
	}

	p = (uint8*) &(hdcBrush->textColor);
	return p;
}

INLINE int gdi_is_mono_pixel_set(uint8* data, int x, int y, int width)
{
	int byte;
	int shift;

	width = (width + 7) / 8;
	byte = (y * width) + (x / 8);
	shift = x % 8;

	return (data[byte] & (0x80 >> shift)) != 0;
}

HGDI_BITMAP gdi_create_bitmap(GDI* gdi, int width, int height, int bpp, uint8* data)
{
	uint8* bmpData;
	HGDI_BITMAP bitmap;
	
	bmpData = freerdp_image_convert(data, NULL, width, height, gdi->srcBpp, bpp, gdi->clrconv);
	bitmap = gdi_CreateBitmap(width, height, gdi->dstBpp, bmpData);
	
	return bitmap;
}

gdiBitmap* gdi_bitmap_new_ex(GDI* gdi, int width, int height, int bpp, uint8* data)
{
	gdiBitmap* bitmap;
	
	bitmap = (gdiBitmap*) malloc(sizeof(gdiBitmap));
	bitmap->hdc = gdi_CreateCompatibleDC(gdi->hdc);
	
	DEBUG_GDI("gdi_bitmap_new: width:%d height:%d bpp:%d", width, height, bpp);

	if (data == NULL)
		bitmap->bitmap = gdi_CreateCompatibleBitmap(gdi->hdc, width, height);
	else
		bitmap->bitmap = gdi_create_bitmap(gdi, width, height, bpp, data);
	
	gdi_SelectObject(bitmap->hdc, (HGDIOBJECT) bitmap->bitmap);
	bitmap->org_bitmap = NULL;

	return bitmap;
}

void gdi_bitmap_free_ex(gdiBitmap* bitmap)
{
	if (bitmap != NULL)
	{
		gdi_SelectObject(bitmap->hdc, (HGDIOBJECT) bitmap->org_bitmap);
		gdi_DeleteObject((HGDIOBJECT) bitmap->bitmap);
		gdi_DeleteDC(bitmap->hdc);
		free(bitmap);
	}
}

void gdi_bitmap_size(rdpUpdate* update, uint32* size)
{
	*size = sizeof(gdiBitmap);
}

void gdi_bitmap_new(rdpUpdate* update, gdiBitmap* bitmap)
{
	uint8* data;
	rdpBitmap* _bitmap;
	GDI* gdi = GET_GDI(update);

	_bitmap = &(bitmap->_p);
	bitmap->hdc = gdi_CreateCompatibleDC(gdi->hdc);

	data = _bitmap->dstData;

	if (data == NULL)
		bitmap->bitmap = gdi_CreateCompatibleBitmap(gdi->hdc, _bitmap->width, _bitmap->height);
	else
		bitmap->bitmap = gdi_create_bitmap(gdi, _bitmap->width, _bitmap->height, gdi->dstBpp, data);

	gdi_SelectObject(bitmap->hdc, (HGDIOBJECT) bitmap->bitmap);
	bitmap->org_bitmap = NULL;
}

void gdi_offscreen_bitmap_new(rdpUpdate* update, gdiBitmap* bitmap)
{
	rdpBitmap* _bitmap;
	GDI* gdi = GET_GDI(update);

	_bitmap = &(bitmap->_p);
	bitmap->hdc = gdi_CreateCompatibleDC(gdi->hdc);

	bitmap->bitmap = gdi_CreateCompatibleBitmap(gdi->hdc, _bitmap->width, _bitmap->height);

	gdi_SelectObject(bitmap->hdc, (HGDIOBJECT) bitmap->bitmap);
	bitmap->org_bitmap = NULL;
}

void gdi_set_surface(rdpUpdate* update, gdiBitmap* bitmap, boolean primary)
{
	GDI* gdi = GET_GDI(update);

	if (primary)
		gdi->drawing = gdi->primary;
	else
		gdi->drawing = bitmap;
}

void gdi_bitmap_free(rdpUpdate* update, gdiBitmap* bitmap)
{
	if (bitmap != NULL)
	{
		gdi_SelectObject(bitmap->hdc, (HGDIOBJECT) bitmap->org_bitmap);
		gdi_DeleteObject((HGDIOBJECT) bitmap->bitmap);
		gdi_DeleteDC(bitmap->hdc);
	}
}

gdiBitmap* gdi_glyph_new(GDI* gdi, GLYPH_DATA* glyph)
{
	uint8* extra;
	gdiBitmap* gdi_bmp;

	gdi_bmp = (gdiBitmap*) malloc(sizeof(gdiBitmap));

	gdi_bmp->hdc = gdi_GetDC();
	gdi_bmp->hdc->bytesPerPixel = 1;
	gdi_bmp->hdc->bitsPerPixel = 1;

	extra = freerdp_glyph_convert(glyph->cx, glyph->cy, glyph->aj);
	gdi_bmp->bitmap = gdi_CreateBitmap(glyph->cx, glyph->cy, 1, extra);
	gdi_bmp->bitmap->bytesPerPixel = 1;
	gdi_bmp->bitmap->bitsPerPixel = 1;

	gdi_SelectObject(gdi_bmp->hdc, (HGDIOBJECT) gdi_bmp->bitmap);
	gdi_bmp->org_bitmap = NULL;

	return gdi_bmp;
}

void gdi_glyph_free(gdiBitmap *gdi_bmp)
{
	if (gdi_bmp != 0)
	{
		gdi_SelectObject(gdi_bmp->hdc, (HGDIOBJECT) gdi_bmp->org_bitmap);
		gdi_DeleteObject((HGDIOBJECT) gdi_bmp->bitmap);
		gdi_DeleteDC(gdi_bmp->hdc);
		free(gdi_bmp);
	}
}

void gdi_bitmap_update(rdpUpdate* update, BITMAP_UPDATE* bitmap)
{
	int i;
	rdpBitmap* bmp;
	gdiBitmap* gdi_bmp;
	GDI* gdi = GET_GDI(update);

	for (i = 0; i < bitmap->number; i++)
	{
		bmp = &bitmap->bitmaps[i];

		gdi_bmp = gdi_bitmap_new_ex(gdi, bmp->width, bmp->height, gdi->dstBpp, bmp->dstData);

		gdi_BitBlt(gdi->primary->hdc,
				bmp->left, bmp->top, bmp->right - bmp->left + 1,
				bmp->bottom - bmp->top + 1, gdi_bmp->hdc, 0, 0, GDI_SRCCOPY);

		gdi_bitmap_free_ex((gdiBitmap*) gdi_bmp);
	}
}

void gdi_palette_update(rdpUpdate* update, PALETTE_UPDATE* palette)
{

}

void gdi_set_bounds(rdpUpdate* update, BOUNDS* bounds)
{
	GDI* gdi = GET_GDI(update);

	if (bounds != NULL)
	{
		gdi_SetClipRgn(gdi->drawing->hdc, bounds->left, bounds->top,
				bounds->right - bounds->left + 1, bounds->bottom - bounds->top + 1);
	}
	else
	{
		gdi_SetNullClipRgn(gdi->drawing->hdc);
	}
}

void gdi_dstblt(rdpUpdate* update, DSTBLT_ORDER* dstblt)
{
	GDI* gdi = GET_GDI(update);

	gdi_BitBlt(gdi->drawing->hdc, dstblt->nLeftRect, dstblt->nTopRect,
			dstblt->nWidth, dstblt->nHeight, NULL, 0, 0, gdi_rop3_code(dstblt->bRop));
}

void gdi_patblt(rdpUpdate* update, PATBLT_ORDER* patblt)
{
	uint8* data;
	BRUSH* brush;
	HGDI_BRUSH originalBrush;
	GDI* gdi = GET_GDI(update);

	brush = &patblt->brush;

	if (brush->style & CACHED_BRUSH)
	{
		brush->data = brush_get(gdi->cache->brush, brush->index, &brush->bpp);
		brush->style = GDI_BS_PATTERN;
	}

	if (brush->style == GDI_BS_SOLID)
	{
		uint32 color;
		originalBrush = gdi->drawing->hdc->brush;

		color = freerdp_color_convert(patblt->foreColor, gdi->srcBpp, 32, gdi->clrconv);
		gdi->drawing->hdc->brush = gdi_CreateSolidBrush(color);

		gdi_PatBlt(gdi->drawing->hdc, patblt->nLeftRect, patblt->nTopRect,
				patblt->nWidth, patblt->nHeight, gdi_rop3_code(patblt->bRop));

		gdi_DeleteObject((HGDIOBJECT) gdi->drawing->hdc->brush);
		gdi->drawing->hdc->brush = originalBrush;
	}
	else if (brush->style == GDI_BS_PATTERN)
	{
		HGDI_BITMAP hBmp;

		if (brush->bpp > 1)
		{
			data = freerdp_image_convert(brush->data, NULL, 8, 8, gdi->srcBpp, gdi->dstBpp, gdi->clrconv);
		}
		else
		{
			data = freerdp_mono_image_convert(brush->data, 8, 8, gdi->srcBpp, gdi->dstBpp,
				patblt->backColor, patblt->foreColor, gdi->clrconv);
		}

		hBmp = gdi_CreateBitmap(8, 8, gdi->drawing->hdc->bitsPerPixel, data);

		originalBrush = gdi->drawing->hdc->brush;
		gdi->drawing->hdc->brush = gdi_CreatePatternBrush(hBmp);

		gdi_PatBlt(gdi->drawing->hdc, patblt->nLeftRect, patblt->nTopRect,
				patblt->nWidth, patblt->nHeight, gdi_rop3_code(patblt->bRop));

		gdi_DeleteObject((HGDIOBJECT) gdi->drawing->hdc->brush);
		gdi->drawing->hdc->brush = originalBrush;
	}
	else
	{
		printf("unimplemented brush style:%d\n", brush->style);
	}
}

void gdi_scrblt(rdpUpdate* update, SCRBLT_ORDER* scrblt)
{
	GDI* gdi = GET_GDI(update);

	gdi_BitBlt(gdi->drawing->hdc, scrblt->nLeftRect, scrblt->nTopRect,
			scrblt->nWidth, scrblt->nHeight, gdi->primary->hdc,
			scrblt->nXSrc, scrblt->nYSrc, gdi_rop3_code(scrblt->bRop));
}

void gdi_opaque_rect(rdpUpdate* update, OPAQUE_RECT_ORDER* opaque_rect)
{
	GDI_RECT rect;
	HGDI_BRUSH hBrush;
	uint32 brush_color;
	GDI *gdi = GET_GDI(update);

	gdi_CRgnToRect(opaque_rect->nLeftRect, opaque_rect->nTopRect,
			opaque_rect->nWidth, opaque_rect->nHeight, &rect);

	brush_color = freerdp_color_convert(opaque_rect->color, gdi->srcBpp, 32, gdi->clrconv);

	hBrush = gdi_CreateSolidBrush(brush_color);
	gdi_FillRect(gdi->drawing->hdc, &rect, hBrush);

	gdi_DeleteObject((HGDIOBJECT) hBrush);
}

void gdi_multi_opaque_rect(rdpUpdate* update, MULTI_OPAQUE_RECT_ORDER* multi_opaque_rect)
{
	int i;
	GDI_RECT rect;
	HGDI_BRUSH hBrush;
	uint32 brush_color;
	DELTA_RECT* rectangle;
	GDI *gdi = GET_GDI(update);

	for (i = 1; i < multi_opaque_rect->numRectangles + 1; i++)
	{
		rectangle = &multi_opaque_rect->rectangles[i];

		gdi_CRgnToRect(rectangle->left, rectangle->top,
				rectangle->width, rectangle->height, &rect);

		brush_color = freerdp_color_convert(multi_opaque_rect->color, gdi->srcBpp, 32, gdi->clrconv);

		hBrush = gdi_CreateSolidBrush(brush_color);
		gdi_FillRect(gdi->drawing->hdc, &rect, hBrush);

		gdi_DeleteObject((HGDIOBJECT) hBrush);
	}
}

void gdi_line_to(rdpUpdate* update, LINE_TO_ORDER* line_to)
{
	uint32 color;
	HGDI_PEN hPen;
	GDI *gdi = GET_GDI(update);

	color = freerdp_color_convert(line_to->penColor, gdi->srcBpp, 32, gdi->clrconv);
	hPen = gdi_CreatePen(line_to->penStyle, line_to->penWidth, (GDI_COLOR) color);
	gdi_SelectObject(gdi->drawing->hdc, (HGDIOBJECT) hPen);
	gdi_SetROP2(gdi->drawing->hdc, line_to->bRop2);

	gdi_MoveToEx(gdi->drawing->hdc, line_to->nXStart, line_to->nYStart, NULL);
	gdi_LineTo(gdi->drawing->hdc, line_to->nXEnd, line_to->nYEnd);

	gdi_DeleteObject((HGDIOBJECT) hPen);
}

void gdi_polyline(rdpUpdate* update, POLYLINE_ORDER* polyline)
{
	int i;
	uint32 color;
	HGDI_PEN hPen;
	DELTA_POINT* points;
	GDI* gdi = GET_GDI(update);

	color = freerdp_color_convert(polyline->penColor, gdi->srcBpp, 32, gdi->clrconv);
	hPen = gdi_CreatePen(0, 1, (GDI_COLOR) color);
	gdi_SelectObject(gdi->drawing->hdc, (HGDIOBJECT) hPen);
	gdi_SetROP2(gdi->drawing->hdc, polyline->bRop2);

	gdi_MoveToEx(gdi->drawing->hdc, polyline->xStart, polyline->yStart, NULL);

	points = polyline->points;
	for (i = 0; i < polyline->numPoints; i++)
	{
		gdi_LineTo(gdi->drawing->hdc, points[i].x, points[i].y);
		gdi_MoveToEx(gdi->drawing->hdc, points[i].x, points[i].y, NULL);
	}

	gdi_DeleteObject((HGDIOBJECT) hPen);
}

void gdi_memblt(rdpUpdate* update, MEMBLT_ORDER* memblt)
{
	gdiBitmap* bitmap;
	GDI* gdi = GET_GDI(update);

	bitmap = (gdiBitmap*) memblt->bitmap;

	gdi_BitBlt(gdi->drawing->hdc, memblt->nLeftRect, memblt->nTopRect,
			memblt->nWidth, memblt->nHeight, bitmap->hdc,
			memblt->nXSrc, memblt->nYSrc, gdi_rop3_code(memblt->bRop));
}

void gdi_mem3blt(rdpUpdate* update, MEM3BLT_ORDER* mem3blt)
{

}

void gdi_fast_index(rdpUpdate* update, FAST_INDEX_ORDER* fast_index)
{
	int i, j;
	int x, y;
	GDI_RECT rect;
	uint32 bgcolor;
	uint32 fgcolor;
	HGDI_BRUSH brush;
	gdiBitmap* bmp;
	gdiBitmap** bmps;
	GLYPH_DATA* glyph;
	GLYPH_DATA** glyphs;
	GLYPH_FRAGMENT* fragment;
	GDI* gdi = GET_GDI(update);

	x = fast_index->bkLeft;
	y = fast_index->y;

	bgcolor = freerdp_color_convert(fast_index->backColor, gdi->srcBpp, 32, gdi->clrconv);
	fgcolor = freerdp_color_convert(fast_index->foreColor, gdi->srcBpp, 32, gdi->clrconv);
	gdi->textColor = gdi_SetTextColor(gdi->drawing->hdc, bgcolor);
	gdi_SetNullClipRgn(gdi->drawing->hdc);
	brush = gdi_CreateSolidBrush(fgcolor);

	if (fast_index->opaqueRect)
	{
		gdi_SetRect(&rect, fast_index->opLeft, fast_index->opTop, fast_index->opRight, fast_index->opBottom);
		gdi_FillRect(gdi->drawing->hdc, &rect, brush);
	}

	for (i = 0; i < fast_index->nfragments; i++)
	{
		fragment = &fast_index->fragments[i];

		if (fragment->operation == GLYPH_FRAGMENT_USE)
		{
			fragment->indices = (GLYPH_FRAGMENT_INDEX*) glyph_fragment_get(gdi->cache->glyph,
							fragment->index, &fragment->nindices, (void**) &bmps);

			glyphs = (GLYPH_DATA**) xmalloc(sizeof(GLYPH_DATA*) * fragment->nindices);

			for (j = 0; j < fragment->nindices; j++)
			{
				glyphs[j] = glyph_get(gdi->cache->glyph, fast_index->cacheId, fragment->indices[j].index, (void**) &bmps[j]);
			}
		}
		else
		{
			bmps = (gdiBitmap**) xmalloc(sizeof(gdiBitmap*) * fragment->nindices);
			glyphs = (GLYPH_DATA**) xmalloc(sizeof(GLYPH_DATA*) * fragment->nindices);

			for (j = 0; j < fragment->nindices; j++)
			{
				glyphs[j] = glyph_get(gdi->cache->glyph, fast_index->cacheId, fragment->indices[j].index, (void**) &bmps[j]);
			}
		}

		for (j = 0; j < fragment->nindices; j++)
		{
			bmp = bmps[j];
			glyph = glyphs[j];

			if (bmp == NULL || glyph == NULL)
				continue;

			gdi_BitBlt(gdi->drawing->hdc, glyph->x + x, glyph->y + y, bmp->bitmap->width,
					bmp->bitmap->height, bmp->hdc, 0, 0, GDI_DSPDxax);

			if ((j + 1) < fragment->nindices)
			{
				if (!(fast_index->flAccel & SO_CHAR_INC_EQUAL_BM_BASE))
				{
					if (fast_index->flAccel & SO_VERTICAL)
					{
						y += fragment->indices[j + 1].delta;
					}
					else
					{
						x += fragment->indices[j + 1].delta;
					}
				}
				else
				{
					x += glyph->cx;
				}
			}
		}

		if (fragment->operation == GLYPH_FRAGMENT_ADD)
		{
			glyph_fragment_put(gdi->cache->glyph, fragment->index,
					fragment->nindices, (void*) fragment->indices, (void*) bmps);
		}
	}

	gdi_SetTextColor(gdi->drawing->hdc, gdi->textColor);
	gdi_DeleteObject((HGDIOBJECT) brush);
}

void gdi_cache_color_table(rdpUpdate* update, CACHE_COLOR_TABLE_ORDER* cache_color_table)
{
	GDI* gdi = GET_GDI(update);
	color_table_put(gdi->cache->color_table, cache_color_table->cacheIndex, (void*) cache_color_table->colorTable);
}

void gdi_cache_glyph(rdpUpdate* update, CACHE_GLYPH_ORDER* cache_glyph)
{
	int i;
	GLYPH_DATA* glyph;
	gdiBitmap* gdi_bmp;
	GDI* gdi = GET_GDI(update);

	for (i = 0; i < cache_glyph->cGlyphs; i++)
	{
		glyph = cache_glyph->glyphData[i];
		gdi_bmp = gdi_glyph_new(gdi, glyph);
		glyph_put(gdi->cache->glyph, cache_glyph->cacheId, glyph->cacheIndex, glyph, (void*) gdi_bmp);
	}
}

void gdi_cache_glyph_v2(rdpUpdate* update, CACHE_GLYPH_V2_ORDER* cache_glyph_v2)
{
	int i;
	uint8* extra;
	GLYPH_DATA_V2* glyph;
	gdiBitmap* gdi_bmp;
	GDI* gdi = GET_GDI(update);

	for (i = 0; i < cache_glyph_v2->cGlyphs; i++)
	{
		glyph = cache_glyph_v2->glyphData[i];
		gdi_bmp = (gdiBitmap*) malloc(sizeof(gdiBitmap));

		gdi_bmp->hdc = gdi_GetDC();
		gdi_bmp->hdc->bytesPerPixel = 1;
		gdi_bmp->hdc->bitsPerPixel = 1;

		extra = freerdp_glyph_convert(glyph->cx, glyph->cy, glyph->aj);
		gdi_bmp->bitmap = gdi_CreateBitmap(glyph->cx, glyph->cy, 1, extra);
		gdi_bmp->bitmap->bytesPerPixel = 1;
		gdi_bmp->bitmap->bitsPerPixel = 1;

		gdi_SelectObject(gdi_bmp->hdc, (HGDIOBJECT) gdi_bmp->bitmap);
		gdi_bmp->org_bitmap = NULL;

		glyph_put(gdi->cache->glyph, cache_glyph_v2->cacheId, glyph->cacheIndex, glyph, (void*) gdi_bmp);
	}
}

void gdi_cache_brush(rdpUpdate* update, CACHE_BRUSH_ORDER* cache_brush)
{
	GDI* gdi = GET_GDI(update);
	brush_put(gdi->cache->brush, cache_brush->index, cache_brush->data, cache_brush->bpp);
}

int tilenum = 0;

void gdi_surface_bits(rdpUpdate* update, SURFACE_BITS_COMMAND* surface_bits_command)
{
	int i, j;
	int tx, ty;
	char* tile_bitmap;
	RFX_MESSAGE* message;
	GDI* gdi = GET_GDI(update);
	RFX_CONTEXT* context = (RFX_CONTEXT*) gdi->rfx_context;
	NSC_CONTEXT* ncontext = (NSC_CONTEXT*) gdi->nsc_context;


	DEBUG_GDI("destLeft %d destTop %d destRight %d destBottom %d "
		"bpp %d codecID %d width %d height %d length %d",
		surface_bits_command->destLeft, surface_bits_command->destTop,
		surface_bits_command->destRight, surface_bits_command->destBottom,
		surface_bits_command->bpp, surface_bits_command->codecID,
		surface_bits_command->width, surface_bits_command->height,
		surface_bits_command->bitmapDataLength);

	tile_bitmap = (char*) xzalloc(32);

	if (surface_bits_command->codecID == CODEC_ID_REMOTEFX)
	{
		message = rfx_process_message(context, surface_bits_command->bitmapData, surface_bits_command->bitmapDataLength);

		DEBUG_GDI("num_rects %d num_tiles %d", message->num_rects, message->num_tiles);

		/* blit each tile */
		for (i = 0; i < message->num_tiles; i++)
		{
			tx = message->tiles[i]->x + surface_bits_command->destLeft;
			ty = message->tiles[i]->y + surface_bits_command->destTop;

			freerdp_image_convert(message->tiles[i]->data, gdi->tile->bitmap->data, 64, 64, 32, 32, gdi->clrconv);

#ifdef DUMP_REMOTEFX_TILES
			sprintf(tile_bitmap, "/tmp/rfx/tile_%d.bmp", tilenum++);
			freerdp_bitmap_write(tile_bitmap, gdi->tile->bitmap->data, 64, 64, 32);
#endif

			for (j = 0; j < message->num_rects; j++)
			{
				gdi_SetClipRgn(gdi->primary->hdc,
					surface_bits_command->destLeft + message->rects[j].x,
					surface_bits_command->destTop + message->rects[j].y,
					message->rects[j].width, message->rects[j].height);

				gdi_BitBlt(gdi->primary->hdc, tx, ty, 64, 64, gdi->tile->hdc, 0, 0, GDI_SRCCOPY);
			}
		}

		gdi_SetNullClipRgn(gdi->primary->hdc);
		rfx_message_free(context, message);
	}
	else if (surface_bits_command->codecID == CODEC_ID_NSCODEC)
	{
		ncontext->width = surface_bits_command->width;
		ncontext->height = surface_bits_command->height;
		nsc_process_message(ncontext, surface_bits_command->bitmapData, surface_bits_command->bitmapDataLength);
		gdi->image->bitmap->width = surface_bits_command->width;
		gdi->image->bitmap->height = surface_bits_command->height;
		gdi->image->bitmap->bitsPerPixel = surface_bits_command->bpp;
		gdi->image->bitmap->bytesPerPixel = gdi->image->bitmap->bitsPerPixel / 8;
		gdi->image->bitmap->data = (uint8*) xrealloc(gdi->image->bitmap->data, gdi->image->bitmap->width * gdi->image->bitmap->height * 4);
		freerdp_image_flip(ncontext->bmpdata, gdi->image->bitmap->data, gdi->image->bitmap->width, gdi->image->bitmap->height, 32);
		gdi_BitBlt(gdi->primary->hdc, surface_bits_command->destLeft, surface_bits_command->destTop, surface_bits_command->width, surface_bits_command->height, gdi->image->hdc, 0, 0, GDI_SRCCOPY);
		nsc_context_destroy(ncontext);
	} 
	else if (surface_bits_command->codecID == CODEC_ID_NONE)
	{
		gdi->image->bitmap->width = surface_bits_command->width;
		gdi->image->bitmap->height = surface_bits_command->height;
		gdi->image->bitmap->bitsPerPixel = surface_bits_command->bpp;
		gdi->image->bitmap->bytesPerPixel = gdi->image->bitmap->bitsPerPixel / 8;

		gdi->image->bitmap->data = (uint8*) xrealloc(gdi->image->bitmap->data,
				gdi->image->bitmap->width * gdi->image->bitmap->height * 4);

		if ((surface_bits_command->bpp != 32) || (gdi->clrconv->alpha == True))
		{
			uint8* temp_image;

			freerdp_image_convert(surface_bits_command->bitmapData, gdi->image->bitmap->data,
				gdi->image->bitmap->width, gdi->image->bitmap->height,
				gdi->image->bitmap->bitsPerPixel, 32, gdi->clrconv);

			surface_bits_command->bpp = 32;
			surface_bits_command->bitmapData = gdi->image->bitmap->data;

			temp_image = (uint8*) xmalloc(gdi->image->bitmap->width * gdi->image->bitmap->height * 4);
			freerdp_image_flip(gdi->image->bitmap->data, temp_image, gdi->image->bitmap->width, gdi->image->bitmap->height, 32);
			xfree(gdi->image->bitmap->data);
			gdi->image->bitmap->data = temp_image;
		}
		else
		{
			freerdp_image_flip(surface_bits_command->bitmapData, gdi->image->bitmap->data,
					gdi->image->bitmap->width, gdi->image->bitmap->height, 32);
		}

		gdi_BitBlt(gdi->primary->hdc, surface_bits_command->destLeft, surface_bits_command->destTop,
				surface_bits_command->width, surface_bits_command->height, gdi->image->hdc, 0, 0, GDI_SRCCOPY);
	}
	else
	{
		printf("Unsupported codecID %d\n", surface_bits_command->codecID);
	}

	if (tile_bitmap != NULL)
		xfree(tile_bitmap);
}

void gdi_bitmap_decompress(rdpUpdate* update, rdpBitmap* bitmap_data)
{
	uint16 length;

	length = bitmap_data->width * bitmap_data->height * (bitmap_data->bpp / 8);

	if (bitmap_data->dstData == NULL)
		bitmap_data->dstData = (uint8*) xmalloc(length);
	else
		bitmap_data->dstData = (uint8*) xrealloc(bitmap_data->dstData, length);

	if (bitmap_data->compressed)
	{
		boolean status;

		status = bitmap_decompress(bitmap_data->srcData, bitmap_data->dstData,
				bitmap_data->width, bitmap_data->height, bitmap_data->length,
				bitmap_data->bpp, bitmap_data->bpp);

		if (status != True)
		{
			printf("Bitmap Decompression Failed\n");
		}
	}
	else
	{
		freerdp_image_flip(bitmap_data->srcData, bitmap_data->dstData,
				bitmap_data->width, bitmap_data->height, bitmap_data->bpp);
	}

	bitmap_data->compressed = False;
	bitmap_data->length = length;
}

/**
 * Register GDI callbacks with libfreerdp-core.
 * @param inst current instance
 * @return
 */

void gdi_register_update_callbacks(rdpUpdate* update)
{
	update->Bitmap = gdi_bitmap_update;
	update->Palette = gdi_palette_update;
	update->SetBounds = gdi_set_bounds;
	update->DstBlt = gdi_dstblt;
	update->PatBlt = gdi_patblt;
	update->ScrBlt = gdi_scrblt;
	update->OpaqueRect = gdi_opaque_rect;
	update->DrawNineGrid = NULL;
	update->MultiDstBlt = NULL;
	update->MultiPatBlt = NULL;
	update->MultiScrBlt = NULL;
	update->MultiOpaqueRect = gdi_multi_opaque_rect;
	update->MultiDrawNineGrid = NULL;
	update->LineTo = gdi_line_to;
	update->Polyline = NULL;
	update->MemBlt = gdi_memblt;
	update->Mem3Blt = gdi_mem3blt;
	update->SaveBitmap = NULL;
	update->GlyphIndex = NULL;
	update->FastIndex = gdi_fast_index;
	update->FastGlyph = NULL;
	update->PolygonSC = NULL;
	update->PolygonCB = NULL;
	update->EllipseSC = NULL;
	update->EllipseCB = NULL;

	update->CacheColorTable = gdi_cache_color_table;
	update->CacheGlyph = gdi_cache_glyph;
	update->CacheGlyphV2 = gdi_cache_glyph_v2;
	update->CacheBrush = gdi_cache_brush;

	update->SurfaceBits = gdi_surface_bits;
	update->BitmapDecompress = gdi_bitmap_decompress;
}

void gdi_init_primary(GDI* gdi)
{
	gdi->primary = gdi_bitmap_new_ex(gdi, gdi->width, gdi->height, gdi->dstBpp, gdi->primary_buffer);
	gdi->primary_buffer = gdi->primary->bitmap->data;

	if (gdi->drawing == NULL)
		gdi->drawing = gdi->primary;

	gdi->primary->hdc->hwnd = (HGDI_WND) malloc(sizeof(GDI_WND));
	gdi->primary->hdc->hwnd->invalid = gdi_CreateRectRgn(0, 0, 0, 0);
	gdi->primary->hdc->hwnd->invalid->null = 1;

	gdi->primary->hdc->hwnd->count = 32;
	gdi->primary->hdc->hwnd->cinvalid = (HGDI_RGN) malloc(sizeof(GDI_RGN) * gdi->primary->hdc->hwnd->count);
	gdi->primary->hdc->hwnd->ninvalid = 0;
}

void gdi_resize(GDI* gdi, int width, int height)
{
	if (gdi && gdi->primary)
	{
		if (gdi->width != width || gdi->height != height)
		{
			if (gdi->drawing == gdi->primary)
				gdi->drawing = NULL;

			gdi->width = width;
			gdi->height = height;
			gdi_bitmap_free_ex(gdi->primary);
			gdi_init_primary(gdi);
		}
	}
}

/**
 * Initialize GDI
 * @param inst current instance
 * @return
 */

int gdi_init(freerdp* instance, uint32 flags, uint8* buffer)
{
	GDI* gdi = (GDI*) malloc(sizeof(GDI));
	memset(gdi, 0, sizeof(GDI));
	SET_GDI(instance->update, gdi);

	gdi->width = instance->settings->width;
	gdi->height = instance->settings->height;
	gdi->srcBpp = instance->settings->color_depth;
	gdi->primary_buffer = buffer;

	/* default internal buffer format */
	gdi->dstBpp = 32;
	gdi->bytesPerPixel = 4;

	if (gdi->srcBpp > 16)
	{
		if (flags & CLRBUF_32BPP)
		{
			gdi->dstBpp = 32;
			gdi->bytesPerPixel = 4;
		}
		else if (flags & CLRBUF_24BPP)
		{
			gdi->dstBpp = 24;
			gdi->bytesPerPixel = 3;
		}
		else if (flags & CLRBUF_16BPP)
		{
			gdi->dstBpp = 16;
			gdi->bytesPerPixel = 2;
		}
	}
	else
	{
		if (flags & CLRBUF_16BPP)
		{
			gdi->dstBpp = 16;
			gdi->bytesPerPixel = 2;
		}
		else if (flags & CLRBUF_32BPP)
		{
			gdi->dstBpp = 32;
			gdi->bytesPerPixel = 4;
		}
	}
	
	gdi->hdc = gdi_GetDC();
	gdi->hdc->bitsPerPixel = gdi->dstBpp;
	gdi->hdc->bytesPerPixel = gdi->bytesPerPixel;

	gdi->clrconv = (HCLRCONV) malloc(sizeof(CLRCONV));
	gdi->clrconv->palette = NULL;
	gdi->clrconv->alpha = (flags & CLRCONV_ALPHA) ? 1 : 0;
	gdi->clrconv->invert = (flags & CLRCONV_INVERT) ? 1 : 0;
	gdi->clrconv->rgb555 = (flags & CLRCONV_RGB555) ? 1 : 0;

	gdi->hdc->alpha = gdi->clrconv->alpha;
	gdi->hdc->invert = gdi->clrconv->invert;
	gdi->hdc->rgb555 = gdi->clrconv->rgb555;

	gdi_init_primary(gdi);

	gdi->tile = gdi_bitmap_new_ex(gdi, 64, 64, 32, NULL);
	gdi->image = gdi_bitmap_new_ex(gdi, 64, 64, 32, NULL);

	if (instance->cache == NULL)
	{
		instance->cache = (void*) cache_new(instance->settings);
		instance->update->cache = instance->cache;
	}

	gdi->cache = (rdpCache*) instance->cache;

	gdi_register_update_callbacks(instance->update);

	bitmap_cache_register_callbacks(instance->update);
	gdi->cache->bitmap->BitmapSize = (cbBitmapSize) gdi_bitmap_size;
	gdi->cache->bitmap->BitmapNew = (cbBitmapNew) gdi_bitmap_new;
	gdi->cache->bitmap->BitmapFree = (cbBitmapFree) gdi_bitmap_free;

	offscreen_cache_register_callbacks(instance->update);
	gdi->cache->offscreen->OffscreenBitmapSize = (cbOffscreenBitmapSize) gdi_bitmap_size;
	gdi->cache->offscreen->OffscreenBitmapNew = (cbOffscreenBitmapNew) gdi_offscreen_bitmap_new;
	gdi->cache->offscreen->OffscreenBitmapFree = (cbOffscreenBitmapFree) gdi_bitmap_free;
	gdi->cache->offscreen->SetSurface = (cbSetSurface) gdi_set_surface;

	gdi->rfx_context = rfx_context_new();
	gdi->nsc_context = nsc_context_new();

	return 0;
}

void gdi_free(freerdp* instance)
{
	GDI *gdi = GET_GDI(instance->update);

	if (gdi)
	{
		gdi_bitmap_free_ex(gdi->primary);
		gdi_bitmap_free_ex(gdi->tile);
		gdi_bitmap_free_ex(gdi->image);
		gdi_DeleteDC(gdi->hdc);
		cache_free(gdi->cache);
		rfx_context_free((RFX_CONTEXT*)gdi->rfx_context);
		free(gdi->clrconv);
		free(gdi);
	}
	
	SET_GDI(instance->update, NULL);
}

