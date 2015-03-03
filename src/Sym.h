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
         
      }

      Sym(const char s[]) {
         _sym = std::string(s);
      }
      
      Sym(const Sym& orig) {
         _sym = orig._sym;
      }

      virtual ~Sym() {

      }

      /*std::string toString() const {
         std::stringstream ss;

         ss << _sym.c_str();
         
         return ss.str();
      }*/
      
      friend std::ostream& operator<<(std::ostream &out, const Sym &s) {      
         out << s._sym;
         return out;      
      }
   
   private:
      std::string _sym;
      
   };
   
}


#endif	/* SYM_H */

