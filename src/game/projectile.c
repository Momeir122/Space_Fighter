#include "projectile.h"

#define PROJECTILE_SPEED 10
#define EXPLOSION_TIME 500

sfClock* explosion_clock = NULL;
sfSprite* explosion_sprite = NULL;

Projectile* CreateProjectile(float x, float y, float scale_x, float scale_y, const char* texture_file, unsigned int damage, int direction){
    Projectile* projectile = malloc(sizeof(Projectile));
    sfTexture* texture = sfTexture_createFromFile(texture_file, NULL);
    sfSprite* sprite = sfSprite_create();

    sfSprite_setTexture(sprite, texture, sfFalse);
    sfSprite_setOrigin(sprite, (sfVector2f){167,432});

    projectile->pos = (sfVector2f){x,y};
    projectile->scale = (sfVector2f){scale_x, scale_y};
    projectile->damage = damage;
    projectile->sprite = sprite;
    projectile->texture = texture;
    projectile->direction = direction;

    sfSprite_setPosition(projectile->sprite, projectile->pos);
    sfSprite_setScale(projectile->sprite, projectile->scale);
    if(direction == -1){
        sfSprite_setRotation(projectile->sprite, 0);
    }
    else{
        sfSprite_setRotation(projectile->sprite, 180);
    }

    return projectile;
}

void UpdateProjectile(sfRenderWindow* window, Projectile* projectile){
    projectile->pos.y += PROJECTILE_SPEED * projectile->direction;

    sfSprite_setPosition(projectile->sprite, projectile->pos);

    sfRenderWindow_drawSprite(window, projectile->sprite, NULL);
}

void DestroyProjectile(Projectile* projectile){
    sfSprite_destroy(projectile->sprite);
    sfTexture_destroy(projectile->texture);
    free(projectile);
}

void CreateExplosion(sfVector2f pos){
    if(explosion_sprite ==  NULL){
        sfTexture* explosion_texture = sfTexture_createFromFile("images/explosion.png", NULL);
        explosion_sprite = sfSprite_create();
        sfSprite_setTexture(explosion_sprite, explosion_texture, sfFalse);

        sfSprite_setOrigin(explosion_sprite, (sfVector2f){174,174});
        sfSprite_setScale(explosion_sprite, (sfVector2f){0.3,0.3});
    }
    
    sfSprite_setPosition(explosion_sprite, pos);
    explosion_clock = sfClock_create();
}

void UpdateExplosion(sfRenderWindow* window){
    if(explosion_clock != NULL){
        sfRenderWindow_drawSprite(window, explosion_sprite, NULL);

        int elapsedTime = sfTime_asMilliseconds(sfClock_getElapsedTime(explosion_clock));
        if(elapsedTime > EXPLOSION_TIME){
            explosion_clock = NULL;
        }
    }
}