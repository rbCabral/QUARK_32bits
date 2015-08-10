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


#include "dquark.h"

u32 iv2[] ={0xCC6C4AB7, 0xD11FA9BD, 0xF6EEDE00, 0x03D87B68, 0xF91BAA70, 0x6C20E900};
u32 l[] = { 0xffc070fd, 0xC4F8CFAC, 0xB2480204, 0x8832684A, 0x87AEB6C0, 0x306CC2B5, 0xC6FC479E, 0xDA0285AA, 0x3EF25826, 0x451B703C, 0x77F218B7, 0x4356796C,
	    0x8224C0B1, 0x4ECE2FD4, 0x5DAC336A, 0xE9E9A93,  0x83E737A2, 0xADF09D1D, 0x7DA4214A, 0xC73FB08D, 0x393C3763, 0x1EFA4A06};

void static inline first_perm(u32 *x, u32 *t){
    u32 aux1, aux2, aux3;
    
    aux1 = align(x[3],19,x[4],13);
    aux2 = align(x[4],22,x[5],10);
    t[2] = ((x[0] << 11) & zero_l) ^ ((x[3] >> 7) & zero_h );
    t[0] = (x[0] & zero_l) ^ ((x[3] >> 16));
    t[1] = (x[3] & zero_l) ^ ((aux1 >> 15) & zero_h);
    t[0] = t[0] ^ t[1];
    aux3 = align(x[0],18,x[1],14);
    t[4] = ((aux3 << 9) & zero_l) ^ ((aux1 >> 10) & zero_h);
    t[13]= ((aux3 << 1) & zero_l) ^ (aux1 >> 16);
    t[3] = (aux3 & zero_l) ^ ((aux2 >> 7) & zero_h);
    t[0] = t[0] ^ t[3];
    t[5] = ((x[1] << 4) & zero_l) ^ ((x[4] >> 10) & zero_h);
    t[6] = ((x[1] << 10)& zero_l) ^ ((x[4] >> 4) & zero_h);
    t[7] = ((x[1] << 15) & zero_l) ^ ((x[4] >> 1) & zero_h);
    t[8] = (align(x[1],26,x[2],6) & zero_l) ^ (aux2 >> 16);
    t[9] = ((x[2] << 3) & zero_l) ^ ((x[5] >> 13) & zero_h);
    t[10]= ((x[2] << 7) & zero_l) ^ ((x[5] >> 11) & zero_h);
    t[0] = t[0] ^ (x[2] & zero_l);
    t[12]= ((x[2] << 15) & zero_l) ^ ((x[5] >> 2) & zero_h);
}
					
    
void static inline third_perm(u32 *x, u32 *t){
    u32 aux1, aux2, aux3;
    
    aux1 = align(x[1],20,x[2],12);
    aux2 = align(x[2],19,x[6],13);
    aux3 = align(x[4],22,x[5],10 );
    t[0] = ((x[0] << 16) & zero_l) ^ (x[3] & zero_h);
    t[1] = ((x[3] << 16) & zero_l) ^ ((x[4] >> 12) & zero_h);
    t[0] = t[0] ^ t[1];
    t[2] = (align(x[0],27,x[1],5) & zero_l) ^ (align(x[3],9,x[4],23) & zero_h );
    t[3] = ((x[1] << 2)  & zero_l) ^ ((x[5] >> 1) & zero_h);
    t[0] = t[0] ^ t[3];
    t[5] = (aux1 & zero_l) ^ (aux3  >> 16);
    t[6] = ((aux1 << 6) &  zero_l) ^ ((aux3 >>10 ) & zero_h);
    t[7] = ((aux1 << 11) & zero_l) ^ ((aux3 >> 7) & zero_h);
    t[8] = ((x[2] << 10) & zero_l) ^ ((x[5] >> 10) & zero_h);
    aux1  = (x[5] << 19) ^ ((x[6] & zero_h) << 3 );
    t[9] = ( aux2 & zero_l) ^ (aux1 >> 16);
    t[10]= ((aux2 << 4) & zero_l) ^ ((aux1 >>14) & zero_h);
    t[0] = t[0] ^ (x[2] << 16);
    t[12]= ((aux2 << 12) & zero_l) ^ ((aux1 >> 5) & zero_h);
    t[13]= ((x[1] << 3) & zero_l) ^ ((x[4] >> 13 )& zero_h);
    t[4] = ((x[1] << 11) & zero_l) ^ ((x[4] >> 7) & zero_h);
}

