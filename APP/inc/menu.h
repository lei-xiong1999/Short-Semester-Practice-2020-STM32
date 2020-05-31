#ifndef __MENU_H
#define __MENU_H

#include "GUI.h"

typedef struct {
	uint8_t flagIsMainMenu;
	uint8_t page_index;
	uint8_t sub_page_index;
	uint8_t page_index_depth;
	
	void (*present_constructor_static)();
	void (*present_constructor_dynamic)();
	
}menu_t;

void menu_init(void);

#endif
