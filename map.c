#include <math.h>
#include "app.h"

void map_init(App *app) {
	int i;
	for(i=0; i<MAP_SIZE; i++) {
		app->heightmap_ceil[i] =
		app->heightmap[i] = 0;
	}

	app->map_x = 0;
	app->map_y = -app->screen->h/4;
}

void map_move(App *app) {
	int i;
	
	if(app->ninja.pos.y - app->map_y > app->screen->h * 0.7
	&& app->ninja.dir == DIR_RIGHT) {
		app->map_y ++;
	}

	if(app->ninja.pos.y - app->map_y < app->screen->h * 0.4
	&& app->ninja.dir == DIR_RIGHT) {
		app->map_y --;
	}

	while(app->ninja.pos.x - app->map_x > app->screen->w * 0.6) {
		app->map_x ++;

		if(app->map_x - app->heightmap_x > TILE_SIZE) {
			app->heightmap_x += TILE_SIZE;
		
			for(i=0; i<MAP_SIZE-1; i++) {
				app->heightmap[i] = app->heightmap[i+1];
				app->heightmap_ceil[i] = app->heightmap_ceil[i+1];
			}


			int ramp = 0;
			for(i=0; i<MAP_SIZE-1; i++) {
				if(ABS(app->heightmap[i] - app->heightmap[i+1]) >= TILE_SIZE) {
					ramp = 1;
					break;
				}
			}
			int p0 = app->heightmap[MAP_SIZE-3];
			int p1 = app->heightmap[MAP_SIZE-2];
			int p2 = p1 + (rand() % TILE_SIZE) - TILE_SIZE/2;
			int p2c = app->heightmap_ceil[MAP_SIZE-2]
				+ (rand() % TILE_SIZE) - TILE_SIZE/2;
			if(ramp) {
				if(ABS(p0 - p1) >= TILE_SIZE) {
					if((rand() % (MAP_SIZE/4)) != 0) {
						p2 = p1 + ((p1 - p0) / TILE_SIZE) * TILE_SIZE;
						p2c = (p2c * 0.8 + p2 * 0.2) / TILE_SIZE * TILE_SIZE;
					}
				}
			} else {
				if((rand() % (MAP_SIZE/8)) == 0) {
					p2 = p1 + TILE_SIZE *  ((rand() % 2) ? +1 : -1);
					p2c = (p2c * 0.8 + p2 * 0.2) / TILE_SIZE * TILE_SIZE;
				}
			}
			app->heightmap[MAP_SIZE-1] = p2;
			app->heightmap_ceil[MAP_SIZE-1] = p2c;
		}
	}

}

inline int map_y(App *app, int x) {
	return app->heightmap[
		CLAMP((x - app->map_x)/TILE_SIZE, 0, MAP_SIZE-1)
	];
}

inline int map_y0(App *app, int x) {
	return app->heightmap_ceil[
		CLAMP((x - app->map_x)/TILE_SIZE, 0, MAP_SIZE-1)
	] - app->sprite_ninja.target_frame_size.y;
}
inline int map_y1(App *app, int x) {
	return app->heightmap[
		CLAMP((x - app->map_x)/TILE_SIZE, 0, MAP_SIZE-1)
	] + app->sprite_ninja.target_frame_size.y * 0.475;
}

void map_render(App *app) {
	int sx;
	Uint32 color = SDL_MapRGB(app->screen->format, 0x00, 0x00, 0x00 );
	for(sx=0; sx<app->screen->w; sx+=TILE_SIZE) {
		int sh = app->screen->h;
		int x = app->map_x + sx;
		int y0 = CLAMP(map_y0(app, x) - app->map_y, 0, app->screen->h-1);
		int y1 = CLAMP(map_y1(app, x) - app->map_y, 0, app->screen->h-1);
		SDL_Rect rect_top    = { sx, 0,  TILE_SIZE, y0 };
		SDL_Rect rect_bottom = { sx, y1, TILE_SIZE, sh - y1};
		SDL_FillRect(app->screen, &rect_top    , color);
		SDL_FillRect(app->screen, &rect_bottom , color);
	}
}

