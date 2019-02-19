#include "aes.h"
#include <stdio.h>
#include <string.h>

#define TARLEN 9
unsigned char sbox[16][16] = {\
			      { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 } ,\
			      { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 } ,\
			      { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 } ,\
			      { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 } ,
			      { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 } ,
			      { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf } ,
			      { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 } ,
			      { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 } ,
			      { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 } ,
			      { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb } ,
			      { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 } ,
			      { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 } ,
			      { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a } ,
			      { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e } ,
			      { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf } ,
			      { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};

uint32_t sub[4] =    { 0xd4e0b81e,
		       0x27bfb441,
		       0x11985d52,
		       0xaef1e530 };

uint32_t shift[4] =  { 0xd4e0b81e,
		       0xbfb44127,
		       0x5d521198,
		       0x30aef1e5 };

uint32_t mix[4] =    { 0x04e04828,
		       0x66cbf806,
		       0x8119d326,
		       0xe59a7a4c };

uint32_t rnd[4] =   { 0xa4686b02,
			0x9c9f5b6a,
			0x7f35ea50,
			0xf22b4349 };

/*
 * Tests return 1 if successful, otherwise 0.
 * if vflag evaluates as true, the test is verbose.
 * TODO use an actual unit testing framework.
 */

int ffAdd_test(int vflag)
{
  unsigned char ret = ffAdd(0x57,0x83);
  if(vflag)
    printf("ffAdd_test: ffAdd(0x57,0x83)=%x\n", ret);
  return ret == 0xd4;
}


int xtime_test(int vflag)
{
  unsigned char xt1 = xtime(0x57); // expect 0xae
  unsigned char xt2 = xtime(0xae); // expect 0x47
  unsigned char xt3 = xtime(0x47); // expect 0x83
  unsigned char xt4 = xtime(0x8e); // expect 0x07

  int tst1 = xt1 == 0xae;
  int tst2 = xt2 == 0x47;
  int tst3 = xt3 == 0x8e;
  int tst4 = xt4 == 0x07;
  
  if(vflag)
    printf("%x %x %x %x\n", xt1, xt2, xt3, xt4);
  return tst1 && tst2 && tst3 && tst4;
}

int ffMultiply_test(int vflag)
{
  unsigned char ff1 = ffMultiply(0x57,0x13);
  unsigned char ff2 = ffMultiply(0x57,0x10);
  unsigned char ff3 = ffMultiply(0x57,0x02);
  unsigned char ff4 = ffMultiply(0x57,0x01);
  unsigned char ff5 = ffMultiply(0x57,0x00);
  unsigned char ff6 = ffMultiply(0x2,0xff);
  unsigned char ff7 = ffMultiply(0xff,0x02);
  unsigned char ff8 = ffMultiply(0x12,0x03);
  unsigned char ff9 = ffMultiply(0x03,0xff);

  if(vflag){
    printf("ffMultiply_test:\n 0x57,0x13=%x\n", ff1);
    printf("0x57,0x10=%x\n", ff2);
    printf("0x57,0x02=%x\n", ff3);
    printf("0x57,0x01=%x\n", ff4);
    printf("0x57,0x00=%x\n", ff5);
    printf("0x02,0xff=%x\n", ff6);
    printf("0xff,0x02=%x\n", ff7);
    printf("0x12,0x03=%x\n", ff8);
    printf("0x03,0xff=%x\n", ff9);
  }

  int t1 = ff1 == 0xfe;
  int t2 = ff2 == 0x07;
  int t3 = ff3 == 0xae;
  int t4 = ff4 == 0x57;
  int t5 = ff5 == 0x00;
  int t6 = ff6 == 0xe5;
  int t7 = ff7 == 0xe5;
  int t8 = ff8 == 0x36;
  int t9 = ff9 == 0x1a;
  
  int tarr[TARLEN] = {t1, t2, t3, t4, t5, t6, t7, t8, t9};
  for(int i = 0; i < TARLEN; i++){
    if(!tarr[i])
      return 0;
  }
  return 1;
}

int itow_test(int vflag)
{
  int val = 0x12345678;
  word w = itow(val);
  if(w.b[0]==0x12 && w.b[1]==0x34 && w.b[2]==0x56 && w.b[3]==0x78)
    return 1;
  else
    return 0;
}

int wtoi_test(int vflag)
{
  word w;
  w.b[0] = 0x12;
  w.b[1] = 0x34;
  w.b[2] = 0x56;
  w.b[3] = 0x78;
  uint32_t i = wtoi(w);
  if( i == 0x12345678 )
    return 1;
  else
    return 0;
}

int sw_test(int vflag)
{
  word w;
  word new;
  
  memset(w.b,0,4);
  uint32_t i1 = 0x00ff0ff0;
  
  new = sub_w(itow(i1));
  if(vflag){
    printf("sw before: %x\n", i1);
    printf("sw after: %x\n", wtoi(new));
  }

  if( new.b[0]==sbox[0][0] &&
      new.b[1]==sbox[15][15] &&
      new.b[2]==sbox[0][15] &&
      new.b[3]==sbox[15][0] ){
    return 1;
  }else{
    return 0;
  }
}

int key_exp_test(int vflag)
{
  uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		      0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

  uint32_t expanded[44] = { 0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c,
			    0xa0fafe17, 0x88542cb1, 0x23a33939, 0x2a6c7605,
			    0xf2c295f2, 0x7a96b943, 0x5935807a, 0x7359f67f,
			    0x3d80477d, 0x4716fe3e, 0x1e237e44, 0x6d7a883b,
			    0xef44a541, 0xa8525b7f, 0xb671253b, 0xdb0bad00,
			    0xd4d1c6f8, 0x7c839d87, 0xcaf2b8bc, 0x11f915bc,
			    0x6d88a37a, 0x110b3efd, 0xdbf98641, 0xca0093fd,
			    0x4e54f70e, 0x5f5fc9f3, 0x84a64fb2, 0x4ea6dc4f,
			    0xead27321, 0xb58dbad2, 0x312bf560, 0x7f8d292f,
			    0xac7766f3, 0x19fadc21, 0x28d12941, 0x575c006e,
			    0xd014f9a8, 0xc9ee2589, 0xe13f0cc8, 0xb6630ca6 };

  uint32_t w[44];
  key_exp(key,w,4);
  for(int i = 0; i < 44; i++){
    if( w[i] != expanded[i] )
      return 0;
  }
  return 1;
}

int rot_w_test(int vflag)
{
  word t1 = rot_w(itow(0x09cf4f3c));
  word t2 = rot_w(itow(0x2a6c7605));
  if( wtoi(t1) != 0xcf4f3c09 )
    return 0;
  if( wtoi(t2) != 0x6c76052a )
    return 0;
  return 1;
}

int sub_bytes_test(word state[Nb])
{
  sub_bytes(state);
  for(int i = 0; i < Nb; i++){
    if(wtoi(state[i]) != sub[i]){
      return 0;
    }
  }
  return 1;
}

int shift_rows_test(word state[Nb])
{
  shift_rows(state);
  for(int i = 0; i < Nb; i++){
    if(wtoi(state[i]) != shift[i]){
      return 0;
    }
  }
  return 1;
}

int mix_cols_test(word state[Nb])
{
  mix_cols(state);
  for(int i = 0; i < Nb; i++){
    if(wtoi(state[i]) != mix[i]){
      return 0;
    }
  }
  return 1;  
}
int add_key_test(word state[Nb])
{
  add_key(state);
  for(int i = 0; i < Nb; i++){
    if(wtoi(state[i]) != rnd[i]){
      return 0;
    }
  }
  return 1;  
}

int cipher_test(int vflag)
{
  word state[Nb];
  int i, sub_bool, shift_bool, mix_bool, rnd_bool;

  /* initialize state */
  state[0] = itow(0x19a09ae9);
  state[1] = itow(0x3df4c6f8);
  state[2] = itow(0xe3e28d48);
  state[3] = itow(0xbe2b2a08);

  sub_bool = sub_bytes_test(state);
  printf("sub_bytes_test: %d\n-------\n", sub_bool);

  shift_bool = shift_rows_test(state);
  printf("shift_rows_test: %d\n-------\n", shift_bool);

  mix_bool = mix_cols_test(state);
  printf("mix_cols_test: %d\n-------\n", mix_bool);

  rnd_bool = add_key_test(state);
  printf("add_key_test: %d\n-------\n", rnd_bool);
  
}

int main(int argc, char** argv)
{
  int vflag;
  if(argc > 1)
    vflag = strcmp(argv[1],"-v")==0;
  printf("ffAdd_test: %d\n-------\n", ffAdd_test(vflag));
  printf("xtime_test: %d\n-------\n", xtime_test(vflag));
  printf("ffMultiply_test: %d\n-------\n", ffMultiply_test(vflag));
  printf("itow_test: %d\n-------\n",   itow_test(vflag));
  printf("wtoi_test: %d\n-------\n",   wtoi_test(vflag));
  printf("sub_word_test: %d\n-------\n",   sw_test(vflag));
  printf("key_exp_test: %d\n-------\n",   key_exp_test(vflag));
  printf("rot_w_test: %d\n-------\n",   rot_w_test(vflag));
  cipher_test(vflag);

  
  return 0;
}
