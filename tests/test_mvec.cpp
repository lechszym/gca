#include <Mvec>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

#ifdef EIGEN_ENABLED    
   Eigen::RowVectorXd n(2);
   n << 1, 1;
   Mvecd m(n);
   
   cout << "m = " << m << endl;
   
   Mvecd m2 = m*m;
   cout << "m*m = " << m2 << endl;


   Eigen::RowVectorXd ee1(2);
   Eigen::RowVectorXd ee2(2);
   ee1 << 1, 0;
   ee2 << 0, 1;

   Mvecd e1(ee1);
   Mvecd e2(ee2);
   
   cout << "e1 = " << e1 << endl;
   cout << "e2 = " << e1 << endl;
   
   Mvecd e1we2 = e1^e2;
   Mvecd eww = e1we2*e1we2;
   cout << "e1^e2 = " << e1we2 << endl;
   cout << "e1^e2*e1^e2 = " << eww << endl;
   

   

   Eigen::RowVectorXd v(4);
   
   v << 1, 2, 3, 4;  

   Mvecd mv(v);
   
   cout << mv << endl;
   
   Mvecd mv2 = mv*mv;
   
   cout << mv2 << endl;
   
   mv2 = mv2/mv;
   cout << mv2 << endl;
#endif   
   //Mvecd v1("4+2 e1+4  e4^e2^e1-2 e3^e1");
    //Mvecd v1("1.2 e1^e2 + 5 e2^e3");
    
    //Mvecd v1("1862 e1^e2^e3^e4^e6 +1741 e1^e2^e3^e4^e5 +1187 e1^e2^e3^e5^e6 +2080 e1^e2^e4^e5^e6 -359 e1^e3^e4^e5^e6 +836 e2^e3^e4^e5^e6");
    //Mvecd v2("4 e1 +2 e2 +1 e3 +2 e4 -4 e5 +5 e6");
    //Mvecd v2("3.2 e1+4.4 e2-5 e4");
    //Mvecd v2("4.4 e2^e3");
    //Mvecd v3;
    
    //cout << "v1=" << v1 << endl;
    //cout << "v2=" << v2 << endl;
    //v3 = v1^v2;
    //cout << "v3=" << v3 << endl;

    return 0;
    
    /*Mvecd v4=~v1;
    cout << "~v1=" << v4 << endl;
    Mvecd v5=v1&v1;
    cout << "v1&v1=" << v5 << endl;
    Mvecd v6 = v1/v1;
    cout << "v1/v1=" << v6 << endl;
    Mvecd v7 = v6*v1;
    cout << "v1/v1*v1=" << v7 << endl;
    
    v6 = v1/v2;
    cout << "v1/v2=" << v6 << endl;
    v7 = v6*v2;
    cout << "v1/v2*v2=" << v7 << endl;
    
    
    Mvecd x1("3 e1+4 e2 +5 e3");
    Mvecd x2("-1 e1 +2 e2 +4 e3");
    
    Mvecd a=x1&x2;
    Mvecd b=x1^x2;
    
    cout << "x1=" << x1 << endl;
    cout << "x2=" << x2 << endl;
    cout << "x1&x2=" << a << endl;
    cout << "x1^x2=" << b << endl;
    Mvecd d=b*b;
    cout << "x1^x2*x1^x2=" << d << endl;

    Mvecd c=a*a-b*b;
    cout << "Result=" << c << endl;
    
    
    return 0;*/
}
