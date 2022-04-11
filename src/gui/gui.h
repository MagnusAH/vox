#pragma once

#include "../core/dynamic_array.h"
#include "../core/string.h"

#include "gui_value.h"

// 24 bytes
struct GUI_framebuffer
{
	float width;
	float height;
	float ratio;
	float iwidth;
	float iheight;
	float iratio;
};
typedef struct GUI_framebuffer GUI_framebuffer_t;



// 136 bytes
struct GUI_root // "window"
{
	// 128 bytes
	GUI_value_t x;
	GUI_value_t y;
	GUI_value_t width;
	GUI_value_t height;

	// 8 bytes
	struct GUI_region* children; 
	
	// 48 bytes
	dynamic_array_t vertices; // list of vertices
	dynamic_array_t indices; // list of visible indices, sorted by layer
	dynamic_array_t ivis; // list of (indice count, visible), sorted by layer, used for updating

	// 16 bytes
	dynamic_array_t text; // list of text elements

	// 8 bytes
	uint8_t hoverable;     // elements can support change on hover, can intercept hover events from reaching lower elements
	uint8_t interactable;  // elements can support interaction
	uint8_t solid;         // elements can block other elements from being clicked
	uint8_t visible;       // is root visible, attempt to draw anything in it
	
	uint8_t verts_damaged;   // set when vertices of an element have changed 
	uint8_t indices_damaged; // set when visibility of an element has changed
	
	uint16_t layer;        // for root drawing order
};
typedef struct GUI_root GUI_root_t;

// 152 bytes
struct GUI_region // region for alignment, allows for coordinate mapping
{
	// 24 bytes
	struct GUI_region* children;
	struct GUI_region* next;
	struct GUI_region* prev;

	// 128 bytes
	GUI_value_t x;
	GUI_value_t y;
	GUI_value_t width;
	GUI_value_t height;
};
typedef struct GUI_region GUI_region_t;

// called when gui element gains cursor hover
typedef void (*GUI_on_hover_t)(void* self, float x, float y);

// called when gui element looses cursor hover
typedef void (*GUI_on_unhover_t)(void* self, float x, float y);

// called when element gains focus
typedef void (*GUI_on_focus_t)(void* self);

// called when element looses focus
typedef void (*GUI_on_unfocus_t)(void* self);

// called when element is clicked
typedef void (*GUI_on_click_t)(void* self, float x, float y, uint32_t state);

// called when a key is pressed and the element is focused
typedef void (*GUI_on_key_t)(void* self, uint32_t key, uint32_t scancode, uint32_t action, uint32_t mods);

// called when rendering element, vertices in format (x, y, s, t, r, g, b, a) 32 bytes per vert
typedef void (*GUI_push_verts_t)(void* self, float* verts, uint16_t* indices, uint16_t* v_index uint16_t* i_index);

// called to check if cursor at (x, y) is inside bounding box
typedef uint32_t (*GUI_bounds_check_t)(void* self, float x, float y);

// called when region sizes change
typedef uint32_t (*GUI_update_values_t)(void* self, GUI_region_t* region, GUI_framebuffer_t* framebuffer);

// 104 bytes
struct GUI_element
{
	// 8 bytes
	uint8_t text;                 // is element text (separate rendering pipeline, always on top of other elements sharing the same root)
	uint8_t hoverable;            // supports change on hover
	uint8_t interactable;         // supports interacting via clicking, therefore is focusable
	uint8_t solid;                // does element block other elements from being clicked
	uint8_t vert_count;           // number of vertices, max 255
	
	uint8_t visible;              // should element be rendered

	uint16_t layer;               // layer, 0 is lowest (can be blocked by anything)

	// 48 bytes
	GUI_on_hover_t on_hover;      // called when element gains hover
	GUI_on_unhover_t on_unhover;  // called when element looses hover
	GUI_on_focus_t on_focus;      // called when element gains focus
	GUI_on_unfocus_t on_unfocus;  // called when element looses focus
	GUI_on_click_t on_click;      // called when element is clicked
	GUI_on_key_t on_key;          // called when element is focused and a key is pressed

	// 16 bytes
	GUI_push_verts_t push_verts;  // called when rendering element
	GUI_update_values_t* update_values; // called to update size/position values

	// 32 bytbes
	struct GUI_element* next;
	struct GUI_element* prev;
	GUI_region_t* region;
	GUI_root_t* root;
};
typedef struct GUI_element GUI_element_t;

// 184 bytes
struct GUI_text
{
	// 128 bytes
	GUI_value_t x;
	GUI_vlaue_t y;
	GUI_value_t width;
	GUI_value_t height;
	
	// 32 bytes
	struct GUI_text* next;
	struct GUI_text* prev;
	GUI_region_t* region;
	GUI_root_t* root;
	
	// 16 bytes
	string4b_t string; // utf-32 string
};
typedef struct GUI_text GUI_text_t;

/*
GUI_element
{
	flags

	callbacks

	push_verts

	// end of cache line
	GUI_region* region
	
	update_points
}

*/