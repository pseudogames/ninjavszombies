#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include "app.h"

#define ZOOM 16
#define RGBA_FORMAT 32,0x00ff0000,0x0000ff00,0x000000ff,0xff000000

void sprite_source_rect(Sprite *sprite, Action action, int frame, SDL_Rect *rect)
{
    frame = frame % sprite->frame_count;
    rect->x = sprite->origin.x + frame *sprite->frame_size.x;
    rect->y = sprite->origin.y + action*sprite->frame_size.y;
    rect->w = sprite->frame_size.x;
    rect->h = sprite->frame_size.y;
}

void sprite_target_rect(Sprite *sprite, Action action, int frame, Direction dir, SDL_Rect *rect)
{
    frame = frame % sprite->frame_count;
    rect->x = frame *sprite->target_frame_size.x;
    rect->y = action*sprite->target_frame_size.y+
        + sprite->target_frame_size.y*sprite->action_count*dir;
    rect->w = sprite->target_frame_size.x;
    rect->h = sprite->target_frame_size.y;
}

void sprite_gen_target(Sprite *sprite)
{
    zoomSurfaceSize(
            sprite->frame_size.x,
            sprite->frame_size.y,
            ZOOM,  // no zoom
            ZOOM,  // no zoom
            &sprite->target_frame_size.x,
            &sprite->target_frame_size.y
            );

    if(sprite->target)
        SDL_FreeSurface(sprite->target);

    sprite->target = SDL_CreateRGBSurface(SDL_HWSURFACE, 
            sprite->target_frame_size.x * sprite->frame_count,
            sprite->target_frame_size.y * sprite->action_count * 2,
            RGBA_FORMAT);
    if(sprite->target == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    //printf("cache size %dx%d for %d angles\n", sprite->target->w, sprite->target->h, 360/ANGLE_STEP);

    SDL_Surface *element = SDL_CreateRGBSurface(SDL_HWSURFACE, 
            sprite->frame_size.x, 
            sprite->frame_size.y,
            RGBA_FORMAT);

    SDL_SetAlpha(sprite->source,0,0xff);
    SDL_SetAlpha(element,0,0xff);
    SDL_SetAlpha(sprite->target,SDL_SRCALPHA,0xff);

    int frame, action, dir;
    for(action=0; action<sprite->action_count; action++) {
        for(frame=0; frame<sprite->frame_count; frame++) {
            SDL_Rect src;
            sprite_source_rect(sprite, action, frame, &src);
            for(dir=0; dir<=1; dir++) {
                SDL_Rect dst;
                sprite_target_rect(sprite, action, frame, dir, &dst);
                SDL_FillRect(element, NULL, 0x00000000);
                SDL_BlitSurface( sprite->source, &src, element, NULL );
                SDL_Surface *zoom = zoomSurface(element, (dir ? 1 : -1) * ZOOM, ZOOM, SMOOTHING_OFF);
                SDL_SetAlpha(zoom,0,0);
                SDL_SetColorKey(zoom,0,0);
                dst.x += dst.w/2 - zoom->w/2;
                dst.y += dst.h/2 - zoom->h/2; // center
                SDL_BlitSurface(zoom, NULL, sprite->target, &dst );
                SDL_FreeSurface(zoom);
            }
        }
    }

    SDL_FreeSurface(element);
}

void sprite_init(Sprite *sprite, int ox, int oy, int fx, int fy, int actions, int frames, const void *mem, int len)
{
    memset(sprite,0,sizeof(Sprite));
    sprite->origin.x = ox;
    sprite->origin.y = oy;
    sprite->frame_size.x = fx;
    sprite->frame_size.y = fy;
    sprite->frame_count = frames;
    sprite->action_count = actions;
    sprite->source = IMG_Load_RW(SDL_RWFromConstMem( mem, len), 0);
    sprite->target = NULL;
	sprite_gen_target(sprite);
}
