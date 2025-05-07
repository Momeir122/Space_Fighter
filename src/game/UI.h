#ifndef UI_H
#define UI_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "character.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#pragma GCC diagnostic pop

void Reset();
void Intro(sfRenderWindow* window, sfFont* font);
void UpdateUI(sfRenderWindow* window, sfFont* font, Character* player);

#endif