#include "app.h"
#include "data/missile1.h"
#include "data/missile2.h"

int last = 0;

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

	app->missile[last % MAX_MISSILES].pos.x = app->ninja.pos.x;
	app->missile[last % MAX_MISSILES].pos.y = app->ninja.pos.y;
	app->missile[last % MAX_MISSILES].speed = -40 + (rand() % 20);
	app->missile[last % MAX_MISSILES].sprite = &app->sprite_missile1;
	last++;
}

void missile_move(App *app) {
	int i;
	for(i = 0; i < (last < MAX_MISSILES ? (last) : (last % MAX_MISSILES)); i++) {
		app->missile[i].pos.x += 25;
		app->missile[i].speed += 5;
		app->missile[i].pos.y = app->missile[i].pos.y + app->missile[i].speed;
	}
}

void missile_render(App *app) {
	int i;
	for(i = 0; i < (last < MAX_MISSILES ? (last) : (last % MAX_MISSILES)); i++) {
		body_render(app, &app->missile[i]);
	}
}