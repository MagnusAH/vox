#include "gui_value.h"

/* 
	value update functions
*/

// raw value of source
GUI_value_update_t GUI_value_raw(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source;
}

// source (in pixels) as an OpenGL distance on the x axis
GUI_value_update_t GUI_size_px_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->iwidth * 2;
}

// source (in pixels) as an OpenGL distance on the y axis
GUI_value_update_t GUI_size_px_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->iheight * 2;
}

// source distance on the x axis as the same pixel distance on the y axis
GUI_value_update_t GUI_size_width_as_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->iratio;	
}

// source distance on the y axis as the same pixel distance on the x axis
GUI_value_update_t GUI_size_height_as_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->ratio; 
}

// source (in pixels) as an OpenGL position on the x axis
GUI_value_update_t GUI_position_px_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->iwidth * 2 - 1 + region->x;
}

// source (in pixels) as an OpenGL position on the y axis
GUI_value_update_t GUI_position_px_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->iheight * 2 - 1 + region->y;
}

// source position on the x axis as the same pixel position on the y axis
GUI_value_update_t GUI_position_width_as_height(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->iratio + region->y; 
}

// source position on the y axis as the same pixel position on the x axis
GUI_value_update_t GUI_position_height_as_width(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->cache = value->source * framebuffer->ratio + region->x;
}


/*
	compound value update functions
*/

// add source_a and source_b
GUI_compound_value_update_t GUI_compound_size_add(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_a->cache + value->source_b->cache;
}

GUI_compound_value_update_t GUI_compound_position_add(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_a->cache + value->source_b->cache + 1;
}

// subtract source_a from source_b
GUI_compound_value_update_t GUI_compound_size_sub_a(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_b->cache - value->source_a->cache;
}

GUI_compound_value_update_t GUI_compound_position_sub_a(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_b->cache - value->source_a->cache; // NOT SURE
}

// subtract source_b from source_a
GUI_compound_value_update_t GUI_compound_size_sub_b(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_a->cache - value->source_b->cache;
}

GUI_compound_value_update_t GUI_compount_position_sub_b(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_a->cache - value->source_b->cache; // NOT SURE
}

// the minimum between source_a and source_b
GUI_compound_value_update_t GUI_compound_min(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_a->cache < value->source_b->cache ? value->source_a->cache : value->source_b->cache;
}

// the maximum between source_a and source_b
GUI_compound_value_update_t GUI_compound_max(GUI_value_t* value, GUI_framebuffer_t* framebuffer, GUI_region_t* region)
{
	value->source_a->update(value->source_a, framebuffer, region);
	value->source_b->update(value->source_a, framebuffer, region);
	value->cache = value->source_a->cache > value->source_b->cache ? value->source_a->cache : value->source_b->cache;
}