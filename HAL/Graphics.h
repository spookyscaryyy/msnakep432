/*
 * Graphics.h
 *
 *  Created on: Dec 30, 2019
 *      Author: Matthew Zhong
 */

#ifndef HAL_GRAPHICS_H_
#define HAL_GRAPHICS_H_

#include <HAL/LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#define FG_COLOR GRAPHICS_COLOR_WHITE
#define BG_COLOR GRAPHICS_COLOR_BLACK

struct _GFX
{
    Graphics_Context context;
    uint32_t foreground;
    uint32_t background;
    uint32_t defaultForeground;
    uint32_t defaultBackground;
};
typedef struct _GFX GFX;

GFX GFX_construct(uint32_t defaultForeground, uint32_t defaultBackground);

void GFX_resetColors(GFX* gfx_p);
void GFX_clear(GFX* gfx_p);

void GFX_print(GFX* gfx_p, char* string, int row, int col);
void GFX_print_nudge(GFX* gfx_p, char* string, int row, int col, int x, int y, bool opaque);
void GFX_setForeground(GFX* gfx_p, uint32_t foreground);
void GFX_setBackground(GFX* gfx_p, uint32_t background);

void GFX_drawSolidCircle(GFX* gfx_p, int x, int y, int radius);
void GFX_drawHollowCircle(GFX* gfx_p, int x, int y, int radius);

void GFX_drawSolidRectangle(GFX* gfx_p, Graphics_Rectangle* rect_p);
void GFX_drawHollowRectangle(GFX* gfx_p, Graphics_Rectangle* rect_p);

void GFX_drawLine(GFX* gfx_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void GFX_drawHorizLine(GFX* gfx_p, int32_t y, int32_t x1, int32_t x2);
void GFX_drawVertLine(GFX* gfx_p, int32_t x, int32_t y1, int32_t y2);

void GFX_removeSolidCircle(GFX* gfx_p, int x, int y, int radius);
void GFX_removeHollowCircle(GFX* gfx_p, int x, int y, int radius);

void GFX_removeSolidRectangle(GFX* gfx_p, Graphics_Rectangle* rect_p);
void GFX_removeHollowRectangle(GFX* gfx_p, Graphics_Rectangle* rect_p);

void GFX_removeLine(GFX* gfx_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void GFX_removeHorizLine(GFX* gfx_p, int32_t y, int32_t x1, int32_t x2);
void GFX_removeVertLine(GFX* gfx_p, int32_t x, int32_t y1, int32_t y2);

#endif /* HAL_GRAPHICS_H_ */
