#include <math.h>
#include "app.h"

void map_init(App *app) {
	int i;
	for(i=0; i<MAP_SIZE; i++) {
		app->heightmap[i] = i;
	}

	app->map_step_x = 0;
	app->map_step_delay = app->screen->w;
}

void map_move(App *app) {
	int i;
	
	if(app->ninja.pos.y - app->map_y > app->screen->h * 0.7  ) {
		app->map_y +=3;
	}

	if(app->ninja.pos.y - app->map_y < app->screen->h * 0.4  ) {
		app->map_y -=3;
	}

	while(app->ninja.pos.x - app->map_x > app->screen->w * 0.6) {
		app->map_x ++;

		if(app->map_x - app->heightmap_x > TILE_SIZE) {
			app->heightmap_x += TILE_SIZE;
		
			for(i=0; i<MAP_SIZE-1; i++) {
				app->heightmap[i] = app->heightmap[i+1];
			}

			int p0 = app->heightmap[MAP_SIZE-3];
			int p1 = app->heightmap[MAP_SIZE-2];
			int p2 = p1;
			if(p0 != p1) {
				if((rand() % 10) != 0) {
					p2 = p1 + (p1 - p0);
				}
			} else {
				if(app->map_x - app->map_step_x > app->map_step_delay
				&& (rand() % (MAP_SIZE/4)) == 0) {
					p2 = p1 + TILE_SIZE / 2 *  ((rand() % 2) ? +1 : -1);
					app->map_step_x = app->map_x;
				}
			}
			app->heightmap[i] = p2;
		}
	}
}

inline int map_y(App *app, int x) {
	return app->heightmap[
		CLAMP((x - app->map_x)/TILE_SIZE, 0, MAP_SIZE-1)
	];
}

inline int map_y0(App *app, int x) {
	return map_y(app, x) - app->sprite_ninja.target_frame_size.y;
}
inline int map_y1(App *app, int x) {
	return map_y(app, x) + app->sprite_ninja.target_frame_size.y/2;
}

void map_render(App *app) {
	int sx;
	Uint32 color = SDL_MapRGB(app->screen->format, 0x00, 0x00, 0x00 );
	for(sx=0; sx<app->screen->w; sx+=TILE_SIZE) {
		int sh = app->screen->h;
		int x = app->map_x + sx;
		int y0 = CLAMP(map_y0(app, x) - app->map_y, 0, app->screen->h-1);
		int y1 = CLAMP(map_y1(app, x) - app->map_y, 0, app->screen->h-1);
		SDL_Rect rect_top    = { sx, 0, TILE_SIZE, y0 };
		SDL_Rect rect_bottom = { sx, y1 , TILE_SIZE,  sh - y1};
		SDL_FillRect(app->screen, &rect_top    , color);
		SDL_FillRect(app->screen, &rect_bottom , color);
	}
}

