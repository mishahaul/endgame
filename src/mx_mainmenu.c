#include "DEC2020.h"

#define MAINMENU_QUITBUTTON_X 705
#define MAINMENU_QUITBUTTON_Y 679
#define MAINMENU_QUITBUTTON_WIDTH 190
#define MAINMENU_QUITBUTTON_HEIGHT 35
#define MAINMENU_LEADER_X 540
#define MAINMENU_LEADER_Y 583
#define MAINMENU_LEADER_WIDTH 525
#define MAINMENU_LEADER_HEIGHT 40
#define MAINMENU_STARTBUTTON_X 285
#define MAINMENU_STARTBUTTON_Y 323
#define MAINMENU_STARTBUTTON_WIDTH 1030
#define MAINMENU_STARTBUTTON_HEIGHT 141

//#define START_BUTTON_PNG "resource/blue.png"
//#define QUIT_BUTTON_PNG "resource/red.png"
//#define START_BACKGROUND_PNG "resource/Textures/Background/Start.png"

int mx_mainmenu(SDL_Renderer *renderer, t_gamestate *game) {

    uint clicked = 0;
    int running = 1;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (SDL_QUIT == event.type)
            //|| (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
            running = 0;
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT
                && event.button.x >= MAINMENU_STARTBUTTON_X
                && event.button.x <= MAINMENU_STARTBUTTON_X + MAINMENU_STARTBUTTON_WIDTH
                && event.button.y >= MAINMENU_STARTBUTTON_Y
                && event.button.y <= MAINMENU_STARTBUTTON_Y + MAINMENU_STARTBUTTON_HEIGHT)
                clicked = 1;
            if (event.button.button == SDL_BUTTON_LEFT
                && event.button.x >= MAINMENU_LEADER_X
                && event.button.x <= MAINMENU_LEADER_X + MAINMENU_LEADER_WIDTH
                && event.button.y >= MAINMENU_LEADER_Y
                && event.button.y <= MAINMENU_LEADER_Y + MAINMENU_LEADER_HEIGHT)
                clicked = 2;
            if (event.button.button == SDL_BUTTON_LEFT
                && event.button.x >= MAINMENU_QUITBUTTON_X
                && event.button.x <= MAINMENU_QUITBUTTON_X + MAINMENU_QUITBUTTON_WIDTH
                && event.button.y >= MAINMENU_QUITBUTTON_Y
                && event.button.y <= MAINMENU_QUITBUTTON_Y + MAINMENU_QUITBUTTON_HEIGHT)
                clicked = 3;

            //mouse click sound
            Mix_Chunk *mainmenu = Mix_LoadWAV("res/Sounds/mainmenu.wav");
            Mix_PlayChannel(-1, mainmenu, 0);
        }
    }
    SDL_Rect background_rect = {0, 0, W_WIDTH, W_HEIGHT};
    SDL_Texture *tex;
    switch (clicked) {
        case 1:
            running = 2;
            tex = mx_get_texture(game, TEX_BACKGROUND_START_CLICKED);
            game->last_time = SDL_GetTicks();
            mx_set_battle_scene(game);
            break;
        case 2:
            running = 2;
            tex = mx_get_texture(game, TEX_BACKGROUND_START_LEADERBOARD_CLICKED);
            game->scene = LEADERBOARD_STATE;
            break;
        case 3:
            running = 3;
            //write(1,"quit", 4);
            tex = mx_get_texture(game, TEX_BACKGROUND_START_QUIT_CLICKED);
            break;
        default:
            tex = mx_get_texture(game, TEX_BACKGROUND_START);
            break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &background_rect);

    return running;
}
