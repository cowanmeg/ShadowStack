#include <cstdlib>
#include <iostream>
#include <string>
#include <stdint.h>
#include "rc4.h"
using namespace std;
int main()
{
    // uint8_t buffer[] = {5,7,3,4,1,9,3};
    // uint8_t key_length = 7;
    
    RC4 rc;
   // uint8_t in = 99;
   // cout << "value is " << in << endl;
   // uint8_t out = rc.encryptByte(in);
    //cout << "value is " << unsigned(out) << endl;
    //uint8_t finalz = rc.decryptByte(out);
   // cout << "value is " << finalz << endl;
    
    uint64_t temp = 4831839168;
    std::cout<<" before: "<< temp<<endl;
    uint64_t res = 0; 
    uint8_t t =0xff;	
    for(int i =0; i < 8; i++){
	uint8_t p = temp & t;
        cout << "befor encrypted last four bytes: " << unsigned(p) << endl;
	uint8_t q = rc.encryptByte(p);
	cout << "after encrypted last four bytes: " << unsigned(q) << endl;
	res |= q;
        cout << "after encrypted res: " << res << endl;
        if(i<7){
		temp >>= 8;
		res  <<= 8;
	}
   }
    cout<<endl;
    cout<< "res: " << res<<endl;
    cout<<endl;
    uint64_t dres = 0;
    for(int i =0; i < 8 ; i++){
	uint8_t p = res & t;
        cout << "befor decrypted last four bytes: " << unsigned(p) << endl;
	uint8_t q = rc.decryptByte(p);
	cout << "after decrypted last four bytes: " << unsigned(q) << endl;
	dres |= q;
        cout << "after decrypted res: " << dres << endl;
	if(i<7){
		res >>=8;
		dres <<=8;
	}	
    }
    cout << "finial: " <<dres <<endl;
        uint64_t tm = 4831839168;
	std::cout<<" before: "<< tm<<endl;
	uint64_t r =  rc.encrypt64(tm);
        cout<<endl;
        cout<< "r: " << r<<endl;
        cout<<endl;
	uint64_t d = rc.encrypt64(r);
	cout << "finial: " <<d <<endl;
    
return 0;
}
