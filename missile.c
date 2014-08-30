#include "app.h"
#include "data/missile1.h"
#include "data/missile2.h"

void missile_init(App *app) {

	sprite_init(&app->sprite_missile1,
		0, 0, // origin
		16, 16, // frame size
		1, 4, // action, frame counts
		missile1_png, missile1_png_len // source
	);

	sprite_init(&app->sprite_missile2,
		0, 0, // origin
		16, 16, // frame size
		1, 4, // action, frame counts
		missile2_png, missile2_png_len // source
	);

}

void missile_spawn(App *app) {

	int i = (app->missile_index++) % MAX_MISSILES;
	app->missile[i].active = 1;
	app->missile[i].pos.x = app->ninja.pos.x;
	app->missile[i].pos.y = app->ninja.pos.y;

	app->missile[i].speed.x =
		(app->ninja.dir == DIR_LEFT ? -1 : +1) * app->screen->w/30;

	app->missile[i].speed.y = -app->screen->h/60 + (rand() % (app->screen->h/90));
	app->missile[i].sprite = &app->sprite_missile1;
}

void missile_move(App *app) {
	int i;
	for(i = 0; i < MAX_MISSILES; i++) {
		if(!app->missile[i].active) continue;
		app->missile[i].speed.y += 2;
		app->missile[i].pos.x += app->missile[i].speed.x;
		app->missile[i].pos.y += app->missile[i].speed.y;
	}
}

void missile_render(App *app) {
	int i;
	for(i = 0; i < MAX_MISSILES; i++) {
		if(!app->missile[i].active) continue;
		body_render(app, &app->missile[i]);
	}
}
