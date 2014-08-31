#include "app.h"
#include "data/missile.h"

void missile_init(App *app) {

	sprite_init(&app->sprite_missile,
		0, 0, // origin
		16, 16, // frame size
		1, 4, // action, frame counts
		missile_png, missile_png_len, // source
		1
	);


}

void missile_spawn(App *app) {

	int i = (app->missile_index++) % MAX_MISSILES;
	app->missile[i].active = 1;
	app->missile[i].pos.x = app->ninja.pos.x;
	app->missile[i].pos.y = app->ninja.pos.y;

	app->missile[i].speed.x =
		(app->ninja.dir == DIR_LEFT ? -1 : +1) 
		* app->sprite_zombie.target_frame_size.x/7;

	// shoot parallel to the ground (for ramps)
	int forward_x = app->missile[i].pos.x;
	int forward_y = 0;
	int j, c=0, n = 4;
	int y = app->missile[i].pos.y;
	for(j=0; j<n; j++) {
		forward_x += app->missile[i].speed.x * TILE_SIZE;
		forward_y += map_y(app, forward_x);
		c++;
		n = forward_y/c < y ? 8 : 4;
	}
	forward_y /= c;
	int delta_y = forward_y - y;
	delta_y /= 8;

	app->missile[i].speed.y = -app->screen->h/60 + (rand() % (app->screen->h/90)) + delta_y;
	app->missile[i].sprite = &app->sprite_missile;
}

void missile_move(App *app) {
	int i, j;
	for(i = 0; i < MAX_MISSILES; i++) {
		if(!app->missile[i].active) continue;
		app->missile[i].speed.y += 2;
		app->missile[i].pos.x += app->missile[i].speed.x;
		app->missile[i].pos.y += app->missile[i].speed.y;

		int ceil = map_y0(app, app->missile[i].pos.x);
		int floor = map_y1(app, app->missile[i].pos.x);
		if(app->missile[i].pos.y < ceil
		|| app->missile[i].pos.y > floor) {
			app->missile[i].active = 0;
		}

		// hit enemies
		for(j=0; j<MAX_ZOMBIES; j++) {
			Body *z = &app->zombie[j];
			if(!z->active) continue;

			int dist = ABS(app->missile[i].pos.x - app->zombie[j].pos.x);
			if(dist < app->sprite_zombie.target_frame_size.x/4) {
				app->missile[i].active = 0;
				blood_spawn(app, app->zombie[j].pos);
				app->zombie[j].health --;
				continue;
			}
		}
	}

}

void missile_render(App *app) {
	int i;
	for(i = 0; i < MAX_MISSILES; i++) {
		if(!app->missile[i].active) continue;
		body_render(app, &app->missile[i]);
	}
}
