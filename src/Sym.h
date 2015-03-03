/* 
 * File:   Sym.h
 * Author: lechszym
 *
 * Created on 3 March 2015, 9:47 PM
 */

#ifndef SYM_H
#define	SYM_H

#include <string>

namespace gca {
 
   class Sym {
   public:

      Sym() {
         _sign = 1;
      }

      Sym(const char s[]) {
         _sign = 1;
         _sym = std::string(s);
      }
      
      Sym(const Sym& orig) {
         _sign = orig._sign;
         _sym = orig._sym;
      }

      virtual ~Sym() {

      }

      /*std::string toString() const {
         std::stringstream ss;

         ss << _sym.c_str();
         
         return ss.str();
      }*/
      
      Sym operator*(const Sym& m) const {
         return Sym(m);
      }

      Sym operator*(const int x) const {
         Sym m = Sym(*this);
         m._sign *= x;
         return m;
      }
      
      friend std::ostream& operator<<(std::ostream &out, const Sym &s) {      
         out << s._sym;
         return out;      
      }
   
   private:
      std::string _sym;
      int         _sign;
      
   };
   
}


#endif	/* SYM_H */

