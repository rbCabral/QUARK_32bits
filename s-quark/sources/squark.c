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



#include "squark.h"


  const u32 l[] ={0xffc070fd, 0xC4F8CFAC, 0xB2480204, 0x8832684A, 0x87AEB6C0, 0x306CC2B5, 0xC6FC479E, 0xDA0285AA, 0x3EF25826, 0x451B703C, 0x77F218B7, 0x4356796C,
	  0x8224C0B1, 0x4ECE2FD4, 0x5DAC336A, 0xE9E9A93,  0x83E737A2, 0xADF09D1D, 0x7DA4214A, 0xC73FB08D, 0x393C3763, 0x1EFA4A06, 0x8CBA5A22, 0xCD291876,
	  0xF05CAE77, 0x733ABBD9, 0x44D8872F, 0x94CCAA7E, 0x635E6B4C, 0x4B85EAAF, 0xF4152F15, 0xEEA6E471};
	  
  const u32 iv2[] ={0x397251ce,0xe1de8aa7,0x3ea26250,0xc6d7be12, 0x8cd3e79d,0xd718c24b,0x8a19d09c,0x2492da5d}; 
             
  /*Compute the input values of the function u*/
void static inline first_perm(const u32 * x ,u32 * t){
  
    u32 aux0,aux1,aux2,aux3,aux4;

    t[2] = (x[0] << 16) ^ (x[1] >> 16);
    //al(t[2],x[1],16);
    aux0 = (x[6] << 16) ^ (x[7] >> 16);
    aux1 = (x[4] << 16) ^ (x[5] >> 16);
    t[3] = ((t[2] << 10) & zero_l) ^ ((aux0 >> 4) & zero_h);
    t[13]= ((t[2] << 12) & zero_l) ^ ((aux1 >> 4) & zero_h);
    aux2 = (x[1] << 16) ^ (x[2] >> 16);
    aux3 = (x[2] << 16) ^ (x[3] >> 16);
    aux4 = (x[5] << 16) ^ (x[6] >> 16);

    t[4] = ((x[1] << 7) & zero_l) ^ ((x[5] >> 11) & zero_h);
    t[5] = ((aux2 << 4) & zero_l) ^ ((aux4 >> 8) & zero_h);
    t[0] = ((x[0] & zero_l) ^ (x[4] >> 16));
    t[0] = t[0] ^ (t[0] << 16);
    t[2] = (x[0] << 16) ^ ((x[4] >> 3) & zero_h);
    t[6] = ((aux2 << 13) & zero_l) ^ ((x[6] >> 15) & zero_h);
    t[7] = ((x[2] << 5) & zero_l) ^ ((x[6] >> 11) & zero_h);
    t[8] = ((aux3 << 4) & zero_l) ^ ((x[6] >> 1) & zero_h);
    t[1]=  ((aux3 << 14) & zero_l) ^ ((aux1 >> 2) & zero_h);
    t[0] = t[0] ^ t[1];
    t[0] = t[0] ^ t[3];
    t[9] = ((x[3] << 1) & zero_l) ^ (x[7] >> 16);
    t[10]= ((x[3] << 7) & zero_l) ^ ((x[7] >> 11) & zero_h);
    t[12] = ((x[3] <<15) & zero_l) ^ ((x[7] >> 3) & zero_h);
    t[0] = t[0] ^ t[2] ^ t[4] ^ t[5] ^ t[6] ^ t[7] ^ t[8] ^ t[9] ^ t[10];

}
						
/*Compute the input values of the function u*/						
void static inline second_perm(const u32 * x ,u32 * t, u32 ax7){
  
    u32 aux0,aux2,aux3,aux4;
						    
    aux2 = (x[5] << 16) ^ (x[6] >> 16);
    aux3 = (x[6] << 16) ^ (x[7] >> 16);
    
    t[3] = ((x[1] << 10) & zero_l) ^ ((x[7] >> 4) & zero_h);
    t[13]= ((x[1] << 12) & zero_l) ^ ((x[5] >> 4) & zero_h);
    t[4] = ((((x[1] << 16) ^ (x[2] >> 16)) << 7) & zero_l) ^ ((aux2 >> 11) & zero_h);
    t[5] = ((x[2] << 4) & zero_l) ^ ((x[6] >> 8) & zero_h);
    t[6] = ((x[2] << 13) & zero_l) ^ ((aux3 >> 15) & zero_h);
    t[7] = ((x[2] << 16) ^ (x[3] >> 16)) << 5;
    t[8] = ((x[3] << 4) & zero_l) ^ ((aux3 >> 1) & zero_h);
    aux4 = (x[3] << 16) ^ (ax7 >> 16);
    aux0 = (x[7] << 16) ^ (ax7 & zero_h);
    t[9] = ((aux4 << 1) & zero_l) ^ (aux0 >> 16);
    t[10]= ((aux4 << 7) & zero_l) ^ ((aux0 >> 11) & zero_h);
    t[1] = ((x[3] << 14) & zero_l) ^ ((x[5] >> 2) & zero_h);
    t[12]= ((aux4 <<15) & zero_l) ^ ((aux0 >> 3) & zero_h);
    
    t[2] = (x[1] & zero_l) ^ ((((x[4] << 16) ^ (x[5] >> 16)) >> 3) & zero_h);
    t[7] = (t[7] & zero_l) ^ ((aux3 >> 11) & zero_h);
    
    t[0] = ((x[0] << 16) ^ (x[4] & zero_h));
    t[0] = t[0] ^ (t[0] << 16);
    t[0] = t[0] ^ t[1];
    t[0] = t[0] ^ t[3];
    t[0] = t[0] ^ t[2] ^ t[4] ^ t[5] ^ t[6] ^ t[7] ^ t[8] ^ t[9] ^ t[10];
}
						
