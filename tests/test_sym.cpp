#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

   Sym x1("x1");
   Sym x3("x3");
   
   BladeS b1(x1,1);
   BladeS b3(x3,3);
   BladeS b2(x1+x3,2);
   
   BladeS c = b2^b3;
   
   cout << b1 << " ^ " << b2 << " = " << c << endl;
//   cout << "b1+b2=" << z << endl;
   
   return 0;
}
