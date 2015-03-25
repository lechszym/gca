#include <Mvec>
#include <iostream>

using namespace std;
using namespace gca;

int main(int argc, char **argv) {
   
   Mvec<double> R = Rotor(0,36,0,false);
   
   cout << "R=" << R << "\n";
   
   return 0;
}
