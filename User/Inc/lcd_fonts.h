#ifndef __FONTS_H
#define __FONTS_H

#include <stdint.h>


// Font related structure definition
typedef struct _pFont
{    
	const uint8_t *pTable; // word module array address
	uint16_t Width; // Font width of a single character
	uint16_t Height; // Font length of a single character
	uint16_t Sizes; //The number of font data for a single character
	uint16_t Table_Rows; // This parameter is only used by Chinese character fonts and represents the row size of the two-dimensional array.
} pFONT;

/*------------------------------------ Medium font ---------- -----------------------------------*/

extern pFONT CH_Font12; // 1212 font
extern pFONT CH_Font16; // 1616 font
extern pFONT CH_Font20; // 2020 font
extern pFONT CH_Font24; // 2424 font
extern pFONT CH_Font32; // 3232 font

/*------------------------------------ASCII font--------- ----------------------------------*/

extern pFONT Font32; // 3216 font
extern pFONT Font24; // 2412 font
extern pFONT Font20; // 2010 font
extern pFONT Font16; // 1608 font
extern pFONT Font12; // 1206 font

#endif 
 