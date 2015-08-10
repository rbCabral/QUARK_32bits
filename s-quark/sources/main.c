/***************************************************************************
 *   Implementation of S-QUARK by Roberto Cabral                           *
 *   rbcabral@ic.unicamp.br  						   *
 *   This code is placed in the public domain                              *
 *                                                                         *
 *   A paper with details of this implementation was published             *
 *   in Lightsec  2015, called Fast software implementation of QUARK On a  * 
 *   32 bit architecture. If you want to use this implementation, please   *
 *   cite the paper.           						   *
 *  									   *
 *   For more information, feedback or questions, please, contact me in    *
 *   rbcabral@ic.unicamp.br						   *
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