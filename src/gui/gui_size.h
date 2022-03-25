#pragma once

#include <stdint.h>

// 24 bytes
struct GUI_framebuffer
{
	float width;   // framebuffer width
	float height;  // framebuffer height
	float ratio;   // framebuffer aspect ratio (width / height)
	float iwidth;  // framebuffer inverse width (1 / width)
	float iheight; // framebuffer inverse height (1 / height)
	float iratio;  // framebuffer inverse aspect ratio (height / width)
};

typedef struct GUI_framebuffer GUI_framebuffer_t;

// called when framebuffer is resized
typedef void (*GUI_value_update_t)(struct GUI_value* size, GUI_framebuffer_t* framebuffer);

// 16 bytes
struct GUI_value
{
	float cached;
	float source;
	GUI_value_update_t update;
};

typedef struct GUI_value GUI_value_t;

GUI_value_update_t GUI_size_px_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer);
GUI_value_update_t GUI_size_px_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer);

GUI_value_update_t GUI_size_pct_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer);
GUI_value_update_t GUI_size_pct_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer);

GUI_value_update_t GUI_size_pct_width_as_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer);
GUI_value_update_t GUI_size_pct_height_as_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer);

GUI_value_update_t GUI_position_px_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer);
GUI_value_update_t GUI_position_px_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer);

GUI_value_update_t GUI_position_pct_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer);
GUI_value_update_t GUI_position_pct_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer);

GUI_value_update_t GUI_position_pct_width_as_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer);
GUI_value_update_t GUI_position_pct_height_as_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer);