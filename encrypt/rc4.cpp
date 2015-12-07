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
    uint8_t l =7;
    genKey(l);
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

uint64_t RC4::encrypt64(uint64_t &in){
   uint64_t temp = in;
   uint64_t res = 0;
   uint8_t t =0xff;

    for(int i =0; i < 8; i++){
        uint8_t p = temp & t;
        cout << "befor last four bytes: " << unsigned(p) << endl;
	uint8_t q = encryptByte(p);
	cout << "after last four bytes: " << unsigned(q) << endl;
        res |= q;
	cout << "after decrypted res: " << res << endl;
        if(i<7){
           temp >>= 8;
           res  <<= 8;
        }
    }
   return res;
}
uint64_t RC4::decrypt64(uint64_t &t){
   return encrypt64(t);    
}

void RC4::genKey(uint8_t tkeyLength){
    
    uint8_t tkey[tkeyLength];
    for(int i =0; i<tkeyLength ; i++){

        tkey[i] = tkey[i] = rand()%256;
    }
    this->keyLength = tkeyLength;
    this->key = tkey;
}
