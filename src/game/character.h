#ifndef CHARACTER_H
#define CHARACTER_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "projectile.h"
#pragma GCC diagnostic pop

typedef struct Stats{
    float speed;
    float normalSpeed;
    float hitSpeed;
    int hp;
}Stats;

typedef struct Character{
    sfVector2f pos;
    sfVector2f scale;
    float angle;
    sfSprite* sprite;
    sfTexture* idleTexture;
    sfTexture* hitTexture;
    Stats stats;
    Projectile* projectile;
}Character;

Character* CreateCharacter(float x, float y, float scale_x, float scale_y,float angle, unsigned int origin_x, unsigned int origin_y, const char* idleTexture_file,const char* hitTexture_file, float speed);
void UpdatePlayer(sfRenderWindow* window, Character* character);
void DestroyCharacter(Character* character);
void Shoot(Character* character, int direction);
Character** CreateEnemies(sfRenderWindow * window, int n);
void UpdateEnemies(sfRenderWindow* window, Character* enemies[], unsigned int n, Character* player);
void DestroyEnemies(Character** enemies, unsigned int n);
void HitCharacter(Character* character, Projectile* projectile);
Character* CreateEnemy(sfRenderWindow * window );

#endif