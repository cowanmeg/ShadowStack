#include "rc4.h"
#include <cstdlib>
#include <stdint.h>
#include <string>
#include <iostream>

using namespace std;
 
RC4::RC4():
s(),
i(0),
j(0)
{   
    printf("rc4 called\n");
    genKey();
}
uint8_t RC4::encryptByte(uint8_t in)
{   
    // initialize
    for(int k = 0; k <= 255; ++k){
        s[k] = k;
    }
       
    uint8_t l = 0;
    for(int k = 0; k <= 255; ++k)
    {
        l = (l + s[k] + key[k % keyLength]) % 256;
        uint8_t tmp = s[k];
        s[k] = s[l];
        s[l] = tmp;
    } 

    i = 0;
    j = 0;

    i = (i + 1) %256;
    j = (j + s[i])%256;

    uint8_t tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
    uint8_t c = s[(s[i]+s[j])%256];

    return in^c;  
}
 
uint8_t RC4::decryptByte(uint8_t in)
{
    return encryptByte(in);
}

uint64_t RC4::encrypt64(uint64_t in){
   uint64_t temp = in;
   uint64_t res = 0;
   uint8_t t =0xff;
   uint8_t total =0;
   for(int i = 0; i < keyLength ;i++){
     total+= key[i];
   } 
   for(int i =0; i < 8; i++){
        uint8_t p = temp & t;
	uint8_t q = encryptByte(p);
        res |= q;
       // printf("En/De %lx : step %d, before en/de = %d, after en/de = %d, key during en/de = %d, keylength during en/de = %d\n", in, i, p, q,total,keyLength);
        if(i<7){
           temp >>= 8;
           res  <<= 8;
        }
    }
   return res;
}
//uint64_t RC4::decrypt64(uint64_t &t){
  // return encrypt64(t);    
//}

void RC4::genKey(){ 
    for(int i = 0; i< keyLength ; i++){
        key[i]  = rand()%256;
    }
}
