# include <iostream>
# include <stdio.h>
# include <fstream>
# include <string.h>
# include <stdlib.h>

# include "DES.h"

using namespace std;

int main()
{
    Des d1;
    char *str  = new char[1000];
    char *str1 = new char[1000];
    //strcpy(str,"PHOENIX it & ece solutions.");
    cout<<"Enter a string : ";
    cin >> str;
    str1=d1.Encrypt(str);
    cout<<"\ni/p Text: "<<str<<endl;
    cout<<"\nCypher  : "<<str1<<endl;
    //  ofstream fout("out2_fil.txt"); fout<<str1; fout.close();
    cout<<"\no/p Text: "<<d1.Decrypt(str1)<<endl;
    return 0;
}
