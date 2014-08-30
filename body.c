#include "app.h"

void body_render(App *app, Body *body) {
	SDL_Rect src;
	sprite_target_rect(
			body->sprite, 
			body->action, 
			(int)body->frame,
			body->dir,
			&src);

	SDL_Rect dest = {
		body->pos.x - body->sprite->target_frame_size.x/2,
		body->pos.y - body->sprite->target_frame_size.y/2,
		0, 0
	};

	body->frame += 0.1;

	SDL_BlitSurface(body->sprite->target, &src, app->screen, &dest);
}

void body_move(App *app, Body *body) {

}
