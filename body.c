#include "app.h"

void body_render(App *app, Body *body) {
	body->frame += 0.3;

	SDL_Rect src;
	sprite_target_rect(
			body->sprite, 
			body->action, 
			(int)body->frame,
			body->dir,
			&src);

	SDL_Rect dest = {
		body->pos.x - body->sprite->target_frame_size.x/2 - app->map_x,
		body->pos.y - body->sprite->target_frame_size.y/2 - app->map_y,
		0, 0
	};

	SDL_BlitSurface(body->sprite->target, &src, app->screen, &dest);
}

