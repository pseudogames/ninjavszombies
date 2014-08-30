#include "app.h"
#include "data/ninja.h"
	
void ninja_init(App *app) {

	sprite_init(&app->sprite_ninja,
		0, 0, // origin
		16, 16, // frame size
		ACTION_COUNT, 4, // action, frame counts
		ninja_png, ninja_png_len // source
	);

	app->ninja.pos.x = 0;
	app->ninja.pos.y = map_y(app, app->ninja.pos.x);
	app->ninja.sprite = &app->sprite_ninja;

}


void ninja_move(App *app) {

	app->ninja.action = app->pressed[SDLK_SPACE] ? ACTION_ATTACK1 : ACTION_MOVE;

	if(app->pressed[SDLK_LEFT])  app->ninja.pos.x -= ZOOM/2, app->ninja.dir=DIR_LEFT;
	if(app->pressed[SDLK_RIGHT]) app->ninja.pos.x += ZOOM/2, app->ninja.dir=DIR_RIGHT;
	if(app->pressed[SDLK_LEFT] || app->pressed[SDLK_RIGHT]) 
		app->ninja.pos.y = map_y(app, app->ninja.pos.x);

}

void ninja_render(App *app) {

	body_render(app, &app->ninja);

}