void static inline first_h(u32 *x, u32 *h){
    u32 aux1;
    
    h[0] = (x[3] << 4) & zero_l;
    h[1] = (align(x[1],25,x[2],7) & zero_l);
    h[2] = (x[1] << 3) & zero_l;
    h[3] = (x[5] << 15) & zero_l;
    h[4] = (x[2] << 4) & zero_l;
    h[5] = (x[0] << 1) & zero_l;
    h[6] = (x[0] << 5) & zero_l;
    h[5] = h[5] ^ h[6];
    h[7] = (x[1] << 8) & zero_l;
    aux1 = align(x[4],23,x[5],9);
    h[8] = (aux1 << 6) & zero_l;
    h[9] = (x[3] << 2) & zero_l;
    h[10]= (aux1 & zero_l);
    h[10] = h[10] ^ h[8];
    h[11]= (x[1] << 16);
    h[10] = h[10] ^ h[11];
    h[12]= (x[2] << 8) & zero_l;
    h[13]= (x[3] << 12) & zero_l;
    h[9] = h[9] ^ h[13];
    h[9] = h[9] ^ h[7];
    h[5] = h[5] ^ h[9];
    h[14]= (align(x[3],24,x[4],8) & zero_l);
    h[10] = h[10] ^ h[14];
    h[10] = h[10] ^ h[5];
}

void static inline first_h2(u32 *x, u32 *h){
    u32 aux1;

    h[9] = align(x[3],18,x[4],14);
    h[1] = (x[2] << 9) & zero_l;
    h[3] = ((x[5] << 31) ^ ((x[6] & zero_h) << 15)) & zero_l;
    h[0] = (h[9] << 2) & zero_l;
    h[13]= (h[9] << 10) & zero_l;
    h[9] = h[9] & zero_l;
    h[9] = h[9] ^ h[13];
    h[2] = align(x[1],19,x[2],13);
    h[7] = (h[2] << 5) & zero_l;
    h[9] = h[9] ^ h[7];
    h[2] = h[2] & zero_l;
    h[4] = align(x[2],20,x[6],12);
    h[12]= (h[4] << 4) & zero_l;
    h[4] = h[4] & zero_l;
    h[5] = align(x[0],17,x[1],15);
    h[6]= (h[5] << 4) & zero_l;
    h[5] = h[5] & zero_l;
    h[5] = h[5] ^ h[6];
    h[5] = h[5] ^ h[9];
    h[8] = (x[5] << 13) & zero_l;
    h[10]= (x[5] << 7) & zero_l;
    h[10] = h[10] ^ h[8];
    h[11]= x[2] & zero_l;
    h[10] = h[10] ^ h[11];
    h[14]= (x[4] << 8) & zero_l;
    h[10] = h[10] ^ h[14];
    h[10] = h[10] ^ h[5];
}

u32 static inline first_comp(u32 *t, u32 *h, u32 *temp, u32 l){

  u32 aux1, aux2, aux3;
  
    temp[0] = t[0] ^ t[2] ^ t[4] ^ t[5] ^ t[6] ^ t[7] ^ t[8] ^ t[9] ^ t[10];
    temp[0] = temp[0] ^ t[6] & (t[7] ^ (t[10] & t[7] & t[9]) ^ (t[5] & t[4]) & ~((t[13] & t[2] ) ^ (t[9] & t[8] & t[7]) ))    ^ (t[13] & t[2] ) ^ (t[10] & t[9] & t[8] );
    temp[1] = ((t[10] ^ (t[10] & t[4] & t[13]) ^ (t[10] & t[7] &  t[9] & t[8]))  ^ (t[8] &  t[5] & t[2]));
    aux1 = temp[0] ^ t[12] & temp[1];
   
    
    temp[2]=0;
    temp[2] ^= h[10] ^ h[12] ^ l ^ h[0] & ( h[4] ^ (h[2] & h[1] ) ^ (h[1] & h[4]) ) ^ (h[1] & h[4]) ^ h[2] ^ (h[2] & l );
    
    temp[3] = ~((h[4]) ^ h[1] & (h[0] ^ (h[2] & l)));
    
    aux2 = h[3] & temp[3] ^ temp[2];
    
    aux2 = (aux2 & zero_l) ^ ((aux2 >> 16) & zero_h);
    
    return aux1 ^ aux2;
}



u32 static inline second_comp(u32 *t, u32 *h, u32 *temp){

    u32 aux1, aux2;
  
    aux1 = temp[0] ^ t[12] & temp[1];
    aux2 = h[3] & temp[3] ^ temp[2];
    aux2 = (aux2 >> 16) ^ (aux2 & zero_l);
  
    return aux1 ^ aux2;
}
/*
#define first_comp(id)				\
						\
						fi = t[0] ^ t[2] ^ t[4] ^ t[5] ^ t[6] ^ t[7] ^ t[8] ^ t[9] ^ t[10];\
						fi = fi ^ t[6] & (t[7] ^ (t[10] & t[7] & t[9]) ^ (t[5] & t[4]) & ~((t[13] & t[2] ) ^ (t[9] & t[8] & t[7]) ))    ^ (t[13] & t[2] ) ^ (t[10] & t[9] & t[8] );\
						aux8 = ((t[10] ^ (t[10] & t[4] & t[13]) ^ (t[10] & t[7] &  t[9] & t[8]))  ^ (t[8] &  t[5] & t[2]));\
						aux7 = fi ^ t[12] & aux8;\
						\
						\
						rhp=0;\
						rhp ^= h[10] ^ h[12] ^ id ^ h[0] & ( h[4] ^ (h[2] & h[1] ) ^ (h[1] & h[4]) ) ^ (h[1] & h[4]) ^ h[2] ^ (h[2] & id );\
						\
						ah1 = ~((h[4]) ^ h[1] & (h[0] ^ (h[2] & id)));\
						\
						rh = h[3] & ah1 ^ rhp;\
						\
						rh = (rh & zero_l) ^ ((rh >> 16) & zero_h);\
						aux7 = aux7 ^ rh;\*/

