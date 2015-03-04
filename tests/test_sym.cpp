#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

   vector<BladeS> bx;
   vector<BladeS> bt;
   char buff[32];
   
   for(unsigned int i=0;i<3;i++) {
       sprintf(buff,"x%d", i+1);
       bx.push_back(BladeS(Sym(buff),i+1));       
       sprintf(buff,"t%d", i+1);
       bt.push_back(BladeS(Sym(buff),i+1));       
   }
   
    
   MvecS  x(bx);
   MvecS  t(bt);
   MvecS  I=MvecS::I(3);
   
   cout << "x = " << x << endl;
   cout << "t = " << t << endl;
   cout << "I = " << I << endl;

   MvecS  z = x*t;
   cout << "xt = " << z << endl;
//   cout << "b1+b2=" << z << endl;
   
   return 0;
}
