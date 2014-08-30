#include "app.h"
#include "data/ninja.h"
	
void ninja_init(App *app) {

	sprite_init(&app->sprite_ninja,
		0, 0, // origin
		16, 16, // frame size
		ACTION_COUNT, 4, // action, frame counts
		ninja_png, ninja_png_len // source
	);

	app->ninja.pos.x = app->screen->w / 2;
	app->ninja.pos.y = app->screen->h / 2;
	app->ninja.sprite = &app->sprite_ninja;

}


void ninja_move(App *app) {

	app->ninja.action = app->pressed[SDLK_SPACE] ? ACTION_ATTACK1 : ACTION_MOVE;

}

void ninja_render(App *app) {

	body_render(app, &app->ninja);

}
