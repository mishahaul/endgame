#include "DEC2020.h"

int mx_playgame(SDL_Renderer *renderer, t_gamestate *game) {
    SDL_Surface* cursor_image;
    SDL_Cursor* cursor;

    int running = 1;
    while (running) {
        if (running == 1) {
            SDL_RenderClear(renderer);
            switch (game->scene) {
                case INTRO_STATE:
                    break;
                case START_STATE:
                    cursor_image = SDL_LoadBMP("resource/Textures/cursor_bullet.bmp");
                    cursor = SDL_CreateColorCursor(cursor_image, 7, 7);
                    SDL_SetCursor(cursor);
                    running = mx_mainmenu(renderer, game);
                    break;
                case GAME_STATE:
                    cursor_image = SDL_LoadBMP("resource/Textures/cursor_crosshair.bmp");
                    cursor = SDL_CreateColorCursor(cursor_image, 25, 25);
                    SDL_SetCursor(cursor);
                    mx_battle(renderer, game);
                    break;
                case GAMEOVER_STATE:
                    cursor_image = SDL_LoadBMP("resource/Textures/cursor_bullet.bmp");
                    cursor = SDL_CreateColorCursor(cursor_image, 7, 7);
                    SDL_SetCursor(cursor);
                    running = mx_gameover_menu(renderer, game);
                    break;
                case LEADERBOARD_STATE:
                    cursor_image = SDL_LoadBMP("resource/Textures/cursor_bullet.bmp");
                    cursor = SDL_CreateColorCursor(cursor_image, 7, 7);
                    SDL_SetCursor(cursor);
                    mx_leaderboard(renderer, game);
                    break;
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(1);
        }
        if (running == 2) {
            SDL_Delay(100);
            running = 1;
        }
        if (running == 3) {
            SDL_Delay(100);
            running = 0;
        }
        SDL_FreeCursor(cursor);
        SDL_FreeSurface(cursor_image);
    }
    return 0;
}
