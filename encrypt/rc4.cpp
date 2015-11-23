#include "rc4.h"
#include <cstdlib>
#include <stdint.h>
#include <string>
#include <iostream>

using namespace std;
 
RC4::RC4(uint8_t *key, uint8_t keyLength):
s(),
i(0),
j(0)
{   
    this->key = key;
    this->keyLength = keyLength;
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