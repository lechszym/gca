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
   MvecS result;
   
   cout << "x = " << x << endl;
   cout << "w = " << w << endl;
   cout << "t = " << t << endl;
   cout << "I = " << I << endl;
   cout << "R = " << R << endl;
   //cout << "~R = " << ~R << endl << endl;

   
   result = R&(~R);
   cout << "\nR&~R=" << result << endl;

   result = R^(~R);
   cout << "\nR^~R=" << result << endl;
   
   
   result = R*(~R);
   cout << "\nR*~R=" << result << endl;
   
   
   result = R&x;
   cout << "R&x= " << result << endl;

   
   result = R^x;
   cout << "R^x=" << result << endl;

   result = R*x;
   cout << "\nR*x=" << result << endl;

   result = (R^x)+(R&x);
   cout << "\nR*x=" << result << endl;
   
   result = R*x*(~R);
   cout << "\nR*x*~R=" << result << endl;

   result = x^t;
   cout << "\nx^t=" << result << endl;

   result = result*(-I);
   cout << "\n(x^t)(-I)=" << result << endl;

   result = R*result;
   cout << "\nR(x^t)=" << result << endl;

   result = result&(~R);
   cout << "\nR(x^t)(-I)&~R=" << result << endl;
   
   result = result&w;
   
   cout << "\n(R(x^t)(-I)~R)&w=" << result << endl;

   
   BladeS   be1(1,1);
   BladeS   be2(1,2);
   BladeS   be3(1,3);
   
   
   cout << "\n\nMultiplying:\n";
   
   MvecS R1 = (R*be1)*~R;
   MvecS R2 = (R*be2)*~R;
   MvecS R3 = (R*be3)*~R;


   cout << "R1=" << R1 << endl;
   cout << "R2=" << R2 << endl;
   cout << "R3=" << R3 << endl;
   
   
   
   return 0;
}
