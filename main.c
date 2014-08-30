#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include "app.h"
#include "data/icon.h"

#define FPS 30

void handleDelay(Uint32 start) {
	Uint32 end = SDL_GetTicks();
	int actual_delta = end - start;
	int expected_delta = 1000/FPS;
	int delay = MAX(0, expected_delta - actual_delta);
	//printf("delay %d %d %d\n", actual_delta, expected_delta, delay);
	SDL_Delay(delay);
}


int main(int argc, char* args[]) {
	App app;
	srand(time(NULL));
	memset(&app,0,sizeof(App));

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

	app.screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE |SDL_DOUBLEBUF
#if RELEASE
			|SDL_FULLSCREEN
#endif
			);

	SDL_WM_SetIcon(IMG_Load_RW(SDL_RWFromConstMem(icon_png, icon_png_len), 0), NULL);
	SDL_WM_SetCaption("Ninja vs Zombies", "Ninja vs Zombies");

	map_init(&app);
	ninja_init(&app);
	missile_init(&app);
	zombie_init(&app);

	while(app.state != STATE_EXIT) {
		Uint32 startTime = SDL_GetTicks();
		
		// input
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					app.pressed[event.key.keysym.sym] = event.type == SDL_KEYDOWN;
			}
			switch(event.type) {
				case SDL_QUIT:
					app.state = STATE_EXIT;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_SPACE:
							missile_init(&app);
							break;
						case SDLK_q:
							app.state = STATE_EXIT;
							break;
					}
					break;
			}
		}

		zombie_spawn(&app);

		ninja_move(&app);
		map_move(&app);
		missile_move(&app);
		zombie_move(&app);

		// render
		Uint32 color = SDL_MapRGB(app.screen->format, 0xff, 0xff, 0xff );
		SDL_FillRect(app.screen, NULL , color);

		map_render(&app);
		missile_render(&app);
		ninja_render(&app);
		zombie_render(&app);

		SDL_Flip(app.screen);
		handleDelay(startTime);
	}
}
