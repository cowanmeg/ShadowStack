#ifndef RC4_H_
#define RC4_H_

#include <stdint.h>
using namespace std;
class RC4
{
    public :
        RC4();
        uint8_t encryptByte(uint8_t in);
	uint64_t encrypt64(uint64_t in);
        uint8_t decryptByte(uint8_t in);
	uint64_t decrypt64(uint64_t in);
        void genKey();
    private :          
        uint8_t s[256];
        uint8_t i,j;
        uint8_t  key[8];
 	const uint8_t  keyLength = 8;
};
 
#endif
