#include <Mvec>
#include <iostream>
#include <cstring>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {
    
   Blade<> a("2 e1^e2");
   
   Blade<> b("4 e2");
   
   Mvec<> r = a*b;
   cout << a << " & " << b << " = " << (a&b) << endl;
   cout << a << " ^ " << b << " = " << (a^b) << endl;
   cout << a << " * " << b << " = " << (r) << endl;
   
   Blade<> c = a&b;
   cout << "c= " << c << endl;
   cout << "cinv=" << c.inv() << endl;
   
   cout << "c & cinv = " << (c&c.inv()) << endl;
   cout << "c ^ cinv = " << (c^c.inv()) << endl;
   
   return 0;
}
