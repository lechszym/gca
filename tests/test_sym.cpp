#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {

   Sym x1("x1");
   Sym x3("x3");
   
   BladeS b1(x1,1);
   BladeS b3(x3,3);
   
   cout << b1 << endl;
   cout << b3 << endl;
   
   return 0;
}