/*Compute the input values of the function h*/						
void static inline first_h(const u32 *x, u32 *h, u32 l){	
    u32 aux1,aux2,aux3;
    h[0] = (x[4] << 8) ^ (x[5] >> 24);
    h[1] = (x[2] << 8) ^ (x[3] >> 24);
    h[2] = (x[1] << 15)^ (x[2] >> 17);
    h[3] = ((x[7] << 15) & zero_l);
    h[4] = ((x[3] << 4) & zero_l);
    
    h[5] = (x[0] << 1) ^ (x[1] >> 31); /*h5*/
    aux1 = ((x[1] ^ x[2]) << 26) ^ ((x[2] ^ x[3]) >> 6);/*h7*/
    aux2 = (x[0] << 7) ^ (x[1] >> 25);/*h6*/
    aux3 = (x[4] << 3) ^ (x[5] >> 29);/*h9*/
    h[5] = h[5] ^ aux1;
    aux1= (x[6] << 16) ^ (x[7] >> 16);/*h10*/
    h[5] = h[5] ^ aux2;
    aux2= (x[6] << 7) ^ (x[7] >> 25);/*h11*/
    h[5] = h[5] ^ aux3;
    aux3= (x[6] << 27) ^ (x[7] >> 5);/*h12*/
    h[5] = h[5] ^ aux1;
    aux1= (x[4] << 18) ^ (x[5] >> 14);/*h13*/
    h[5] = h[5] ^ aux2;
    aux2= (x[5] << 2) ^ (x[6] >> 30);/*h14*/
    h[5] = h[5] ^ aux3;
    aux3= ((x[3] << 9) & zero_l);/*h15*/
    h[5] = h[5] ^ aux1;
    h[5] = h[5] ^ aux2;
    h[5] = h[5] ^ aux3;
    h[6] = h[1] & (h[0] ^ (l & h[2]));
   
}

/*Compute the input values of the function h*/
void static inline second_h(u32 *h, u32 ax7, u32 * x){
    u32 aux1;
    h[3] = ((((x[7]) << 15)) ^ (((ax7 & zero_h) >> 1)));
    aux1 = (x[3] << 20) ^ ((ax7 >> 12));
    h[15] = (aux1 >> 11) & zero_h;
    h[4]= aux1 >> 16;
}
						

u32 static inline first_comp(u32 *t, u32 *h, u32 *rhp, u32 l){						
    u32 aux,rh;
   
    aux = t[0] ^ t[10]& ( t[12] & ~(t[13] & t[4]) ^  t[9] & (t[8] ^ t[7] & (t[6] ^  t[8]&t[12]))) ^ t[5] & (t[2]&t[8]&t[12] ^ t[4] & t[6]&~(t[2]&t[13] ^ t[7]&t[8]&t[9])) ^ t[2]&t[13]  ^ t[6]&t[7];
    *rhp = h[5] ^ l ^ h[2] ^ (h[2] & (l ^ (h[0] & h[1])));
    rh = h[3] ^ (h[4] & ((h[0] | h[1]) ^ h[3])) ^ h[3] &  h[6] ^ (*rhp) ;
    rh = (rh & zero_l) ^(rh >> 16);
  
    return aux ^ rh;
}
						
	
	
u32 static inline second_comp(u32 *t, u32 *h, u32 rhp){
    u32 aux,rh;

    aux = t[0] ^ t[10]& ( t[12] & ~(t[13] & t[4]) ^  t[9] & (t[8] ^ t[7] & (t[6] ^  t[8]&t[12]))) ^ t[5] & (t[2]&t[8]&t[12] ^ t[4] & t[6]&~(t[2]&t[13] ^ t[7]&t[8]&t[9])) ^ t[2]&t[13]  ^ t[6]&t[7] ;
    rh = h[15] ^ h[3] ^ (h[4] & ((h[0] | h[1]) ^ h[3])) ^ h[3] & ( h[6]) ^rhp ;
    rh = (rh & zero_h) ^(rh << 16);
    
    return aux ^ rh;
}
					
					

