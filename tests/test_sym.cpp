#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

   vector<BladeS> bx;
   vector<BladeS> bt;
   vector<BladeS> br;
   vector<BladeS> bw;
   char syms[] = {'1','2','3'};
   char buff[32];
   
   br.push_back(BladeS(Sym("r_0")));
   
   int k=0;
   for(unsigned int i=0;i<3;i++) {
       sprintf(buff,"x_%c", syms[i]);
       bx.push_back(BladeS(Sym(buff),i+1));       
       sprintf(buff,"w_%c", syms[i]);
       bw.push_back(BladeS(Sym(buff),i+1));       
       sprintf(buff,"t_%c", syms[i]);
       bt.push_back(BladeS(Sym(buff),i+1));       
       for(unsigned int j=i+1;j<3;j++) {
           vector<unsigned long> e;
           e.push_back(i+1);
           e.push_back(j+1);
           sprintf(buff,"r_%c", syms[k++]);
           br.push_back(BladeS(Sym(buff),e));
       }
   }
   
   MvecS  x(bx);
   MvecS  t(bt);
   MvecS  w(bw);
   MvecS  I=MvecS::I(3);
   MvecS  R(br);
   
   cout << "x = " << x << endl;
   cout << "w = " << w << endl;
   cout << "t = " << t << endl;
   cout << "I = " << I << endl;
   cout << "R = " << R << endl << endl;

   
   MvecS result = R&x;
   cout << "R&x= " << result << endl;

   result = R^x;
   cout << "R^x=" << result << endl;

   result = R*x;
   cout << "R*x=" << result << endl;

   result = (R^x)+(R&x);
   cout << "R*x=" << result << endl;

   result = R*x*~R;
   cout << "R*x*~R=" << result << endl;

   result = (R*((x^t)*(-I))*~R)&w;
   
   cout << "R(x^t)(-I)~R&w=" << result << endl;

   return 0;
}
