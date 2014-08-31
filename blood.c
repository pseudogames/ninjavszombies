#include <math.h>
#include "app.h"
#include "SDL_gfxPrimitives.h"

#define BLOOD_DROPS 5

typedef struct {
	Point pos;
	int radius;
	float speed;
	float distance;
} Drop;

Drop drops[MAX_ZOMBIES * BLOOD_DROPS];
int number_of_drops = 0;

void blood_move(App *app) {
	int i;
	for(i = 0; i < (number_of_drops % (MAX_ZOMBIES * BLOOD_DROPS)); i++) {
		int moving_drop = 1;
		if(drops[i].speed > 0) {
			drops[i].distance += drops[i].speed;
			drops[i].speed -= 0.01 / (pow(drops[i].radius, 4) * (pow(10, -5)));
		} else {
			moving_drop = 0;
		}
		if(drops[i].radius > 16) {
			filledCircleRGBA(app->blood, 
							 drops[i].pos.x - app->heightmap_x, 
							 drops[i].pos.y + (int)drops[i].distance, 
							 (moving_drop ? (drops[i].radius - (pow(drops[i].radius, 0.5) * 3.7)) : (drops[i].radius - (pow(drops[i].radius, 0.5) * 3.7) + 1)), 
							 0xfe, 
							 0, 
							 0, 
							 0xfe);
		}
	}
}

void blood_spawn(App *app, Point pos) {
	int i;
	for(i = 0; i < BLOOD_DROPS; i++) {
		int hx = (rand() % 50) * ((rand() % 2) ? 1 : (-1));
		int hy = (rand() % 50) * ((rand() % 2) ? 1 : (-1));
		int radius = 5 + (rand() % 20);
		Drop drop;
		drop.pos.x = app->screen->w/2 + hx;
		drop.pos.y = app->screen->h/3 + hy;
		drop.radius = radius;
		drop.speed = 1.0;
		drop.distance = 0.0;
		//aacircleRGBA(app->blood, app->screen->w/2 + hx, app->screen->h/3 + hy, radius, 0xfe, 0, 0, 0xfe);
		filledCircleRGBA(app->blood, drop.pos.x, drop.pos.y, drop.radius, 0xfe, 0, 0, 0xfe);
		drops[number_of_drops] = drop;
		number_of_drops++;
	}
}

void blood_init(App *app) {
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
    blood_spawn(app, app->ninja.pos);
}

void blood_render(App *app) {
	SDL_BlitSurface( app->blood, NULL, app->screen, NULL );
}

