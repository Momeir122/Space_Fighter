#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "character.h"
#include "UI.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#pragma GCC diagnostic pop

#define FPS 60
#define WIN_WIDTH 1200
#define WIN_HEIGHT 800
#define ENEMY_NUMBER 10

sfRenderWindow* create_window(const char* title, unsigned int w, unsigned int h, unsigned int bbp){
    sfVideoMode videoMode = {w, h, 32};
    return sfRenderWindow_create(videoMode, "SPACE FIGHTER", sfDefaultStyle, NULL);
}

void Game(){
    Reset();
    sfRenderWindow * window = create_window("Window", WIN_WIDTH, WIN_HEIGHT, 32);
    sfRenderWindow_setFramerateLimit(window, FPS);
    sfEvent event;

    sfFont* defaultFont = sfFont_createFromFile("fonts/console.ttf");

    Character* player = CreateCharacter(400,200,0.3,0.3, 0,68,79, "images/spaceship.png", "images/spaceship_hit.png", 5);
    Character** enemies = CreateEnemies(window, ENEMY_NUMBER);

    while (sfRenderWindow_isOpen(window))
    {
        sfRenderWindow_clear(window, (sfColor){0,0,0});

        while(sfRenderWindow_pollEvent(window, &event)){
            if(event.type == sfEvtClosed){
                sfRenderWindow_close(window);
            }
        }

        UpdateUI(window, defaultFont, player);
        UpdatePlayer(window, player);
        UpdateEnemies(window, enemies, ENEMY_NUMBER, player);

        /*--------- Killing player -----------------*/
        if(player->stats.hp <= 0){
            sfRenderWindow_close(window);
            Game();
        }

        sfRenderWindow_display(window);
    }
    
    sfRenderWindow_destroy(window);
    DestroyCharacter(player);
    DestroyEnemies(enemies, ENEMY_NUMBER);

}

int main(void){
    Game();

    return 0;
}

// gcc src/game/*.c -o bin/out.exe -I./include -L./lib -lcsfml-graphics -lcsfml-window -lcsfml-system