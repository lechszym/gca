#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

   vector<BladeS> bx;
   vector<BladeS> bt;
   vector<BladeS> br;
   char syms[] = {'1','2','3'};
   char buff[32];
   
   br.push_back(BladeS(Sym("r_0")));
   
   for(unsigned int i=0;i<3;i++) {
       sprintf(buff,"x_%c", syms[i]);
       bx.push_back(BladeS(Sym(buff),i+1));       
       sprintf(buff,"t_%c", syms[i]);
       bt.push_back(BladeS(Sym(buff),i+1));       
       for(unsigned int j=i+1;j<3;j++) {
           vector<unsigned long> e;
           e.push_back(i+1);
           e.push_back(j+1);
           sprintf(buff,"r_%c%c", syms[i], syms[j]);
           br.push_back(BladeS(Sym(buff),e));
       }
   }
   
   //Sym x2 = Sym("x")+Sym("x");
   cout << "x-x=" << (Sym("x")-Sym("x")) << endl;
   cout << "2x-x=" << (2*Sym("x")-Sym("x")) << endl;
   //cout << "x-2x=" << (Sym("x")-x) << endl;
    
   MvecS  x(bx);
   MvecS  t(bt);
   MvecS  I=MvecS::I(3);
   MvecS  R(br);
   
   cout << "x = " << x << endl;
   cout << "t = " << t << endl;
   cout << "I = " << I << endl;
   cout << "R = " << R << endl;

   MvecS za = R*x;
   MvecS zza = za&~R;
   MvecS zzb = za^~R;
   MvecS zb = za*~R;
   cout << "Rx= " << za << endl;
   cout << "~R=" << ~R << endl;
   cout << "Rx&~R= " << zza << endl;
   cout << "Rx^~R= " << zzb << endl;
   cout << "Rx~R= " << zb << endl;
   
   MvecS  z = (x^t);
   cout << "x^t = " << z << endl;
   
   MvecS w = z*(-I);
   cout << "(x^t)I=" << w << endl;
   
   MvecS g1 = R*w;
   MvecS g2 = g1*(~R);
   
   cout << "R(x^t)I~R=" << g2 << endl;
   
   return 0;
}
