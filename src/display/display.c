
#include "display.h"
#include "periph.h"
#include "ili9341.h"

#include <math.h>
#include <stdio.h>
#include <string.h>


int display_draw_text_8x12(int x, int y, const char *str, uint16_t color)
{
	uint16_t *fbp = (uint16_t *)VB_Addr;
	uint8_t mask = 0x80;
	uint8_t *fptr = (uint8_t *)pixfont8x12;
	int dx = 11;

	if (x >= 240) {
		return -1;
	}

	if (y >= 320) {
		return -1;
	}

	fbp += ((y * 240) + x);

	while (*str != 0) {
		if (mask & (*(fptr + ((*str) * 12) + dx))) {
			*fbp = color;
		} else {
			*fbp = 0;
		}

		dx --;
		if (dx < 0) {
			dx = 11;
			fbp += 229;
			mask >>= 1;
			if (mask == 0) {
				mask = 0x80;
				str++;
			}
			continue;
		}
		fbp++;
	}

	return 0;
}

void draw_pixel(int x, int y, uint16_t color)
{
	*(VB_Addr + ((y * 240) + x)) = color;
}

void draw_line(int start_x, int start_y,
	int end_x, int end_y , unsigned short color)
{
	int dx = fabs(end_x - start_x);
	int dy = fabs(end_y - start_y);
	int sx = (start_x < end_x) ? 1 : -1;
	int sy = (start_y < end_y) ? 1 : -1;
	int error = dx - dy;
	int error2;
	
	for (;;) {
		draw_pixel(start_x, start_y, color);

		if ((start_x == end_x) && (start_y == end_y)) {
			break;
		}
		
		error2 = error * 2;
		
		if (error2 > -dy) {
			error -= dy;
			start_x += sx;
		}
		if (error2 < dx){
			error += dx;
			start_y += sy;
		}
	}
}

void draw_ellipse(int Xpos, int Ypos, int Radius, int Radius2, uint16_t color)
{
	int x = -Radius, y = 0, err = 2-2 * Radius, e2;
	float K = 0, rad1 = 0, rad2 = 0;
	rad1 = Radius;
	rad2 = Radius2;
	
	if (Radius > Radius2) { 
		do {
			K = (float)(rad1 / rad2);
			*(__IO uint16_t*) (VB_Addr + (2*((Xpos - x) + 240 * (Ypos + (uint16_t)(y / K))))) = color; 
			*(__IO uint16_t*) (VB_Addr + (2*((Xpos + x) + 240 * (Ypos + (uint16_t)(y / K))))) = color;
			*(__IO uint16_t*) (VB_Addr + (2*((Xpos + x) + 240 * (Ypos - (uint16_t)(y / K))))) = color;
			*(__IO uint16_t*) (VB_Addr + (2*((Xpos - x) + 240 * (Ypos - (uint16_t)(y / K))))) = color;
				  
			e2 = err;
			if (e2 <= y) {
				err += ++y * 2 + 1;
				if (-x == y && e2 <= x) {
					e2 = 0;
				}
			}
			
			if (e2 > x) {
				err += ++x*2+1;
			}
		} while (x <= 0);
	} else {
		y = -Radius2; 
		x = 0;
		do { 
			K = (float)(rad2 / rad1);
			*(__IO uint16_t*) (VB_Addr + (((Xpos - (uint16_t)(x / K)) + 240 * (Ypos + y)))) = color; 
			*(__IO uint16_t*) (VB_Addr + (((Xpos + (uint16_t)(x / K)) + 240 * (Ypos + y)))) = color;
			*(__IO uint16_t*) (VB_Addr + (((Xpos + (uint16_t)(x / K)) + 240 * (Ypos - y)))) = color;
			*(__IO uint16_t*) (VB_Addr + (((Xpos - (uint16_t)(x / K)) + 240 * (Ypos - y)))) = color;
				
			e2 = err;
			if (e2 <= x) {
				err += ++x * 2 + 1;
				if (-y == x && e2 <= y) {
					e2 = 0;
				}
			}
			
			if (e2 > y) {
				err += ++y * 2 + 1;
			}
		} while (y <= 0);
	}
}

void draw_rect_1(int x, int y, int w, int h, uint16_t color)
{
	draw_line(x, y, x,  y + h, color);
	draw_line(x, y, x + w, y, color);
	draw_line(x, y + h, x + w, y + w, color);
	draw_line(x + w, y, x + w, y + h, color);
}

void draw_grid(void)
{
	int i;

	for (i = 0; i < 33; i++) {
		int x = i * 7;
		int y = i * 9;
		draw_line(x + 8, 16, x + 8, 304, RGB565CONVERT(16, 16, 16));
		draw_line(8, y + 16, 232, y + 16, RGB565CONVERT(16, 16, 16));
	}

	for (i = 0; i < 9; i++) {
		int x = i * 28;
		int y = i * 36;
		draw_line(x + 8, 16, x + 8, 304, RGB565CONVERT(32, 32, 32));
		draw_line(8, y + 16, 232, y + 16, RGB565CONVERT(32, 32, 32));
	}

	for (i = 0; i < 3; i++) {
		int x = i * 112;
		int y = i * 144;
		draw_line(x + 8, 16, x + 8, 304, RGB565CONVERT(64, 64, 64));
		draw_line(8, y + 16, 232, y + 16, RGB565CONVERT(64, 64, 64));
	}
	draw_ellipse(120, 160, 120, 160, 0xFFFF);
//	draw_pixel(50, 50, 0xFFFF);
//	draw_line(30, 30, 30, 70, 0xFFFF);
//	for (i = 0; i < 13; i++) {
//		draw_line(i + 9, 18, i + 9, 159, 0);
//	}
}

void draw_curve(int *curve, uint16_t color)
{
	int i;
	for (i = 0; i < 288; i++) {
		int x = *(curve + i) + 120;
		int x1 = *(curve + i + 1) + 120;
		int y = i + 16;
		int y1 = y + 1;
		draw_line(x, y, x1, y1, color);
	}
}

void init_display(void)
{
	ili9341_init();
	LTDC_Init();
}

