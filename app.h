#include <SDL.h>
#include <SDL_image.h>

typedef enum {
	DIR_LEFT=0,
	DIR_RIGHT=1
} Direction;

typedef enum {
	STATE_MENU,
	STATE_PAUSED,
	STATE_PLAYING,
	STATE_GAMEOVER,
	STATE_HELP,
	STATE_CREDITS,
	STATE_EXIT,
} AppState;

typedef enum { 
    ACTION_IDLE=0, 
    ACTION_MOVE, 
    ACTION_JUMP, 
    ACTION_ATTACK1, 
    ACTION_ATTACK2, 
    ACTION_DEATH, 
    ACTION_COUNT
} Action;

typedef struct { int x,y; } Point;

typedef struct {
    Point origin;
    Point frame_size;
    int frame_count;
    int action_count;
    SDL_Surface *source;
    Point target_frame_size;
    SDL_Surface *target;
} Sprite;

typedef struct {
	int active;
	Point pos;
	Sprite *sprite;
	Action action;
	float frame;
	Direction dir;
	int health;
	Point speed;
} Body;

#define WIDTH 1024
#define HEIGHT 768
#define TILE_SIZE 3
#define MAP_SIZE (WIDTH/TILE_SIZE)
#define MAX_ZOMBIES 666
#define MAX_MISSILES 66
typedef struct {
	SDL_Surface *screen;
	AppState state;
	Sprite sprite_ninja;
	Sprite sprite_zombie;
	Sprite sprite_missile1;
	Sprite sprite_missile2;
	Body ninja;
	Body zombie[MAX_ZOMBIES];
	Body missile[MAX_MISSILES];
	int pressed[SDLK_LAST];
	int zombie_spawn_time;
	float zombie_spawn_delay;
	int map_x;
	int map_y;
	int heightmap_x;
	int heightmap[MAP_SIZE];
	int map_step_x;
	int map_step_delay;
	int missile_index;
	SDL_Surface *blood;
	SDL_Surface *blood_off;
} App;

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define CLAMP(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))

#define RGBA_FORMAT 32,0x00ff0000,0x0000ff00,0x000000ff,0xff000000


void sprite_source_rect(Sprite *sprite, Action action, int frame, SDL_Rect *rect);
void sprite_target_rect(Sprite *sprite, Action action, int frame, Direction dir, SDL_Rect *rect);
void sprite_gen_rotation(Sprite *sprite);
void sprite_init(Sprite *sprite, int ox, int oy, int fx, int fy, int actions, int frames, const void *mem, int len);

void body_move(App *app, Body *body);
void body_render(App *app, Body *body);

void map_init(App *app); 
void map_move(App *app); 
inline int map_y(App *app, int x);
inline int map_y0(App *app, int x);
inline int map_y1(App *app, int x);
void map_render(App *app);

void blood_init(App *app); 
void blood_spawn(App *app, Point pos); 
void blood_move(App *app); 
void blood_render(App *app);

void ninja_init(App *app); // one time setup (e.g., load sprite)
void ninja_spawn(App *app); // (re)spawn init pos, anim
void ninja_move(App *app); // keyaboard input and inertia (e.g, jump), check if died, change action, attack
void ninja_render(App *app); // draw on screen

void zombie_init(App *app); // one time setup (e.g., load sprite)
void zombie_spawn(App *app); // spawn one zombie
void zombie_move(App *app); // zombies movement decision
void zombie_render(App *app); // draw zombies

void missile_init(App *app); // one time setup (e.g., load sprite)
void missile_spawn(App *app); // spawn one missile from ninja
void missile_move(App *app); // move missile, check collision
void missile_render(App *app); // render missile