/*The permutation function P*/					
int permute_p( u32 * x ) {
  /* state of 256=2x128 bits */
  u32 aux1,aux2,aux3,aux4,aux5,aux6,aux7,aux8;
  u32 ah1,rh=0,rhp=0;
  int i,idd;
   
  u32 t[15];
  u32 h[16];
  
  idd=0;
  
#ifdef UNROLED  
  for(i=0;i<64;i++){
  
      
      first_perm(x,t);
      first_h(x,h,l[idd]);
      aux7 = first_comp(t,h,&rhp,l[idd]);
      
      i = i+2;
      idd++;

      second_perm(x,t,aux7);
      second_h(h, aux7, x);
      aux8 = second_comp(t,h,rhp);

      aux2  = (aux7 << 16) ^ (aux8 & zero_h);/*x4*/
      aux1  = (aux7 & zero_l) ^ (aux8 >> 16);/*x0*/

      x[0] = x[1];
      x[1] = x[2];
      x[2] = x[3];
      x[3] = aux1;
      
      x[4] = x[5];
      x[5] = x[6];
      x[6] = x[7];
      x[7] = aux2;
      
      first_perm(x,t);
      first_h(x,h,l[idd]);
      aux7 = first_comp(t,h,&rhp,l[idd]);
      
      i = i+2;
      idd++;
      
      second_perm(x,t,aux7);
      second_h(h, aux7, x);
      aux8 = second_comp(t,h,rhp);
      
      aux2  = (aux7 << 16) ^ (aux8 & zero_h);/*x5*/
      aux1  = (aux7 & zero_l) ^ (aux8 >> 16);/*x1*/
  
      x[0] = x[1];
      x[1] = x[2];
      x[2] = x[3];
      x[3] = aux1;
      
      x[4] = x[5];
      x[5] = x[6];
      x[6] = x[7];
      x[7] = aux2;

      
      first_perm(x,t);
      first_h(x,h,l[idd]);
      aux7 = first_comp(t,h,&rhp,l[idd]);
      
      i = i+2;
      idd++;
      
      second_perm(x,t,aux7);
      second_h(h, aux7, x);
      aux8 = second_comp(t,h,rhp);
      
      aux2  = (aux7 << 16) ^ (aux8 & zero_h);/*x6*/
      aux1  = (aux7 & zero_l) ^ (aux8 >> 16);/*x2*/
            
      x[0] = x[1];
      x[1] = x[2];
      x[2] = x[3];
      x[3] = aux1;
      
      x[4] = x[5];
      x[5] = x[6];
      x[6] = x[7];
      x[7] = aux2;
      
      first_perm(x,t);
      first_h(x,h,l[idd]);
      aux7 = first_comp(t,h,&rhp,l[idd]);
      
      i = i+2;
      idd++;

      second_perm(x,t,aux7);
      second_h(h, aux7, x);
      aux8 = second_comp(t,h,rhp);
      
      aux2  = (aux7 << 16) ^ (aux8 & zero_h);/*x7*/
      aux1  = (aux7 & zero_l) ^ (aux8 >> 16);/*x3*/
      
      x[0] = x[1];
      x[1] = x[2];
      x[2] = x[3];
      x[3] = aux1;
      
      x[4] = x[5];
      x[5] = x[6];
      x[6] = x[7];
      x[7] = aux2;
 
  }
#else
  for(i=0;i<64;i++){
  
      
      first_perm(x,t);
      first_h(x,h,l[idd]);
      aux7 = first_comp(t,h,&rhp,l[idd]);
      
      i++;
      idd++;

      second_perm(x,t,aux7);
      second_h(h, aux7, x);
      aux8 = second_comp(t,h,rhp);

      aux2  = (aux7 << 16) ^ (aux8 & zero_h);/*x4*/
      aux1  = (aux7 & zero_l) ^ (aux8 >> 16);/*x0*/

      x[0] = x[1];
      x[1] = x[2];
      x[2] = x[3];
      x[3] = aux1;
      
      x[4] = x[5];
      x[5] = x[6];
      x[6] = x[7];
      x[7] = aux2;
  }
#endif
  
  return 0;
}



/* permutation of the state */
static void permute(u32 * x) {
permute_p( x );
}


/* initialization of the IV */
int init(hashState *state,u32 *data,int inlen)
{
  int i;
   
	    
  /* initialize state */
  for (i = 0; i < 8; i++){
    state->x[i] = iv2[i];
  }
  
  state->len = inlen;
  state->data = data;
  
  return 0;
}

int update(hashState *state)
{
  int i;
  u32 *u = state->data;
  int len = state->len;
   
  while (len >= 32) {

    /* get next byte */
        

   /* xor state with each bit */
   state->x[7] ^= *u;
    u += 1;
    len-=32;

    permute(state->x);
  }
  
  state->len = len;

  return 0;
}


int final(hashState *state, u32 *out)
{
  u32 temp;
  int i,outbytes=0,j;
  
  temp = *state->data;
  temp = temp >> (31 - state->len); 
  temp |= 0x1;
  temp = temp << (31 - state->len);
    
  state->x[7] ^= temp;

  
  permute(state->x);
  
  out[outbytes++] = state->x[7];
  
    /* extract one byte */
    for(i=0;i<7;i++) {
      permute(state->x);
      out[outbytes++] = state->x[7];
    }
  
  
  
  return 0;
}

int quark(u32 *out, u32 *in, u64 inlen) {
  /* inlen in bytes */

  hashState state;
  init(&state,in,inlen);
  update(&state);
  final(&state, out);
  
  return 0;
}


