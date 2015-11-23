#include <cstdlib>
#include <iostream>
#include <string>
#include <stdint.h>
#include "rc4.h"
using namespace std;
int main()
{
    uint8_t buffer[] = {5,7,3,4,1,9,3};
    uint8_t key_length = 7;
    RC4 rc(buffer,key_length);

    uint8_t in = 99;
    cout << "value is " << unsigned(in) << endl;
    uint8_t out = rc.encryptByte(in);
    cout << "value is " << unsigned(out) << endl;
    uint8_t final = rc.decryptByte(out);
    cout << "value is " << unsigned(final) << endl;
    return 0;
}