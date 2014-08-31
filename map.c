#include <math.h>
#include "app.h"
#include "SDL_gfxPrimitives.h"

#define BLOOD_DROPS 5

typedef struct {
	Point pos;
	int radius;
} Drop;

Drop drops[MAX_ZOMBIES * BLOOD_DROPS];
int number_of_drops = 0;

void draw_blood_flow(App *app) {
	int i;
	for(i = 0; i < (number_of_drops % (MAX_ZOMBIES * BLOOD_DROPS)); i++) {
		drops[i].pos.y++;

		filledCircleRGBA(app->blood, drops[i].pos.x - app->heightmap_x, drops[i].pos.y, drops[i].radius - 7, 0xfe, 0, 0, 0xfe);
	}
}

void draw_blood(App *app) {
	int i;
	for(i = 0; i < BLOOD_DROPS; i++) {
		int hx = (rand() % 50) * ((rand() % 2) ? 1 : (-1));
		int hy = (rand() % 50) * ((rand() % 2) ? 1 : (-1));
		int radius = 5 + (rand() % 20);
		Drop drop;
		drop.pos.x = app->screen->w/2 + hx;
		drop.pos.y = app->screen->h/3 + hy;
		drop.radius = radius;
		//aacircleRGBA(app->blood, app->screen->w/2 + hx, app->screen->h/3 + hy, radius, 0xfe, 0, 0, 0xfe);
		filledCircleRGBA(app->blood, drop.pos.x, drop.pos.y, drop.radius, 0xfe, 0, 0, 0xfe);
		drops[number_of_drops] = drop;
		number_of_drops++;
	}
}

void map_init(App *app) {
	int i;
	for(i=0; i<MAP_SIZE; i++) {
		app->heightmap[i] = 0;
	}

	app->map_step_x = 0;
	app->map_step_delay = app->screen->w;

    app->blood = SDL_CreateRGBSurface(SDL_HWSURFACE, 
            app->screen->w,
            app->screen->h,
            RGBA_FORMAT);

    SDL_SetAlpha(app->blood,SDL_SRCALPHA,0xff);
	SDL_FillRect(app->blood, NULL, 0x00000000);

    app->blood_off = SDL_CreateRGBSurface(SDL_HWSURFACE, 
            app->blood->w,
            app->blood->h,
            RGBA_FORMAT);

    SDL_SetAlpha(app->blood_off,0,0xff);
    draw_blood(app);
}

void map_move(App *app) {
	int i;
	
	if(app->ninja.pos.y - app->map_y > app->screen->h * 0.7  ) {
		app->map_y += app->screen->h/50;
	}

	if(app->ninja.pos.y - app->map_y < app->screen->h * 0.4  ) {
		app->map_y -= app->screen->h/50;
	}

	while(app->ninja.pos.x - app->map_x > app->screen->w * 0.6) {
		app->map_x ++;

		if(app->map_x - app->heightmap_x > TILE_SIZE) {
			app->heightmap_x += TILE_SIZE;
		
			for(i=0; i<MAP_SIZE-1; i++) {
				app->heightmap[i] = app->heightmap[i+1];
			}

			{ // move blood
				SDL_Rect src = {
					TILE_SIZE,
					0,
					app->blood->w - TILE_SIZE,
					app->blood->h
				};

				SDL_Rect dst = {
					0,
					0,
					app->blood->w - TILE_SIZE,
					app->blood->h
				};

				SDL_Rect clear = {
					app->blood->w - TILE_SIZE,
					0,
					TILE_SIZE,
					app->blood->h
				};
				SDL_FillRect(app->blood_off, &clear, 0);

				SDL_SetAlpha(app->blood, 0, 0xff);
				SDL_BlitSurface( app->blood, &src, app->blood_off, &dst );
				SDL_BlitSurface( app->blood_off, NULL, app->blood, NULL );
				SDL_SetAlpha(app->blood, SDL_SRCALPHA, 0xff);
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
	SDL_BlitSurface( app->blood, NULL, app->screen, NULL );

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

	draw_blood_flow(app);
}