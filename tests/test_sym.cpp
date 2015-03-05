#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

   vector<BladeS> bx;
   vector<BladeS> bt;
   vector<BladeS> br;
   char buff[32];
   
   br.push_back(BladeS(Sym("r0")));
   
   for(unsigned int i=0;i<3;i++) {
       sprintf(buff,"x%d", i+1);
       bx.push_back(BladeS(Sym(buff),i+1));       
       sprintf(buff,"t%d", i+1);
       bt.push_back(BladeS(Sym(buff),i+1));       
       for(unsigned int j=i+1;j<3;j++) {
           vector<unsigned long> e;
           e.push_back(i+1);
           e.push_back(j+1);
           sprintf(buff,"r%d%d", i+1, j+1);
           br.push_back(BladeS(Sym(buff),e));
       }
   }
   
    
   MvecS  x(bx);
   MvecS  t(bt);
   MvecS  I=MvecS::I(3);
   MvecS  R(br);
   
   cout << "x = " << x << endl;
   cout << "t = " << t << endl;
   cout << "I = " << I << endl;
   cout << "R = " << R << endl;
   cout << "~R=" << ~R << endl;
   
   MvecS  z = (x^t);
   cout << "x^t = " << z << endl;
   
   MvecS w = z*(-I);
   cout << "(x^t)I=" << w << endl;
   
   MvecS g1 = R*w;
   MvecS g2 = g1*(~R);
   
   cout << "R(x^t)I~R=" << g2 << endl;
   
   return 0;
}
