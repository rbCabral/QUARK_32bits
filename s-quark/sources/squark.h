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

#include <stdint.h>

#define BENCH 1

#define CAPACITY 28
#define RATE      4
#define WIDTH    32
#define DIGEST WIDTH

#define ROUNDS_S 4*256
#define N_LEN_S    128
#define L_LEN_S     10

#define zero_h 0x0000FFFF
#define zero_l 0xFFFF0000

#define get_low(v,d,z)	(( v >> d) & z)
#define get_high(v,d,z)	(( v << d) & z)
#define align(a,da,b,db) ((a << da) ^ (b >> db))
#define align_l(a,da,b,db) ((a << da) ^ (b << db))

#define al(r1,r2,c)  asm ("shld %2,%1, %0"\
             : "+r" (r1)\
             : "r" (r2), "i" (c) \
             );

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t   u8; 

typedef struct {
  int len; /* number of bytes read into x from current block */
  u32 x[8];
  u32 *data;
} hashState;



int quark(u32 *out, u32 *in, u64 inlen);
