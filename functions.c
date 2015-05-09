#include "functions.h"

void brush() {
    swap_screen();
    while(quit_prg==0) {
        test_events();
        test_end_screen();
        restore_screen();
        draw();
        display_brush();
        //SDL_framerateDelay(&manager);
   }
}

void swap_screen() {
    double posY;
    SDL_Rect src;
    SDL_Rect dest;
    src.x=0;
    src.y=0;
    src.w=1024;
    dest.x=0;
    dest.w=1024;
    double x=0.0;
    int loop=1;
    while(loop&&!quit_prg) {
        posY=(double)768-(x*x);
        if(posY<0) {
            posY=0.0;
            loop=0;
        }
        x+=1.0;
        dest.y=(int)posY;
        src.h=768-(int)posY;
        dest.h=768-(int)posY;
        SDL_BlitSurface(p_background, &src, p_screen, &dest);
        SDL_framerateDelay(&manager);
        SDL_UpdateWindowSurface(p_window);
    }
}

void test_end_screen() {
    if(end_screen) {
        end_screen=0;
        if(index_draw>0&&mem[index_draw-1]!=-1) {
        mem[index_draw]=-1;
        index_draw++;
        swap_screen();
        }
        /*SDL_FillRect(p_save, NULL, SDL_MapRGB(p_save->format, 0, 0, 0));
        SDL_FillRect(p_screen, NULL, SDL_MapRGB(p_screen->format, 0, 0, 0));
        SDL_UpdateWindowSurface(p_window);*/
    }
}

void draw() {
    if(drawing) {
        if(!(last_x==x&&last_y==y)) {
        mem[index_draw]=x;
        mem[index_draw+1]=y;
        last_x=x; last_y=y;
        index_draw+=2;
        printf("x=%d y=%d\n", x, y);
        }
        double xPos=(double)prevX;
        double yPos=(double)prevY;
        double stepX=((double)x-xPos)/(double)100;
        double stepY=((double)y-yPos)/(double)100;
        int i=0;
        SDL_Rect rect;
        rect.w=16;
        rect.h=16;
        while(i<100) {
            rect.x=(int)xPos;
            rect.y=(int)yPos;
            SDL_BlitSurface(p_brush, NULL, p_screen, &rect);
            xPos+=stepX;
            yPos+=stepY;
            i++;
        }
        SDL_UpdateWindowSurface(p_window);
        }
}

void restore_screen() {
    SDL_Rect dest = { prevX, prevY, 10, 10};
    SDL_BlitSurface(p_save, NULL, p_screen, &dest);
}

void display_brush() {
   SDL_FillRect(p_save, NULL, SDL_MapRGB(p_save->format, 0, 0, 0));
    SDL_Rect src={ x, y, 10, 10};
    SDL_BlitSurface(p_screen, &src, p_save, NULL);
     SDL_Rect rect={ x, y, 10, 10};
    SDL_BlitSurface(p_brush, NULL, p_screen, &rect);
    SDL_UpdateWindowSurface(p_window);
}

void test_events() {
    prevX=x;
    prevY=y;
    SDL_PollEvent(&event);
    //
    switch(event.type) {
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                quit_prg=1;
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            x=event.motion.x;
            y=event.motion.y;
        break;
        case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button) {
            case SDL_BUTTON_LEFT:
            if(drawing==0) {
                 drawing=1;
                 if(tick!=0) {
                tick=SDL_GetTicks()-tick;
                mem[index_draw++]=-3;
                mem[index_draw]=(int)tick;
                index_draw++;
                } else {
                    tick=SDL_GetTicks();
                }
                }
                 break;
            case SDL_BUTTON_RIGHT:
                drawing=0;
                end_screen=1;
                break;
        }

            break;
        case SDL_MOUSEBUTTONUP:
        switch (event.button.button) {
            case SDL_BUTTON_LEFT:
               if(drawing==1) {
                tick=SDL_GetTicks();
                }
                 drawing=0;
                doublon=0;
                break;
        }
            break;
    }
    if(x>1023-10) {
        x=1023-10;
    }

    if(y>767-10) {
        y=767-10;
    }
}

void init() {
    mem=(int*)malloc(MEMSIZE*sizeof(int));
    index_draw=0;
    quit_prg=0;
    x=event.motion.x;
    y=event.motion.y;
    drawing=0;
    end_screen=1;
    doublon=0;
     last_x=0;
    last_y=0;
   //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    p_window=SDL_CreateWindow("HMD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_BORDERLESS);
    p_screen=SDL_GetWindowSurface(p_window);
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    p_save=SDL_CreateRGBSurface(0, 10, 10, 24, rmask, gmask, bmask, amask);
    SDL_SetSurfaceBlendMode(p_save, SDL_BLENDMODE_NONE);
    //SDL_SetColorKey(p_save, SDL_TRUE, SDL_MapRGB(p_save->format, 0, 0, 0));
    SDL_FillRect(p_save, NULL, SDL_MapRGB(p_save->format, 0, 0, 0));
    int flags=IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    p_brush=IMG_Load("gfx/brush.png");
    p_background=IMG_Load("gfx/walter.png");
    SDL_SetColorKey(p_brush, SDL_TRUE, SDL_MapRGB(p_brush->format, 0, 0, 0));
    SDL_ShowCursor(0);
    //
    SDL_initFramerate(&manager);
    SDL_setFramerate(&manager, 60);
}

void save_coords() {
    if(index_draw>MEMSIZE) {
        printf("Allocation trop faible : index=%d\n", index_draw);
    } else {
        fichier=fopen("coords", "w");
        fprintf(fichier, "int size=%d;\n", index_draw);
        fprintf(fichier, "{ ");
        int i, j=0;
        for(i=0; i<index_draw; i++) {
            fprintf(fichier, "%d, ", mem[i]);
            j++;
            if(j>=30) {
                j=0;
                fprintf(fichier, "\n");
            }
        }
        fclose(fichier);
        printf("nombre d'éléments=%d\n", index_draw);
    }
}

void quit() {
    index_draw++;
    SDL_FreeSurface(p_brush);
    IMG_Quit();
    SDL_DestroyWindow(p_window);
    SDL_Quit();
    save_coords();
    free(mem);
}
