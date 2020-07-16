#include "DEC2020.h"



t_player_stats mx_read_leaderboeard_file(void) {
    t_player_stats stats;
    FILE *board;
    board = fopen(LEADERBOARD_FILE_NAME, "r+");
    for (int i = 0; i < NAME_NUMBER; i++) {
        fscanf(board, "%d %s", &stats.scores[i], stats.names[i]);
    }
    fclose(board);
    return stats;
}

void file_write(t_player_stats stats) {
    FILE *board;
    board = fopen(LEADERBOARD_FILE_NAME, "w+");
    for (int i = 0; i < NAME_NUMBER; i++) {
        fprintf(board, "%d %s\n", stats.scores[i], stats.names[i]);
    }
    fclose(board);
}

t_player_stats board_updated(t_player_stats stats, char new_name[MAX_NAME_LENGTH], int new_score) {
    int i = 0;
    while (i < NAME_NUMBER && new_score <= stats.scores[i]) {
        i++;
    }
    if (i < NAME_NUMBER) {
        for (int j = NAME_NUMBER-2; j >= i; j--) {
            stats.scores[j+1] = stats.scores[j];
            strcpy(stats.names[j+1], stats.names[j]);
        }
        stats.scores[i] = new_score;
        strcpy(stats.names[i], new_name);
    }
    return stats;
}

void mx_leaderboard(SDL_Renderer *renderer, t_gamestate *game) {
    //uint clicked = 0;
    //int running = 1;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (SDL_QUIT == event.type){

        }
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            mx_set_start_scene(game);
    }

    SDL_Rect background_rect = {0, 0, W_WIDTH, W_HEIGHT};
    SDL_Texture *tex = mx_get_texture(game, TEX_LEADERBOARD);
    SDL_RenderCopy(renderer, tex, NULL, &background_rect);

    t_player_stats stats = mx_read_leaderboeard_file();
    TTF_Font * fnt = TTF_OpenFont("resource/RubikMonoOne-Regular.ttf", 32); // Загружаем шрифт по заданному адресу размером sz
    SDL_Color clr; // Тип для цвета. 4 числа — R, G, B и A, соответственно.
    clr.r = 255;
    clr.g = 255;  // Зададим параметры цвета
    clr.b = 255;
    clr.a = 255;
    for (int i = 0; i < NAME_NUMBER; i++) {

        SDL_Rect dest;
        dest.x = 250;
        dest.y = 165 + i * 70;
        SDL_Surface * TextSurface = TTF_RenderText_Blended(fnt, stats.names[i], clr); // Переносим на поверхность текст с заданным шрифтом и цветом

        SDL_Texture* Name = SDL_CreateTextureFromSurface(renderer, TextSurface); //now you can convert it into a texture

        SDL_Rect Message_rect; //create a rect
        Message_rect.x = 250;  //controls the rect's x coordinate
        Message_rect.y = 165 + i * 70; // controls the rect's y coordinte
        Message_rect.w = 48 * strlen(stats.names[i]); // controls the width of the rect
        Message_rect.h = 48;
        SDL_RenderCopy(renderer, Name, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

        //SDL_BlitSurface(TextSurface, NULL, renderer, &dest);
         // Освобождаем память уже ненужной поверхности

        SDL_Rect destScore;
        destScore.x = 1147;
        destScore.y = 165 + i * 70;
        char score_text[11];
        sprintf(score_text, "%.5d", stats.scores[i]);
        //write(2,score_text,11);
        SDL_Surface *score_surface = TTF_RenderText_Blended(fnt, score_text, clr); // Переносим на поверхность текст с заданным шрифтом и цветом
        SDL_Texture *score = SDL_CreateTextureFromSurface(renderer, score_surface); //now you can convert it into a texture

        SDL_Rect score_rect = {1147, 165 + i * 70, 48 * strlen(score_text), 48}; //create a rect

        SDL_RenderCopy(renderer, score, NULL, &score_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

        //SDL_BlitSurface(TextSurface, NULL, renderer, &dest);
        SDL_FreeSurface(TextSurface);
        SDL_FreeSurface(score_surface); // Освобождаем память уже ненужной поверхности



    }
    TTF_CloseFont(fnt); // Закрываем шрифт
}
