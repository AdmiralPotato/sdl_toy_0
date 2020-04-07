#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <tmx.h>

#include "map.h"

tmx_map *map = NULL;
static SDL_Renderer *ren = NULL;
SDL_Texture *mapTexture;
SDL_Texture *map_bmp;
SDL_Rect mapRect;
SDL_Rect mapSrcRect;
SDL_Rect mapDestRect;

void fatal_error (char* message) {
    printf("FATAL ERROR: %s\n", message);
    quit(1);
}


void set_color(int color) {
    unsigned char r, g, b;

    r = (color >> 16) & 0xFF;
    g = (color >>  8) & 0xFF;
    b = (color)       & 0xFF;

    SDL_SetRenderDrawColor(ren, r, g, b, SDL_ALPHA_OPAQUE);
}

void* sdl_img_loader(const char *path) {
    return IMG_LoadTexture(ren, path);
}

void draw_polyline(double **points, double x, double y, int pointsc) {
    int i;
    for (i=1; i<pointsc; i++) {
        SDL_RenderDrawLine(ren, x+points[i-1][0], y+points[i-1][1], x+points[i][0], y+points[i][1]);
    }
}

void draw_polygon(double **points, double x, double y, int pointsc) {
    draw_polyline(points, x, y, pointsc);
    if (pointsc > 2) {
        SDL_RenderDrawLine(ren, x+points[0][0], y+points[0][1], x+points[pointsc-1][0], y+points[pointsc-1][1]);
    }
}

void draw_objects(tmx_object_group *objgr) {
    SDL_Rect rect;
    set_color(objgr->color);
    tmx_object *head = objgr->head;
    /* FIXME line thickness */
    while (head) {
        if (head->visible) {
            if (head->obj_type == OT_SQUARE) {
                rect.x =     head->x;  rect.y =      head->y;
                rect.w = head->width;  rect.h = head->height;
                SDL_RenderDrawRect(ren, &rect);
            } else if (head->obj_type  == OT_POLYGON) {
                draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
            } else if (head->obj_type == OT_POLYLINE) {
                draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
            } else if (head->obj_type == OT_ELLIPSE) {
                /* FIXME: no function in SDL2 */
            }
        }
        head = head->next;
    }
}

unsigned int gid_clear_flags(unsigned int gid) {
    return gid & TMX_FLIP_BITS_REMOVAL;
}

void draw_layer(tmx_map *map, tmx_layer *layer) {
    unsigned long i, j;
    unsigned int gid;
    float op;
    tmx_tileset *ts;
    tmx_image *im;
    SDL_Rect srcrect, dstrect;
    SDL_Texture* tileset;
    op = layer->opacity;
    for (i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            gid = gid_clear_flags(layer->content.gids[(i*map->width)+j]);
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                srcrect.x = map->tiles[gid]->ul_x;
                srcrect.y = map->tiles[gid]->ul_y;
                srcrect.w = dstrect.w = ts->tile_width;
                srcrect.h = dstrect.h = ts->tile_height;
                dstrect.x = j*ts->tile_width;  dstrect.y = i*ts->tile_height;
                /* TODO Opacity and Flips */
                if (im) {
                    tileset = (SDL_Texture*)im->resource_image;
                }
                else {
                    tileset = (SDL_Texture*)ts->image->resource_image;
                }
                SDL_RenderCopy(ren, tileset, &srcrect, &dstrect);
            }
        }
    }
}

void draw_image_layer(tmx_image *img) {
    SDL_Rect dim;

    dim.x = dim.y = 0;
    SDL_QueryTexture((SDL_Texture*)img->resource_image, NULL, NULL, &(dim.w), &(dim.h));

    SDL_RenderCopy(ren, (SDL_Texture*)img->resource_image, NULL, &dim);
}

void draw_all_layers(tmx_map *map, tmx_layer *layers) {
    while (layers) {
        if (layers->visible) {
            if (layers->type == L_GROUP) {
                draw_all_layers(map, layers->content.group_head);
            } else if (layers->type == L_OBJGR) {
                draw_objects(layers->content.objgr);
            } else if (layers->type == L_IMAGE) {
                draw_image_layer(layers->content.image);
            } else if (layers->type == L_LAYER) {
                draw_layer(map, layers);
            }
        }
        layers = layers->next;
    }
}

