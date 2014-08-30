#include <math.h>
#include "app.h"


void map_move(App *app) {
}

float map_y(App *app, float x) {
	return sin(x);
}

void map_render(App *app) {
	int x;
	for(x=0; x<app->screen->w; x+=16) {
		int y = map_y(app,x);
	}
}
