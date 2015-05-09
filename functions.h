#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_framerate.h>
#include <stdio.h>
#include <errno.h>

#define MEMSIZE 300000

SDL_Window *p_window;
SDL_Surface *p_brush;
SDL_Surface *p_background;
SDL_Surface *p_screen;
SDL_Surface *p_save;
//
SDL_Event event;
//
FPSmanager manager;

int x;
int y;
int prevX, prevY, last_x, last_y;
int quit_prg;
int drawing;
int end_screen;
int doublon;
//
int *mem;
int index_draw;
Uint32 tick;
//
FILE *fichier;

void init();
void quit();
void brush();
void test_events();
void display_brush();
void draw();
void restore_screen();
void test_end_screen();
void save_coords();
void swap_screen();

#endif // FUNCTIONS_H_INCLUDED
