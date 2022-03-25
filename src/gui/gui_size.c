#include "gui_size.h"

GUI_value_update_t GUI_size_px_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 * framebuffer->iwidth;	
}

GUI_value_update_t GUI_size_px_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 * framebuffer->iheight;
}

GUI_value_update_t GUI_size_pct_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2;
}

GUI_value_update_t GUI_size_pct_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2;
}

GUI_value_update_t GUI_size_pct_width_as_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 * framebuffer->ratio;
}

GUI_value_update_t GUI_size_pct_height_as_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 * framebuffer->iratio;
}

GUI_value_update_t GUI_position_px_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source	* 2 * framebuffer->iwidth - 1;
}

GUI_value_update_t GUI_position_px_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 * framebuffer->iheight - 1;
}

GUI_value_update_t GUI_position_pct_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 - 1;
}

GUI_value_update_t GUI_position_pct_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 - 1;
}

GUI_value_update_t GUI_position_pct_width_as_height(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 * framebuffer->ratio - 1;
}

GUI_value_update_t GUI_position_pct_height_as_width(GUI_value_t* size, GUI_framebuffer_t* framebuffer)
{
	size->cached = size->source * 2 * framebuffer->iratio - 1;
}