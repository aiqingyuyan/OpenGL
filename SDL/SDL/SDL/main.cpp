//
//  main.cpp
//  SDL
//
//  Created by Yan Yu on 2/17/15.
//  Copyright (c) 2015 Yan Yu. All rights reserved.
//

#include <cstdio>
#include <SDL2/SDL.h>

int main(int argc, const char * argv[]) {
    
    // Initialize SDL2 subsysten
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    
    if (result < 0) {
        // Init failed
        printf("Could not initialize SDL subsysten: %s\n", SDL_GetError());
        
        //return
        return 1;
    }
    
    // Create an application window with following settings:
    SDL_Window* window = SDL_CreateWindow(
                     "An SDL2 Window",          // title
                     SDL_WINDOWPOS_UNDEFINED,   // initial x pos
                     SDL_WINDOWPOS_UNDEFINED,   // initial y pos
                     1280,                      // width in pixel
                     768,                       // height in pixel
                     SDL_WINDOW_OPENGL          // flags
    );
    SDL_Surface* surface = NULL;
    
    // Check if windows is successfuly created
    if (window == NULL) {
        // print error msg
        printf("Could not create window: %s\n", SDL_GetError());
        
        // return
        return 1;
    }
    
    surface = SDL_GetWindowSurface(window);
    
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xCC, 0x20, 0x20));
    SDL_UpdateWindowSurface(window);
    
    // The Window is open: enter program loop (see SDL_PollEvent)
    SDL_Delay(5000);    // pause execution for 3000 milliseconds
    
    // Close and Destroy the window
    SDL_DestroyWindow(window);
    
    //Clean up
    SDL_Quit();
    
    return 0;
}
