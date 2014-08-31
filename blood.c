#include <math.h>
#include "app.h"
#include "SDL_gfxPrimitives.h"

#define BLOOD_DROPS_PER_SPLASH 5
#define BLOOD_DRIPPING 100

typedef struct {
	Point pos;
	int radius;
	float speed;
	float distance;
	int active;
	int frame;
} Drop;

Drop drops[BLOOD_DRIPPING];

void blood_move(App *app) {
	int i;


	if(app->blood_x != app->map_x
	|| app->blood_y != app->map_y) { // move blood
		int delta_x = app->map_x - app->blood_x;
		int delta_y = app->map_y - app->blood_y;
		app->blood_x = app->map_x;
		app->blood_y = app->map_y;

		SDL_Rect src = {
			delta_x,
			delta_y,
			app->blood->w - delta_x,
			app->blood->h - delta_y
		};

		SDL_FillRect(app->blood_off, NULL, 0);

		SDL_SetAlpha(app->blood, 0, 0xff);
		SDL_BlitSurface( app->blood, &src, app->blood_off, NULL );
		SDL_BlitSurface( app->blood_off, NULL, app->blood, NULL );
		SDL_SetAlpha(app->blood, SDL_SRCALPHA, 0xff);
	}


	for(i = 0; i < BLOOD_DRIPPING; i++) {
		if(!drops[i].active) continue;

		// skip offscreen
		int offscreen = drops[i].pos.x - app->map_x - drops[i].radius < 0
		|| drops[i].pos.x - app->map_x + drops[i].radius > app->screen->w
		|| drops[i].pos.y - app->map_y - drops[i].radius < 0
		|| drops[i].pos.y - app->map_y + drops[i].radius > app->screen->h
		;
		
		int moving_drop = 1;
		int first = drops[i].frame == 0;
		if(!offscreen) {
			drops[i].frame++;
			if(drops[i].speed > 0) {
				drops[i].distance += drops[i].speed;
				drops[i].speed -= 0.01 / (pow(drops[i].radius, 4) * (pow(10, -5)));
			} else {
				moving_drop = 0;
			}
		}
		if(drops[i].radius > 16 || first) {
			filledCircleRGBA(app->blood, 
							 drops[i].pos.x - app->blood_x, 
							 drops[i].pos.y - app->blood_y 
								+ (int)drops[i].distance, 
							first ?  drops[i].radius :
							(moving_drop 
								? (drops[i].radius - (pow(drops[i].radius, 0.5) * 3.7)) 
								: (drops[i].radius - (pow(drops[i].radius, 0.5) * 3.7) + 1)), 
							 0xfe, 0, 0, 0xff);
		}
		drops[i].active = moving_drop;
	}
}


void blood_spawn(App *app, Point pos) {
	int i, j;
	for(i = 0; i < BLOOD_DROPS_PER_SPLASH; i++) {
		for(j = 0; j < BLOOD_DRIPPING; j++) {
			if(!drops[j].active) {
				drops[j].active = 1;
				drops[j].frame = 0;
				float angle = (rand() % 1000)/1000.0 * 2 * M_PI;
				float dist  = (rand() % 50);
				drops[j].pos.x = pos.x + cos(angle) * dist;
				drops[j].pos.y = pos.y + sin(angle) * dist;
				drops[j].radius = 5 + (rand() % 20);
				drops[j].speed = 1.0;
				drops[j].distance = 0.0;
				break;
			}
		}
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

	app->blood_x = app->map_x;
	app->blood_y = app->map_y;

	memset(drops,0,sizeof(drops));
}

void blood_render(App *app) {
	SDL_BlitSurface( app->blood, NULL, app->screen, NULL );
}

