#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Defines ----------------------------------------------------------
#define C_RED			0xF800
#define C_DARK_RED		0x8000
#define C_GREEN			0x07E0
#define C_DARK_GREEN	0x0400
#define C_BLUE			0x001F
#define C_DARK_BLUE		0x0010
#define C_CYAN			0x07FF
#define C_DARK_CYAN		0x0410
#define C_MAGENTA		0xF81F
#define C_DARK_MAGENTA	0x8010
#define C_YELLOW		0xFFE0
#define C_DARK_YELLOW	0x8400
#define C_BLACK 		0x0000
#define C_GRAY			0xC618
#define C_DARK_GRAY		0x8410
#define C_WHITE 		0xFFFF


#define RGB565CONVERT(red, green, blue) \
	(int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))


extern const char pixfont8x12[256][12];


int display_draw_text_8x12(int x, int y, const char *str, uint16_t color);
void draw_pixel(int x, int y, uint16_t color);
void draw_line(int start_x, int start_y,
	int end_x, int end_y , unsigned short color);
void draw_grid(void);
void draw_curve(int *curve, uint16_t color);
void init_display(void);


#endif // DISPLAY_H
