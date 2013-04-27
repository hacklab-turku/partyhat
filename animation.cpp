
#include "animation.h"


void animationUpdateDefault(){
    vectors[0].direction += 1;
    vectors[1].direction += 2;
    vectors[2].direction += 3;
}

void animationInitDefault(){
    vectors[0].r = 255;
    vectors[0].g = 0;
    vectors[0].b = 0;
    vectors[0].direction = 0;
    vectors[0].sector = 6;

    vectors[1].r = 0;
    vectors[1].g = 255;
    vectors[1].b = 0;
    vectors[1].direction = 83;
    vectors[1].sector = 6;

    vectors[2].r = 0;
    vectors[2].g = 0;
    vectors[2].b = 255;
    vectors[2].direction = -83;
    vectors[2].sector = 6;
}

void animationUpdatePolice(){
    static int8_t dir = 15;
    vectors[0].r += dir;
    vectors[1].b -= dir;
    if (vectors[0].r == 0) dir = 15;
    if (vectors[1].b == 0) dir = -15;
}

void animationInitPolice(){
    vectors[0].r = 0;
    vectors[0].g = 0;
    vectors[0].b = 0;
    vectors[0].direction = 0;
    vectors[0].sector = 7;

    vectors[1].r = 0;
    vectors[1].g = 0;
    vectors[1].b = 255;
    vectors[1].direction = -128;
    vectors[1].sector = 7;

}



uint8_t currentAnimation;


void selectAnimation(const uint8_t id){
    currentAnimation = id;

    for (uint8_t i = 0; i < NUMBER_OF_VECTORS; i++){
        vectors[i].r = 0;
        vectors[i].g = 0;
        vectors[i].b = 0;
        vectors[i].direction = 0;
        vectors[i].sector = 0;
    }


    switch (currentAnimation){
    case 1:
        animationInitDefault();
        break;

    case 2:
        animationInitPolice();
        break;

    default:
        break;

    }
}

void updateAnimation(const uint32_t time){

    switch (currentAnimation){
    case 1:
        animationUpdateDefault();
        break;

    case 2:
        animationUpdatePolice();
        break;

    default:
        break;

    }


}
