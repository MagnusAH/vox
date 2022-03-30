#pragma once

#include <stdint.h>

// 16 bytes
// width and height in pixels
struct GUI_framebuffer
{
	float ratio;   // framebuffer aspect ratio (width / height)
	float iwidth;  // framebuffer inverse width (1 / width)
	float iheight; // framebuffer inverse height (1 / height)
	float iratio;  // framebuffer inverse aspect ratio (height / width)
};

typedef struct GUI_framebuffer GUI_framebuffer_t;

// 16 bytes
// x, y, width, height in OpenGL system
struct GUI_region
{
	float x;      // x (lower left) in OpenGL coords (-1 to 1)
	float y;      // y (lower left) in OpenGL coords (-1 to 1)
	float width;  // width in OpenGL distance (0 to 2)
	float height; // height in OpenGL distance (0 to 2)
};

typedef struct GUI_region GUI_region_t;

// called when framebuffer or region is resized
typedef void (*GUI_value_update_t)(struct GUI_value* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// called when framebuffer or region is resized
typedef void (*GUI_compound_value_update_t)(struct GUI_value* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// 32 bytes
struct GUI_value
{
	GUI_value_update_t update;
	float cache;
	union {
		float source;
		struct {
			struct GUI_value* source_a;
			struct GUI_value* source_b;
		};
	};
};

typedef struct GUI_value GUI_value_t;

struct GUI_value
{
	GUI_value_update_t update;
	float cache;
	
}


inline void GUI_init_value(GUI_value_t* value, float source, GUI_value_update_t func)
{
	value->source = source;
	value->update = func;
}

inline void GUI_init_compound_value(GUI_value_t* value, GUI_value_t* source_a, GUI_compound_value_update_t func, GUI_value_t* source_b)
{
	value->source_a = source_a;
	value->source_b = source_b;
	value->update = func;
}

/* 
	value update functions
*/

// raw value of source
GUI_value_update_t GUI_value_raw(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// source (in pixels) as an OpenGL distance on the x axis
GUI_value_update_t GUI_size_px_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
// source (in pixels) as an OpenGL distance on the y axis
GUI_value_update_t GUI_size_px_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// source distance on the x axis as the same pixel distance on the y axis
GUI_value_update_t GUI_size_width_as_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
// source distance on the y axis as the same pixel distance on the x axis
GUI_value_update_t GUI_size_height_as_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// source (in pixels) as an OpenGL position on the x axis
GUI_value_update_t GUI_position_px_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
// source (in pixels) as an OpenGL position on the y axis
GUI_value_update_t GUI_position_px_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// source position on the x axis as the same pixel position on the y axis
GUI_value_update_t GUI_position_width_as_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
// source position on the y axis as the same pixel position on the x axis
GUI_value_update_t GUI_position_height_as_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

/*
	compound value update functions
*/

// add source_a and source_b
GUI_compound_value_update_t GUI_compound_size_add(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
GUI_compound_value_update_t GUI_compound_position_add(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// subtract source_a from source_b
GUI_compound_value_update_t GUI_compound_size_sub_a(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
GUI_compound_value_update_t GUI_compound_position_sub_a(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// subtract source_b from source_a
GUI_compound_value_update_t GUI_compound_size_sub_b(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
GUI_compound_value_update_t GUI_compount_position_sub_b(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);

// the minimum between source_a and source_b
GUI_compound_value_update_t GUI_compound_min(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);
// the maximum between source_a and source_b
GUI_compound_value_update_t GUI_compound_max(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region);