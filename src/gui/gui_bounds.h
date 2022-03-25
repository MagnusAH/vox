#pragma once

#include <stdint.h>

// called to check if cursor at (x, y) is inside bounding box
typedef void (*GUI_bounds_check_t)(void* self, float x, float y);

// called when display is resized with (width, height) framebuffer dimensions
typedef void (*GUI_update_bounds_t)(void* self, float width, float height);

// 16 bytes
struct GUI_bounds
{
	GUI_bounds_chech_t check_bounds;
	GUI_update_bounds_t update_bounds;
	// extend with custom fields of GUI_size_t
}

typedef struct GUI_bounds GUI_bounds_t;

/* example bounding class

struct GUI_bounding_box
{
	GUI_bounds_t root;
	vec2f_t pos;
	vec2f_t	size;
};

GUI_bounds_check_t box_check_bounds(struct GUI_bounding_box* self, float x, float y)
{
	return (x > self->pos.x && x < self->pos.x + self->size.x) && (y > self->pos.y && y < self->pos.y + self->size.y);
}

void init_bounding_box(struct GUI_bounding_box* box, float x, float y, float width, float height)
{
	box->root.check_bounds = box_check_bounds;
	box->pos.x = x;
	box->pos.y = y;
	box->size.x = width;
	box->size.y = height;
}

*/