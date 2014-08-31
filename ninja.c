#include "app.h"
#include "data/ninja.h"
#include "data/karate.h"

#define HUD_SIZE (TILE_SIZE * 6)

void ninja_init(App *app) {

	sprite_init(&app->sprite_ninja,
		0, 0, // origin
		80, 80, // frame size
		ACTION_COUNT, 6, // action, frame counts
		ninja_png, ninja_png_len, // source
		TILE_SIZE
	);

	app->ninja.sprite = &app->sprite_ninja;

    TTF_Init();
    app->font = TTF_OpenFontRW(SDL_RWFromConstMem(karate_ttf,karate_ttf_len), 0, HUD_SIZE*1.5);


}

void ninja_spawn(App *app) {
	app->ninja.pos.x = app->map_x + app->screen->w/2;
	app->ninja.pos.y = map_y(app, app->ninja.pos.x);
	app->ninja.dir = DIR_RIGHT;
	app->ninja.health = 100;
	app->ninja.action = ACTION_IDLE;
	app->ninja.frame = 0;
	app->ninja.close_range = -1;
	app->start_x = app->map_x;
	app->kills = 0;
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

	if(app->ninja.action == ACTION_JUMP) {
		if(app->ninja.frame > app->ninja.sprite->frame_count) {
			app->ninja.action = ACTION_IDLE;
		} else {
			if(app->ninja.pos.x - app->map_x > app->sprite_ninja.target_frame_size.x/2) {
				app->ninja.pos.x += (app->ninja.dir == DIR_RIGHT ? +1 : -1) * 3 * TILE_SIZE;
				app->ninja.pos.y = map_y(app, app->ninja.pos.x);
			}

			return;
		}
	}

	if(app->pressed[SDLK_UP]) {
		app->ninja.action = ACTION_JUMP;
		app->ninja.frame = 0;
		return;
	}

	if(app->pressed[SDLK_SPACE]) {
		app->ninja.action = app->ninja.close_range != -1
			? ACTION_ATTACK2 : ACTION_ATTACK1;
		app->ninja.frame = 0;
	}


	if(app->ninja.action == ACTION_ATTACK1) {
		if((int)app->ninja.frame == 2) {
			missile_spawn(app);
		} else if(app->ninja.frame > app->ninja.sprite->frame_count) {
			app->ninja.action = ACTION_IDLE;
		} else {
			return;
		}
	} else if(app->ninja.action == ACTION_ATTACK2) {
		if((int)app->ninja.frame == 1
		|| (int)app->ninja.frame == 3
		|| (int)app->ninja.frame == 4) {
			if(app->ninja.close_range != -1) {
				int i = app->ninja.close_range;
				app->zombie[i].health -= app->ninja.frame;
				blood_spawn(app, app->zombie[i].pos);
			}
		} else if(app->ninja.frame > app->ninja.sprite->frame_count) {
			app->ninja.action = ACTION_IDLE;
		}
	} else {
		if(app->pressed[SDLK_LEFT] || app->pressed[SDLK_RIGHT])
			app->ninja.action = ACTION_MOVE;
		else 
			app->ninja.action = ACTION_IDLE;
	}

	if(app->pressed[SDLK_LEFT]
	&& app->ninja.pos.x - app->map_x > app->sprite_ninja.target_frame_size.x/2) {
		app->ninja.pos.x -= TILE_SIZE;
		app->ninja.dir=DIR_LEFT;
	}

	if(app->pressed[SDLK_RIGHT]) {
		app->ninja.pos.x += TILE_SIZE;
		app->ninja.dir=DIR_RIGHT;
	}

	app->ninja.pos.y = map_y(app, app->ninja.pos.x);


}

void ninja_render(App *app) {
	Uint32 color = SDL_MapRGB(app->screen->format, 0xff, 0x00, 0x00 );
	SDL_Rect rect = {
		HUD_SIZE,
		HUD_SIZE,
		MAX(0,app->ninja.health),
		HUD_SIZE
	};
	SDL_FillRect(app->screen, &rect, color);

	// score: timer, distance, ...
	char text[256];
	snprintf(text, sizeof(text)-1, "%d Ninja vs. Zombies %d",
		app->end_x - app->start_x, app->kills);
	SDL_Color red = { 0xff, 0x00, 0x00, 0xff };
	SDL_Surface *hud = TTF_RenderText_Blended(app->font, text, red);
	rect.x = app->screen->w - hud->w - HUD_SIZE;
    SDL_BlitSurface(hud, NULL, app->screen, &rect);
    SDL_FreeSurface(hud);


	body_render(app, &app->ninja);
}
