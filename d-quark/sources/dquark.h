/***************************************************************************
 *   Copyright (C) 2015 by Roberto Cabral                                  *
 *   rbcabral@ic.unicamp.br                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.   	   *
 *									   *	
 *   A paper with details of this implementation was published             *
 *   in Lightsec  2015, called Fast software implementation of QUARK On a  * 
 *   32 bit architecture. If you want to use this implementation, please   *
 *   cite the paper.           						   *
 *  									   *
 *   For more information, feedback or questions, please, contact me in    *
 *   rbcabral@ic.unicamp.br						   **
 ***************************************************************************/

#include <stdint.h>

#define BENCH 1

#define CAPACITY 20
#define RATE      2
#define WIDTH    24
#define DIGEST WIDTH

//#define ROUNDS_S 4*256
//#define N_LEN_S    128
//#define L_LEN_S     10

#define zero_h 0x0000FFFF
#define zero_l 0xFFFF0000
#define zero_l2 0xFFFFFF00
#define ff00ff00 0xFF00FF00

#define align(a,da,b,db) ((a << da) ^ (b >> db))


#define al(r1,c,r2,d)  asm ("shld %2,%1, %0"\
             : "+r" (r1)\
             : "r" (r2), "i" (c) \
             );


typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t   u8;

typedef struct {
  int len; /* number of bytes read into x from current block */
  u32 x[8];
  u16 *data;
} hashState;


int quark(u16 *out, u16 *in, u64 inlen);
