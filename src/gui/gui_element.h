#pragma once

#include <stdint.h>

#include "gui_bounds.h"

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
typedef void (*GUI_on_key_t)(void* self);

// called when text is typed and the element is focused
typedef void (*GUI_on_char_t)(void* self, uint32_t codepoint);


// 96 bytes 
struct GUI_element
{
	// ordering
	struct GUI_element* parent;   // parent element
	struct GUI_element* next;     // next sibling element
	struct GUI_element* children; // singly linked list of children (using next field)
	struct GUI_bounds* bounds;    // for checking bounding box (and getting position)
	
	// static flags
	uint8_t hoverable;            // supports change on hover
	uint8_t interactable;         // supports interacting via clicking, therefore is focusable
	uint8_t click_on_return;      // calls click event on return press when focused
	uint8_t solid;                // should this block elements beneath it
	
	// dynamic flags
	uint8_t visible;              // should element and child elements be rendered
	uint8_t opt;                  // not sure yet
	
	uint16_t layer;               // layer, 0 is lowest (can be blocked by anything)
	
	// function callbacks
	GUI_on_hover_t on_hover;
	GUI_on_unhover_t on_unhover;
	GUI_on_focus_t on_focus;
	GUI_on_unfocus_t on_unfocus;
	GUI_on_click_t on_click;
	GUI_on_key_t on_key;
	GUI_on_char_t on_char;
};

typedef struct GUI_element GUI_element_t;