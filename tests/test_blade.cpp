#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

   Bladed b1(2,1);
   
   Bladed b2(3,2);
   Bladed b3(-1,3);
   
   Bladed c1 = b1&b3;

   cout << b1 << " & " << b2 << " = " << c1 << endl;
   
   c1 = b3^b1;
   
   cout << "c1 = " << b3 << " ^ " << b1 << " = " << c1 << endl;
   
   Bladed d=c1.inv();
   cout << "c1 inv = " << d << endl;
   
   cout << "c1 & c1inv = " << (c1&d) << endl;
   cout << "c1 ^ c1inv = " << (c1^d) << endl;
   
   Bladed c3 = c1&b2;
   
   cout << c3 << endl;
    //Bladed a1("-1 e1");
    //Bladed a2("-2 e1");

    
    
    //Bladed a2("3 e1^e2");
    //Bladed a3;
    
    //cout << "a1=" << a1 << endl;
    //cout << "a2=" << a2 << endl;
    //Bladed a3;
    //a3 = a1&~a2;
    //cout << "a1&~a2=" << a3 << endl;
    //a3 = a2^a1;
    //cout << "a2^a1=" << a3 << endl;
    //cout << "a2=" << a2 << endl;
    //cout << "grade(a2)=" << a2.grade() << endl;
    //cout << "a1&a1=" << a3 << endl;
    //a3 = a1&a2;
    //cout << "a1&a2=" << a3 << endl;
    //a3 = a2&a1;
    
#if 0    
    
    Bladed b1("2 e4^e2");
    Bladed b2("3 e1^e3");
    Bladed b3 = b1;
    b3.wedge(b2);
    cout << "b1=" << b1 << endl;
    cout << "b2=" << b2 << endl;
    cout << "b3=b1^b2=" << b3 << endl;
    cout << "b3 blade has grade:" << b3.grade() << endl;
    
    
    Bladed c1("3 e1^e2");
    Bladed c2 = ~c1;
    cout << "c1=" << c1 << endl;
    cout << "c2=" << c2 << endl;
    //cout << "c1*~c1=" << c1*c2 << endl;
    
    
    Bladed d1 = a1;
    d1.inv();
    
    cout << "d1=" << d1 << endl;
#endif
    return 0;
}
