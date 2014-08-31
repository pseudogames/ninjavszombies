#include <math.h>
#include "app.h"
#include "data/zombie.h"

void zombie_init(App *app) {

	sprite_init(&app->sprite_zombie,
		0, 0, // origin
		84, 84, // frame size
		ACTION_COUNT, 6, // action, frame counts
		zombie_png, zombie_png_len, // source
		TILE_SIZE
	);

	app->zombie_spawn_time = SDL_GetTicks();
	app->zombie_spawn_delay = 5000;
}

void zombie_spawn(App *app) {
	int i;
	int t = SDL_GetTicks();
	if(t > app->zombie_spawn_time + app->zombie_spawn_delay) {
		//printf("search free zombie slot\n");
		app->zombie_spawn_time = t;
		app->zombie_spawn_delay *= 0.99;

		for(i=0; i<MAX_ZOMBIES; i++) {
			if(app->zombie[i].active) continue;
			//printf("spawn zombie %d\n", i);
			app->zombie[i].sprite = &app->sprite_zombie;
			app->zombie[i].frame = rand() % 
				app->zombie[i].sprite->frame_count;
			app->zombie[i].pos.x = app->map_x + (rand() % 2
				? (- app->zombie[i].sprite->target_frame_size.x) 
				: (app->screen->w + app->zombie[i].sprite->target_frame_size.x)
			);
			app->zombie[i].pos.y = map_y(app, app->zombie[i].pos.x);
			app->zombie[i].health = 10;
			app->zombie[i].active = 1;
			break;
		}
	}

}

void zombie_move(App *app) {
	int i;

	app->ninja.close_range = -1;

	for(i=0; i<MAX_ZOMBIES; i++) {
		Body *z = &app->zombie[i];
		if(!z->active) continue;

		int dist = ABS(app->ninja.pos.x - app->zombie[i].pos.x);
		if(dist > app->screen->w) {
			z->active = 0;
			continue;
		}

		if(z->health < 0) {
			if(z->action != ACTION_DEATH) {
				z->action = ACTION_DEATH;
				z->frame = 0;
			} else {
				if((((int)ceil(z->frame)) % z->sprite->frame_count) == 0 ) {
					z->active = 0;
				}
			}
			break;
		}


		// damage potential
		float ideal_dist = (
			app->ninja.sprite->target_frame_size.x/2+
			z->sprite->target_frame_size.x/2
		)/2;

		float max_damage = 1 - CLAMP(ABS( dist - ideal_dist ) / ideal_dist, 0, 1);

		float damage = 0;

		if(z->action == ACTION_ATTACK1
		&&((int)z->frame == 2 || (int)z->frame == 4)) {
			damage = max_damage * 4;
		}
		if(z->action == ACTION_MOVE && (int)z->frame == 5) {
			damage = max_damage * 2;
		}
		if(z->action == ACTION_JUMP && (int)z->frame >0) {
			damage = max_damage * 1;
		}

		if(damage > 0) {
			app->ninja.health -= damage;
		}
				
		// move to ninja
		Direction dir = app->ninja.pos.x < z->pos.x ? DIR_LEFT : DIR_RIGHT;
		z->dir = dir;

		int close_range = ABS(app->ninja.pos.x - z->pos.x) < ideal_dist * 0.8;
		z->close_range = close_range;

		int ninja_facing = (app->ninja.dir == DIR_RIGHT ? 1 : -1) * (z->pos.x - app->ninja.pos.x);
		if(ninja_facing > -ideal_dist * 0.3 && ninja_facing < ideal_dist * 1.3) {
			app->ninja.close_range = i;
		}

		if(z->action == ACTION_MOVE) {
			// attack if too close
			if(max_damage > 0.8 || close_range) {
				if(close_range && (rand() % FPS) != 0) {
					z->action = ACTION_IDLE;
				} else {
					z->action = app->ninja.action == ACTION_JUMP
						? ACTION_JUMP : ACTION_ATTACK1;
				}
				z->frame = 0;
			} else {
				z->pos.x += dir == DIR_LEFT ? -1 : +1;
				z->pos.y = map_y(app, z->pos.x);
			}

		} else if((((int)ceil(z->frame)) % z->sprite->frame_count) == 0 ) {
			z->action = rand() % 2 ? ACTION_MOVE : ACTION_IDLE;
		}

	}


}

void zombie_render(App *app) {
	int i;

	for(i=0; i<MAX_ZOMBIES; i++) {
		Body *z = &app->zombie[i];
		if(!z->active) continue;
		body_render(app, z);
	}

}
