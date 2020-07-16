#include "DEC2020.h"

#define GAMEOVERMENU_MENUBUTTON_X 705
#define GAMEOVERMENU_MENUBUTTON_Y 825
#define GAMEOVERMENU_MENUBUTTON_WIDTH 189
#define GAMEOVERMENU_MENUBUTTON_HEIGHT 32
#define GAMEOVERMENU_LEADER_X 540
#define GAMEOVERMENU_LEADER_Y 727
#define GAMEOVERMENU_LEADER_WIDTH 525
#define GAMEOVERMENU_LEADER_HEIGHT 33
#define GAMEOVERMENU_RESTARTBUTTON_X 540
#define GAMEOVERMENU_RESTARTBUTTON_Y 605
#define GAMEOVERMENU_RESTARTBUTTON_WIDTH 525
#define GAMEOVERMENU_RESTARTBUTTON_HEIGHT 57

#define RESTART_BUTTON_PNG "resource/blue.png"
#define QUIT_BUTTON_PNG "resource/red.png"

void mx_input_your_name(SDL_Renderer *renderer, t_gamestate *game){
    char text[16] = "\0";
    bool flag = true;
    SDL_StartTextInput();
    //SDL_Rect input_rect = {300,421,1000,60};
    //SDL_SetTextInputRect(&input_rect);

    bool done = false;
    TTF_Font * fnt = TTF_OpenFont("resource/RubikMonoOne-Regular.ttf", 32); // Загружаем шрифт по заданному адресу размером sz
    SDL_Color clr; // Тип для цвета. 4 числа — R, G, B и A, соответственно.
    clr.r = 255;
    clr.g = 255;  // Зададим параметры цвета
    clr.b = 255;
    clr.a = 255;
    while (!done) {

            SDL_Rect background_rect = {0, 0, W_WIDTH, W_HEIGHT};
            SDL_Texture *tex;
            tex = mx_get_texture(game, TEX_BACKGROUND_GAMEOVER_TYPE);
            SDL_RenderCopy(renderer, tex, NULL, &background_rect);
        if (strlen(text)){
            SDL_Surface *TextSurface = TTF_RenderText_Blended(fnt, text, clr); // Переносим на поверхность текст с заданным шрифтом и цветом
            SDL_Texture *Name = SDL_CreateTextureFromSurface(renderer, TextSurface); //now you can convert it into a texture
            SDL_Rect Message_rect; //create a rect
            Message_rect.x = (W_WIDTH - 48 * strlen(text))/2;  //controls the rect's x coordinate
            Message_rect.y = 677; // controls the rect's y coordinte
            Message_rect.w = 48 * strlen(text); // controls the width of the rect
            Message_rect.h = 48;
            SDL_RenderCopy(renderer, Name, NULL, &Message_rect);
            SDL_FreeSurface(TextSurface);
        }
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            int key = event.key.keysym.sym;

            if (key == SDLK_RETURN)
            {
                //t_player_stats = mx_read_leaderboeard_file();
                if (!strlen(text)){
                    strcpy(text, "empty");
                }
                file_write(board_updated(mx_read_leaderboeard_file(), text, game->kill_count));
                done = 1;
                game->kill_count = 0;
            }
            if (key == SDLK_BACKSPACE){
                if (flag) {
                    if (strlen(text)){
                        text[strlen(text)-1]='\0';
                        flag = false;
                    }
                }
                else {
                    flag = true;
                }
            }
            if (event.type == SDL_TEXTINPUT) {
                if (strlen(text) < MAX_NAME_LENGTH-1)
                    strcat(text, event.text.text);
            }

        }
        SDL_RenderPresent(renderer);
        //SDL_FreeSurface(TextSurface);
        //SDL_Delay(5);
    }

    TTF_CloseFont(fnt);
    SDL_StopTextInput();

}

int mx_gameover_menu(SDL_Renderer *renderer, t_gamestate *game) {
    if (game->kill_count) {
        t_player_stats stats = mx_read_leaderboeard_file();
        if (stats.scores[NAME_NUMBER-1] < game->kill_count)
            mx_input_your_name(renderer, game);
    }
    uint clicked = 0;
    int running = 1;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (SDL_QUIT == event.type){

        }
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = 0;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT
                && event.button.x >= GAMEOVERMENU_RESTARTBUTTON_X
                && event.button.x <= GAMEOVERMENU_RESTARTBUTTON_X + GAMEOVERMENU_RESTARTBUTTON_WIDTH
                && event.button.y >= GAMEOVERMENU_RESTARTBUTTON_Y
                && event.button.y <= GAMEOVERMENU_RESTARTBUTTON_Y + GAMEOVERMENU_RESTARTBUTTON_HEIGHT)
                clicked = 1;
            if (event.button.button == SDL_BUTTON_LEFT
                && event.button.x >= GAMEOVERMENU_LEADER_X
                && event.button.x <= GAMEOVERMENU_LEADER_X + GAMEOVERMENU_LEADER_WIDTH
                && event.button.y >= GAMEOVERMENU_LEADER_Y
                && event.button.y <= GAMEOVERMENU_LEADER_Y + GAMEOVERMENU_LEADER_HEIGHT)
                clicked = 2;
            if (event.button.button == SDL_BUTTON_LEFT
                && event.button.x >= GAMEOVERMENU_MENUBUTTON_X
                && event.button.x <= GAMEOVERMENU_MENUBUTTON_X + GAMEOVERMENU_MENUBUTTON_WIDTH
                && event.button.y >= GAMEOVERMENU_MENUBUTTON_Y
                && event.button.y <= GAMEOVERMENU_MENUBUTTON_Y + GAMEOVERMENU_MENUBUTTON_HEIGHT)
                clicked = 3;
        }
    }
    SDL_Rect background_rect = {0, 0, W_WIDTH, W_HEIGHT};
    SDL_Texture *tex;
    switch (clicked) {
        case 1:
            running = 2;
            tex = mx_get_texture(game, TEX_BACKGROUND_GAMEOVER_RETRY);
            mx_restart_gamestate(game);
            mx_set_battle_scene(game);
            break;
        case 2:
            running = 2;
            tex = mx_get_texture(game, TEX_BACKGROUND_GAMEOVER_LEADER);
            game->scene = LEADERBOARD_STATE;
            break;
        case 3:
            running = 2;
            tex = mx_get_texture(game, TEX_BACKGROUND_GAMEOVER_MENU);
            mx_restart_gamestate(game);
            mx_set_start_scene(game);
            break;
        default:
            tex = mx_get_texture(game, TEX_BACKGROUND_GAMEOVER);
            break;
    }
    SDL_RenderCopy(renderer, tex, NULL, &background_rect);



    return running;
}
