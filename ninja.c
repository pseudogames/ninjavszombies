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
	app->ninja.pos.x = app->map_x + app->screen->w/2;
	app->ninja.pos.y = map_y(app, app->ninja.pos.x);
	app->ninja.dir = DIR_RIGHT;
	app->ninja.health = 100;
	app->ninja.action = ACTION_IDLE;
	app->ninja.frame = 0;
}

void ninja_move(App *app) {
	if(app->ninja.action == ACTION_DEATH) {
		if(app->ninja.frame > app->ninja.sprite->frame_count) {
			ninja_spawn(app);
		}
		return;
	} else {
		if(app->ninja.health <= 0) {
			app->ninja.action = ACTION_DEATH;
			app->ninja.frame = 0;
			return;
		}
	}

	app->ninja.action = ACTION_IDLE;
	if(app->pressed[SDLK_LEFT] || app->pressed[SDLK_RIGHT])
		app->ninja.action = ACTION_MOVE;
	if(app->pressed[SDLK_SPACE])
		app->ninja.action = ACTION_ATTACK1;

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
	Uint32 color = SDL_MapRGB(app->screen->format, 0xff, 0x00, 0x00 );
	SDL_Rect rect = {
		TILE_SIZE,
		TILE_SIZE,
		MAX(0,app->ninja.health),
		TILE_SIZE * 4
	};
	SDL_FillRect(app->screen, &rect, color);

	body_render(app, &app->ninja);
}