SDL_Texture* render_map(tmx_map *map) {
    SDL_Texture *res;
    int w, h;

    w = map->width  * map->tile_width;  /* Bitmap's width and height */
    h = map->height * map->tile_height;

    if (!(res = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h)))
        fatal_error(SDL_GetError());
    SDL_SetRenderTarget(ren, res);

    set_color(map->backgroundcolor);
    SDL_RenderClear(ren);

    draw_all_layers(map, map->ly_head);

    SDL_SetRenderTarget(ren, NULL);
    return res;
}

// Almost all of the above came from: `tmx/src/examples/sdl/sdl.c`

unsigned int getTileIndexByCoordinate (uint16_t x, uint16_t y, tmx_map *map) {
    return (
        ((y / map->tile_height) * map->width)
        + (x / map->tile_width)
    );
}

unsigned int lastTileIndex = -1; // not likely to be matched, intentional underflow
tmx_tile* lastTile = NULL;
tmx_tile* getTileByCoordinate (uint16_t x, uint16_t y) {
    unsigned int tileIndex = getTileIndexByCoordinate(x, y, map);
    unsigned int gid;
    tmx_tile *tile = NULL;
    tmx_layer *layer = map->ly_head;
	// if tileIndex is same as last call, no need to look it up again
    if (tileIndex != lastTileIndex) {
        while (layer) {
            if (layer->visible) {
                if (layer->type == L_LAYER) { // only care about tile-based layers
                    gid = gid_clear_flags(layer->content.gids[tileIndex]);
                    if(map->tiles[gid]) {
                        tile = map->tiles[gid];
                    }
                }
            }
            layer = layer->next;
        }
        lastTileIndex = tileIndex;
        lastTile = tile;
    } else {
        tile = lastTile;
    }
    return tile;
}

void initMap (SDL_Renderer *renderer) {

    ren = renderer;

    tmx_img_load_func = (void* (*)(const char*))sdl_img_loader;
    tmx_img_free_func = (void  (*)(void*))      SDL_DestroyTexture;

    if (!(map = tmx_load(MAP_FILE))) {
        fatal_error(tmx_strerr());
    }
    printf("Map Loaded: %s\n", MAP_FILE);

    mapRect.w = map->width * map->tile_width;
    mapRect.h = map->height * map->tile_height;
    mapRect.x = 0; mapRect.y = 0;

    if (!(map_bmp = render_map(map))){
        fatal_error(SDL_GetError());
    }

    mapSrcRect.x = gameRect.x;
    mapSrcRect.y = gameRect.y;
    mapSrcRect.w = gameRect.w;
    mapSrcRect.h = gameRect.h;
    mapDestRect.x = gameRect.x;
    mapDestRect.y = gameRect.y;
    mapDestRect.w = gameRect.w;
    mapDestRect.h = gameRect.h;

}

void unloadMap () {
    tmx_map_free(map);
    SDL_DestroyTexture(map_bmp);
    map_bmp = NULL;
}

void updateMap () {
    mapSrcRect.x = (int) fmax(playerX - gameRectHalf.w, 0);
    mapSrcRect.y = (int) fmax(playerY - gameRectHalf.h, 0);
    mapSrcRect.w = (int) fmin(mapRect.w - playerX + gameRectHalf.w, gameRect.w);
    mapSrcRect.h = (int) fmin(mapRect.h - playerY + gameRectHalf.h, gameRect.h);

    mapDestRect.x = (int) fmax(gameRectHalf.w - playerX, 0);
    mapDestRect.y = (int) fmax(gameRectHalf.h - playerY, 0);
    mapDestRect.w = mapSrcRect.w;
    mapDestRect.h = mapSrcRect.h;
}

void drawMap (SDL_Renderer *renderer) {
    SDL_RenderCopy(
        renderer,
        map_bmp,
        &mapSrcRect,
        &mapDestRect
    );
}