// #define second_comp(id)				\
// 						aux7 = fi ^ t[12] & aux8;\
// 						\
// 						rh = h[3] & ah1 ^ rhp;\
// 						\
// 						rh = (rh >> 16) ^ (rh & zero_l);\
// 						aux7   = aux7 ^ rh;\



	   

int permute_s( u32 * x ) {
  /* state of 256=2x128 bits */
  
  u32 aux1,aux2,aux3,aux4,aux5,aux6,aux7,aux8;
  u32 ah1,ah2,rh=0,rhp=0,fi;
  
  int i,idd;
  
 

  u32 temp[4];
  u32 t[13];
  u32 h[13];
  
  for(i=0;i<22;i++){
      /*i =0*/
  
       first_perm(x,t);
       first_h(x,h);
       aux7 = first_comp(t,h,temp,l[i]);
       
//        fi = temp[0];
// aux8 = temp[1];
// rhp = temp[2];
// ah1 = temp[3];
       
       x[2] = (x[2] & zero_l2) ^ (aux7 >> 24);
       x[5] = (x[5] & zero_l2) ^ ((aux7 & zero_h) >> 8);
      
       /*second perm and h*/
       t[12]= ((x[2] << 15) & zero_l) ^ ((x[5] >> 2) & zero_h);\
       h[3] = (x[5] << 15);
       aux7 = second_comp(t,h,temp);
      
       x[6] = (aux7 << 8);

       third_perm(x,t);
       first_h2(x,h);
       aux7 = first_comp(t,h,temp,(l[i] << 16));
       
//        fi = temp[0];
// aux8 = temp[1];
// rhp = temp[2];
// ah1 = temp[3];
       
       x[6] = (x[6] & ff00ff00) ^ ((aux7 & ff00ff00) >> 8);
       
       /*fourth perm and h*/
       t[12]= ((x[6] >> 1) & zero_l) ^ ((x[6] >> 2) & zero_h);\
       h[3] = (x[6] << 15);

       aux7 = second_comp(t,h,temp);
      
       x[0] = (x[6] & zero_l) ^ (((aux7 >> 8) & zero_h));
       x[3] = (x[6] << 16) ^ (((aux7 << 8) & zero_h));
       
       /*i =1*/
       aux1 = x[0];
       x[0] = x[1];
       x[1] = x[2];
       x[2] = aux1;
       
       aux1 = x[3];
       x[3] = x[4];
       x[4] = x[5];
       x[5] = aux1;
  
       }       
  
   return 0;
 }



/* permutation of the state */
static void permute(u32 * x) {
permute_s( x );
}


/* initialization of the IV */
int init(hashState *state,u16 *data,int inlen)
{
  int i;
  /* initialize state */
  for (i = 0; i < 6; i++){
    state->x[i] = iv2[i];
  }
  
  state->len = inlen;
  state->data = data;
  
  return 0;
}

int update(hashState *state)
{
  int i,k=1;
  u16 *u = state->data;
  int len = state->len;
  u32 m1  = 0x00FFFF00;
   
  while (len >= 16) {
  /* xor state with each bit */
      state->x[5] ^= (*u << 8);
      u+=1;
      len-=16;
      permute(state->x);
  }
  
  state->len = len;

  return 0;
}

int final(hashState *state, u16 *out)
{
  uint16_t temp;
  int i,outbytes=0,j;
  u32 aux;
  u32 m1  = 0x00FFFF00;
  u32 m3  = 0xFFFF0000;
  u32 m2  = 0x0000FFFF;
  
  temp = *state->data;
  temp = temp >> (15 - (state->len)); 
  temp |= 0x1;
  temp = temp << (15 - (state->len));
  
  state->x[5] ^= ((temp << 8) & m1);
  permute(state->x);
  
  out[outbytes++] = ((state->x[5] >> 8));
  permute(state->x);
  
    /* extract one byte */
    for(i=0;i<10;i++) {
      out[outbytes++] = ((state->x[5] >> 8));
      permute(state->x);
    }
  
  return 0;
}


int quark(u16 *out, u16 *in, u64 inlen) {
  /* inlen in bytes */

  hashState state;
  init(&state,in,inlen);
  update(&state);
  final(&state, out);
  
  return 0;
}
