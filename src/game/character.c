#include "character.h"

#define INVINCIBILITY_TIME 1000
#define COLIISION_DAMAGE 10

sfClock* playerInvincibility_clock = NULL;

Character* CreateCharacter(float x, float y, float scale_x, float scale_y,float angle, unsigned int origin_x, unsigned int origin_y, const char* idleTexture_file,const char* hitTexture_file, float speed){
    Character* character = malloc(sizeof(Character));
    sfTexture* idleTexture = sfTexture_createFromFile(idleTexture_file, NULL);
    sfTexture* hitTexture = sfTexture_createFromFile(hitTexture_file, NULL);
    sfSprite* sprite = sfSprite_create();
    Stats stats;

    sfSprite_setTexture(sprite, idleTexture, sfFalse);
    sfSprite_setOrigin(sprite, (sfVector2f){origin_x,origin_y});

    character->pos = (sfVector2f){x, y};
    character->scale = (sfVector2f){scale_x, scale_y};
    character->idleTexture = idleTexture;
    character->hitTexture = hitTexture;
    character->sprite = sprite;
    character->stats = stats;
    character->projectile = NULL;
    character->angle = angle;

    character->stats.hp = 100;
    character->stats.normalSpeed = speed;
    character->stats.hitSpeed = speed /2;
    character->stats.speed = speed;

    sfSprite_setPosition(character->sprite, character->pos);
    sfSprite_setScale(character->sprite, character->scale);
    sfSprite_setRotation(sprite, character->angle);

    return character;
}

void UpdatePlayer(sfRenderWindow* window, Character* character){
    UpdateExplosion(window);
    sfSprite_setPosition(character->sprite, character->pos);

    if(sfKeyboard_isKeyPressed(sfKeyQ)){
        character->pos.x -= character->stats.speed;
    }
    if(sfKeyboard_isKeyPressed(sfKeyD)){
        character->pos.x += character->stats.speed;
    }

    if(sfKeyboard_isKeyPressed(sfKeyZ)){
        character->pos.y -= character->stats.speed;
    }
    if(sfKeyboard_isKeyPressed(sfKeyS)){
        character->pos.y += character->stats.speed;
    }
    if(sfKeyboard_isKeyPressed(sfKeySpace)){
        Shoot(character, -1);
    }

    if(character->projectile != NULL){
        UpdateProjectile(window, character->projectile);

        if(character->projectile->pos.y <= 0){
            DestroyProjectile(character->projectile);
            character->projectile = NULL;
        }
    }

    /*------------ Player Invincibility ------------*/
    if(playerInvincibility_clock != NULL){
        int elapsedTime = sfTime_asMilliseconds(sfClock_getElapsedTime(playerInvincibility_clock));
        if(elapsedTime > INVINCIBILITY_TIME){
            playerInvincibility_clock = NULL;
        }

        sfSprite_setTexture(character->sprite, character->hitTexture, sfFalse);
        character->stats.speed = character->stats.hitSpeed;
    }
    else{
        sfSprite_setTexture(character->sprite, character->idleTexture, sfFalse);
        character->stats.speed = character->stats.normalSpeed;
    }

    sfRenderWindow_drawSprite(window, character->sprite, NULL);
}

void DestroyCharacter(Character* character){
    sfSprite_destroy(character->sprite);
    sfTexture_destroy(character->idleTexture);
    free(character);
}

void Shoot(Character* character, int direction){
    if(character->projectile == NULL){
        character->projectile = CreateProjectile(character->pos.x, character->pos.y, 0.06,0.06, "images/projectile.png", 10, direction);
    }
}

Character* CreateEnemy(sfRenderWindow* window){
    return CreateCharacter(randint(1, sfRenderWindow_getSize(window).x/100 -1) * 100,randint(0, 200), 0.5, 0.5, 180, 68,79,"images/spaceship.png","images/spaceship_hit.png", 5);
}

Character** CreateEnemies(sfRenderWindow * window, int n){
    Character** enemies = malloc(sizeof(Character) * n);
    for(int i = 0; i < n; i++){
        srand(i);
        enemies[i] = CreateEnemy(window);
    }

    return enemies;
}

void UpdateEnemies(sfRenderWindow* window, Character* enemies[], unsigned int n, Character* player){
    for(int i = 0; i < n; i++){
        if(enemies[i] != NULL){
            sfRenderWindow_drawSprite(window, enemies[i]->sprite, NULL);
            sfSprite_setPosition(enemies[i]->sprite, enemies[i]->pos);

            enemies[i]->pos.y += enemies[i]->stats.speed;

            Shoot(enemies[i], 1);

            if(enemies[i]->projectile != NULL){
                UpdateProjectile(window, enemies[i]->projectile);

                /*---------------- Checking Projectiles Collisions ------------------*/

                sfFloatRect r1 = sfSprite_getGlobalBounds(enemies[i]->projectile->sprite);
                sfFloatRect r2 = sfSprite_getGlobalBounds(player->sprite);
                sfFloatRect r3 = sfSprite_getGlobalBounds(enemies[i]->sprite);
                if(sfFloatRect_intersects(&r1,&r2, NULL)){
                    HitCharacter(player, enemies[i]->projectile);
                }

                if(player->projectile != NULL){
                    sfFloatRect r4 = sfSprite_getGlobalBounds(player->projectile->sprite);
                    r4.height *= 2;r4.width *= 2;
                    if(sfFloatRect_intersects(&r3,&r4, NULL)){
                        HitCharacter(enemies[i], player->projectile);
                    }
                }
                if(sfFloatRect_intersects(&r3,&r2, NULL)){
                    if(playerInvincibility_clock == NULL){
                        player->stats.hp -= COLIISION_DAMAGE;
                        playerInvincibility_clock = sfClock_create();
                    }
                }

                /*-------------- Checking if enemy projectile is out of window -----------------*/
        
                if(enemies[i]->projectile->pos.y >= sfRenderWindow_getSize(window).y){
                    DestroyProjectile(enemies[i]->projectile);
                    enemies[i]->projectile = NULL;
                }
            }

            /*-------------- Making the enemy die ------------------------*/
            if(enemies[i]->stats.hp <= 0){
                CreateExplosion(enemies[i]->pos);
                enemies[i] = NULL;
            }

            if(enemies[i] != NULL){
                if(enemies[i]->pos.y >= sfRenderWindow_getSize(window).y - 100){
                    enemies[i] = CreateEnemy(window);
                }
            }
            else{
                enemies[i] = CreateEnemy(window);
            }
        }
    }
}

void DestroyEnemies(Character** enemies, unsigned int n){
    for(int i = 0; i < n; i++){
        DestroyCharacter(enemies[i]);
    }
}

void HitCharacter(Character* character, Projectile* projectile){
    if(projectile->direction == -1){
        character->stats.hp -= projectile->damage;
    }
    else{
        if(playerInvincibility_clock == NULL){
            character->stats.hp -= projectile->damage;
            playerInvincibility_clock = sfClock_create();
        }
    }
}