#include "app.h"
#include "data/zombie.h"

void zombie_init(App *app) {

	sprite_init(&app->sprite_zombie,
		0, 0, // origin
		16, 16, // frame size
		ACTION_COUNT, 4, // action, frame counts
		zombie_png, zombie_png_len // source
	);

}
