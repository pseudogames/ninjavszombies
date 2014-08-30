#include <math.h>
#include "app.h"

void map_init(App *app) {
	int i;
	for(i=0; i<MAP_SIZE; i++) {
		app->heightmap[i] = sin(i*0.1) * 100;
	}
}

void map_move(App *app) {
	int px = app->ninja.pos.x;
	int mx = app->map_x;
	int sw3 = app->screen->w/3;
	int d = px - mx;
#if 0
	if(d < -sw3) {
		app->map_x -= ZOOM;
	}
#endif
	if(d > sw3) {
		app->map_x += ZOOM;
	}
}

int map_y(App *app, int x) {
	return app->heightmap[
		CLAMP(MAP_SIZE / 2 + (x - app->map_x)/ZOOM, 0, MAP_SIZE-1)
	];
}

void map_render(App *app) {
	int sx;
	Uint32 color = SDL_MapRGB(app->screen->format, 0x00, 0x00, 0x00 );
	for(sx=0; sx<app->screen->w; sx+=ZOOM) {
		int sw = app->screen->w;
		int sh = app->screen->h;
		int x = sx - sw/2;
		int y = sh/2 + (int)map_y(app, x);
		int h0 = y - app->sprite_ninja.target_frame_size.y;
		int h1 = y + app->sprite_ninja.target_frame_size.y/2;
		SDL_Rect rect_top    = { sx, 0, ZOOM, h0 };
		SDL_Rect rect_bottom = { sx, h1 , ZOOM,  sh - h1};
		SDL_FillRect(app->screen, &rect_top    , color);
		SDL_FillRect(app->screen, &rect_bottom , color);
	}
}
