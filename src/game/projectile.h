#ifndef PROJECTILE_H
#define PROJECTILE_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include <stdlib.h>
#pragma GCC diagnostic pop

typedef struct Projectile{
    sfVector2f pos;
    sfVector2f scale;
    sfSprite* sprite;
    sfTexture* texture;
    int direction;
    unsigned int damage;
}Projectile;

Projectile* CreateProjectile(float x, float y, float scale_x, float scale_y, const char* texture_file, unsigned int damage, int direction);
void UpdateProjectile(sfRenderWindow* window, Projectile* projectile);
void DestroyProjectile(Projectile* projectile);
void CreateExplosion(sfVector2f pos);
void UpdateExplosion(sfRenderWindow* window);

#endif