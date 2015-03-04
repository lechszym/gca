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
         _v = 0.0;
         _inv = false;
      }

      Sym(const char s[]) {
         _v = 1.0;
         _sym = std::string(s);
         _inv = false;
      }
      
      Sym(const Sym& orig) {
         _v = orig._v;
         _sym = orig._sym;
         _inv = orig._inv;
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
         m._v *= x;
         return m;
      }

      int getSign() const {
          int sign = 1;
          if(_v < 0) {
              sign *= -1;
          }
          for(std::size_t i=0;i<_mult.size();i++) {
              sign *= _mult[i].getSign();
          }
          return sign;
      }
      
      std::string toString() const {
        std::stringstream ss;

        ss << _sym;
        
        return ss.str();
        
      }
      
      friend std::ostream& operator<<(std::ostream &out, const Sym &s) {      
         out << s.toString();
         return out;      
      }
   
   private:
      std::string _sym;
      double      _v;
      bool        _inv;
      std::vector<Sym> _mult;
      std::vector<Sym> _sum;
   };
   
}


#endif	/* SYM_H */

