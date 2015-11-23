#ifndef RC4_H_
#define RC4_H_

#include <stdint.h>
using namespace std;
class RC4
{
    public :
        RC4();
        uint8_t encryptByte(uint8_t in);
        uint8_t decryptByte(uint8_t in);
        void genKey(uint8_t keyLength);
    private :          
        uint8_t s[256];
        uint8_t i,j;
 		uint8_t * key;
 		uint8_t  keyLength;
};
 
#endif
