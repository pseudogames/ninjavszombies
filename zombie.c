#include "app.h"
#include "data/zombie.h"

void zombie_init(App *app) {

	sprite_init(&app->sprite_zombie,
		0, 0, // origin
		16, 16, // frame size
		ACTION_COUNT, 4, // action, frame counts
		zombie_png, zombie_png_len // source
	);

	app->zombie_spawn_time = SDL_GetTicks();
	app->zombie_spawn_delay = 2000;
}

void zombie_spawn(App *app) {
	int i;
	int t = SDL_GetTicks();
	if(t > app->zombie_spawn_time + app->zombie_spawn_delay) {
		//printf("search free zombie slot\n");
		app->zombie_spawn_time = t;
		app->zombie_spawn_delay *= 0.95;

		for(i=0; i<MAX_ZOMBIES; i++) {
			if(app->zombie[i].active) continue;
			//printf("spawn zombie %d\n", i);
			int w = app->screen->w / 2;
			app->zombie[i].pos.x = app->map_x + w * (rand() % 2 ? -1 : 1);
			app->zombie[i].pos.y = map_y(app, app->zombie[i].pos.x);
			app->zombie[i].sprite = &app->sprite_zombie;
			app->zombie[i].health = 10;
			app->zombie[i].active = 1;
			break;
		}
	}
}

void zombie_move(App *app) {
	int i;

	for(i=0; i<MAX_ZOMBIES; i++) {
		Body *z = &app->zombie[i];
		if(!z->active) continue;
		z->frame += 0.1;
		if(z->health < 0) {
			if(z->action != ACTION_DEATH) {
				z->action = ACTION_DEATH;
				z->frame = 0;
			} else {
				if(z->frame > z->sprite->frame_count) {
					z->active = 0;
				}
			}
			break;
		}
		Direction dir = app->ninja.pos.x < app->zombie[i].pos.x ? DIR_LEFT : DIR_RIGHT;
		app->zombie[i].dir = dir;
		app->zombie[i].pos.x += dir == DIR_LEFT ? -1 : +1;
		app->zombie[i].pos.y = map_y(app, app->zombie[i].pos.x);
	}

}

void zombie_render(App *app) {
	int i;

	for(i=0; i<MAX_ZOMBIES; i++) {
		Body *z = &app->zombie[i];
		if(!z->active) continue;
		body_render(app, z);
	}

}
