
#ifndef INCLUDE_PGM_ANIMATIONS
#define INCLUDE_PGM_ANIMATIONS

#include <avr/pgmspace.h>

void loadPgmAnimation(uint16_t* target, const uint16_t* source)
{
    memcpy_P(target, source, 256);
}




const uint16_t anim_rand[] PROGMEM = { 0x1801, 0x0105, 0x0304, 0x0309, 0x030e, 0x0313, 0x0318, 0x031d, 0x0322, 0x0327, 0x0100, 0x0303, 0x01e0, 0x0308, 0x01c0, 0x030d, 0x01a0, 0x0312, 0x0180, 0x0317, 0x0120, 0x031c, 0x0140, 0x0321, 0x0160, 0x0326, 0x0143, 0x032a, 0x0121, 0x032d, 0x0100, 0x0328, 0x1756, 0x0126, 0x032d, 0x0105, 0x0328, 0x1756, 0x012b, 0x032d, 0x010a, 0x0328, 0x1756, 0x0130, 0x032d, 0x010f, 0x0328, 0x1756, 0x0135, 0x032d, 0x0114, 0x0328, 0x1756, 0x013a, 0x032d, 0x0119, 0x0328, 0x1756, 0x013f, 0x032d, 0x011e, 0x0328, 0x1756, 0x0144, 0x032d, 0x0123, 0x0328, 0x1756, 0x0903, 0x0908, 0x090d, 0x0912, 0x0917, 0x091c, 0x0921, 0x0926, 0x0903, 0x0908, 0x090d, 0x0912, 0x0917, 0x091c, 0x0921, 0x0926, 0x1102, 0x171c, 0x0107, 0x862a, 0x0429, 0x0928, 0x0103, 0x862a, 0x0429, 0x0928, 0x0107, 0x862a, 0x0429, 0x1d00};

const uint16_t anim_epy[] PROGMEM = { 0x1801, 0x010c, 0x032a, 0x0104, 0x0304, 0x0309, 0x030e, 0x0313, 0x0318, 0x031d, 0x01c8, 0x0300, 0x0306, 0x030c, 0x030f, 0x0310, 0x0315, 0x0316, 0x0319, 0x031b, 0x022a, 0x0303, 0x022a, 0x0308, 0x022a, 0x030d, 0x022a, 0x0312, 0x022a, 0x0317, 0x022a, 0x031c, 0x022a, 0x0321, 0x022a, 0x0326, 0x1102, 0x1714};

const uint16_t anim_spectrum[] PROGMEM = { 0x1801, 0x0107, 0x0304, 0x0309, 0x030e, 0x0100, 0x0303, 0x0155, 0x0308, 0x01ab, 0x030d, 0x01ff, 0x0300, 0x0307, 0x030b, 0x0102, 0x0403, 0x0104, 0x0408, 0x0106, 0x040d, 0x1102, 0x170f};

const uint16_t anim_police[] PROGMEM = { 0x1801, 0x0140, 0x0303, 0x01c0, 0x0308, 0x0106, 0x0304, 0x0309, 0x0105, 0x0380, 0x0100, 0x0300, 0x01ff, 0x0307, 0x1101, 0x0280, 0x0400, 0x01ff, 0x8880, 0x0381, 0x0101, 0x8481, 0x0407, 0x0180, 0x1c80, 0x171e, 0x0106, 0x1b00, 0x170e, 0x1722, 0x01f9, 0x1c00, 0x170e, 0x1722, 0x01ff, 0x0880, 0x0101, 0x0480, 0x170e};

const uint16_t anim_epy2[] PROGMEM  = { 0x1801, 0x0107, 0x0304, 0x0309, 0x0100, 0x0303, 0x0180, 0x0308, 0x0143, 0x032a, 0x022a, 0x0300, 0x0301, 0x0302, 0x0305, 0x0306, 0x0307, 0x110a, 0x170a};

const uint16_t anim_off[] PROGMEM = { 0x1801, 0x110a, 0x1701};

const uint16_t anim_light [] PROGMEM = { 0x1801, 0x0140, 0x0303, 0x01c0, 0x0308, 0x0107, 0x0304, 0x0309, 0x01ff, 0x0300, 0x0301, 0x0302, 0x0305, 0x0306, 0x0307, 0x110a, 0x170f};



void loadNextAnimation(uint16_t* target)
{
		static uint8_t pgm_animation = 0;
		if(pgm_animation == 0) memcpy_P(target, anim_rand, 98 * sizeof(uint16_t));
		else if(pgm_animation == 1) memcpy_P(target, anim_epy, 38 * sizeof(uint16_t));
		else if(pgm_animation == 2) memcpy_P(target, anim_spectrum, 23 * sizeof(uint16_t));	
		else if(pgm_animation == 3) memcpy_P(target, anim_police, 39 * sizeof(uint16_t));	
		else if(pgm_animation == 4) memcpy_P(target, anim_epy2, 19 * sizeof(uint16_t));
		else if(pgm_animation == 5) memcpy_P(target, anim_light, 17 * sizeof(uint16_t));	
		else if(pgm_animation == 6) memcpy_P(target, anim_off, 3 * sizeof(uint16_t));

		pgm_animation++;
		if(pgm_animation == 7) pgm_animation = 0;
}

#endif //INCLUDE_PGM_ANIMATIONS
