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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "squark.h"
#include "bench.h"


extern long long total;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t   u8; 

int main(int argc, char **argv){
  u32 *data;
  u32 out[32];
  int inlen;
  hashState state;
  uint64_t i;
  
  inlen = atoi(argv[1]);
  
  data = (u32*)malloc(inlen*sizeof(u32));
  
  inlen = inlen * 8;


  BENCH_FUNCTION(quark, out, data, inlen);
  
  for(i=0;i<8;i++)
    printf("%08X",out[i]);
   printf("\n");
   
   


free(data);

  return 0;
}
