#include "app.h"
#include "data/ninja.h"
	
void ninja_init(App *app) {

	sprite_init(&app->sprite_ninja,
		0, 0, // origin
		80, 80, // frame size
		ACTION_COUNT, 6, // action, frame counts
		ninja_png, ninja_png_len // source
	);

	app->ninja.sprite = &app->sprite_ninja;

}

void ninja_spawn(App *app) {
	app->ninja.pos.x = app->screen->w/2;
	app->ninja.pos.y = map_y(app, app->ninja.pos.x);
	app->ninja.dir = DIR_RIGHT;
}

void ninja_move(App *app) {

	app->ninja.action = app->pressed[SDLK_SPACE] ? ACTION_ATTACK1 : ACTION_MOVE;

	if(app->pressed[SDLK_LEFT]
	&& app->ninja.pos.x - app->map_x > app->sprite_ninja.target_frame_size.x/2) {
		app->ninja.pos.x -= TILE_SIZE;
		app->ninja.dir=DIR_LEFT;
	}

	if(app->pressed[SDLK_RIGHT]) {
		app->ninja.pos.x += TILE_SIZE;
		app->ninja.dir=DIR_RIGHT;
	}

	if(app->pressed[SDLK_LEFT] || app->pressed[SDLK_RIGHT]) 
		app->ninja.pos.y = map_y(app, app->ninja.pos.x);

}

void ninja_render(App *app) {

	body_render(app, &app->ninja);

}
