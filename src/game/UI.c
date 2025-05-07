#include "UI.h"

sfText* hp_text = NULL;
sfText* intro_text = NULL;

void Reset(){
    intro_text = NULL;
}

void Intro(sfRenderWindow* window, sfFont* font){
    if(intro_text == NULL){
        intro_text = sfText_create();
        sfText_setFont(intro_text, font);

        sfText_setString(intro_text, "---------------------- SPACE_FIGHTER ----------------------");
        sfText_setPosition(intro_text, (sfVector2f){0, -150});
    }

    sfText_move(intro_text, (sfVector2f){0, 5});
    sfRenderWindow_drawText(window, intro_text, sfFalse);
}

void UpdateUI(sfRenderWindow* window, sfFont* font, Character* player) {
    if(intro_text == NULL){
        Intro(window, font);
    }
    else{
        if(sfText_getPosition(intro_text).y < sfRenderWindow_getSize(window).y - 100){
            Intro(window, font);
        }
    }

    if (hp_text == NULL) {
        hp_text = sfText_create();
        sfText_setFont(hp_text, font);
    }

    char buffer[50];
    
    sprintf(buffer, "HP : %d", player->stats.hp);
    
    sfText_setString(hp_text, buffer);
    sfText_setPosition(hp_text, (sfVector2f){sfRenderWindow_getSize(window).x - 175, sfRenderWindow_getSize(window).y - 50});
    sfText_setScale(hp_text, (sfVector2f){1,1});
    sfRenderWindow_drawText(window, hp_text, NULL);
}