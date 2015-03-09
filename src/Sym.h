/* 
 * File:   Sym.h
 * Author: lechszym
 *
 * Created on 3 March 2015, 9:47 PM
 */

#ifndef SYM_H
#define	SYM_H

#include <string>
#include <algorithm>

namespace gca {
  
   typedef struct sym_s {
      int pow;
      std::string s;
   } sym_t;
   
   
   class Mult {
   public:

      Mult() {
         _v = 0.0;
      }

      Mult(const double v) {
          _v = v;
      }
      
      Mult(const char s[])  {
         _v = 1.0;
         sym_t sym;
         sym.pow = 1.0;
         sym.s = s;
         _mult.push_back(sym);
      }
      
      Mult(const Mult& orig) {
         _v = orig._v;
         _mult = orig._mult;
      }

      virtual ~Mult() {

      }

      Mult operator*(const Mult& m) const {
       
         
         if(m._mult.empty()) {
             return (*this) * m._v;
         } 
          
         if(m._mult.empty()) {
             return m * _v;
         }
         
         
         Mult result(*this);
         
         result._v *= m._v;

         for(std::size_t i=0;i<m._mult.size();i++) {
             std::vector<sym_t>::iterator si = result._mult.begin();
    
             bool multiplied = false;
             while(si != result._mult.end()) {
                if(si->s == m._mult[i].s) {
                    si->pow += m._mult[i].pow;

                    if(si->pow == 0) {
                        si = result._mult.erase(si);
                    }
                    multiplied = true;
                    break;
                 }
                 si++;
             }         
             if(multiplied) {
                 continue;
             }

             result._mult.push_back(m._mult[i]);
         }
         
         std::sort(result._mult.begin(), result._mult.end(), Mult::compBySym);         
         
         
         return result;
      }

      /*bool operator==(const Sym& m) const {
         if(_mult.size() != m._mult.size()) {
            return false;
         }
         
         for(std::size_t i=0;i<_mult.size();i++) {
            if( (_mult[i]._sym != b[i]._sym) ||
                (a[i]._pow != b[i]._pow)) {
               return false;
            }
         }
         
         
         return true;

         return false;
      }*/
      
      Mult operator*(const double x) const {
         Mult result = Mult(*this);
         result._v *= x;
         return result;
      }
      
      Mult operator*=(const double x) const {
         Mult result = Mult(*this);
         result._v *= x;
         return result;
      }

      
      bool operator>=(double v) const {
         return _v >= v;
      }

      bool operator<(double v) const {
         return _v < v;
      }
      
      bool operator>(double v) const {
         return _v > v;
      }      
            
      bool operator==(double v) const {
         if(_mult.empty() && _v==v) {
            return true;
         } else {
            return false;
         }
      }
      
      bool operator==(const Mult &m) const {
          return equal(_mult,m._mult);
      }
      
      std::string toString() const {
        std::stringstream ss;

        if(_v == 0.0) {
           ss << "0";
           return ss.str();
        }
        
        if(_v != 1.0) { 
           if(_v == -1.0) {
              ss << "-";
           } else {
             ss << _v;
           }
        }     
        
        for(std::size_t i=0;i<_mult.size();i++) {
           ss << _mult[i].s;
           if(_mult[i].pow != 1) {
              ss << "^" << _mult[i].pow;
           }
        } 
        
        return ss.str();
        
      }
      
      friend std::ostream& operator<<(std::ostream &out, const Mult &m) {      
         out << m.toString();
         return out;      
      }
      
      static bool compBySym(const sym_t &a, const sym_t &b) {
         if(a.pow == b.pow) {
            return a.s < b.s;
         } else {
            return a.pow > b.pow;
         }
      }
      
      bool equal(const std::vector<sym_t>& a, const std::vector<sym_t>& b) const {
         
         if(a.size() != b.size()) {
            return false;
         }
         
         for(std::size_t i=0;i<a.size();i++) {
            if( (a[i].pow != b[i].pow) ||
                (a[i].s != b[i].s)) {
               return false;
            }
         }
         
         
         return true;
      }
      
      
      //std::string _sym;
      double      _v;
      std::vector<sym_t>  _mult;
   };
   
   
   class Sym {
   public:

      Sym() {

      }

      Sym(const double v) {
         _sum.push_back(Mult(v));
      }
      
      Sym(const char s[])  {
         _sum.push_back(Mult(s));
      }
      
      Sym(const Sym& orig) {
         _sum = orig._sum;
      }

      virtual ~Sym() {

      }

      /*std::string toString() const {
         std::stringstream ss;

         ss << _sym.c_str();
         
         return ss.str();
      }*/
      
      Sym operator*(const Sym& b) const {
       
         Sym result;
         
         for(std::size_t i=0;i<_sum.size();i++) {
            for(std::size_t j=0;j<b._sum.size();j++) {
               Mult m = _sum[i]*b._sum[j];
               if( !(m == 0.0) ) {
                  result += m;
               }
            }
         }
         std::sort(result._sum.begin(), result._sum.end(), Sym::compByMult);         
         return result;
      }

      Sym& operator+=(const Mult& m) {
         if(m == 0.0) {
            return *this;
         } else {
             std::vector<Mult>::iterator si = _sum.begin();
             bool added = false;
             
             while(si != _sum.end()) {
                 if(*si == m) {
                    si->_v += m._v;
                    if(*si == 0.0) {
                        _sum.erase(si);
                    }
                    added = true;
                    break;
                 }
                 si++;
             }

             if(!added) {
                 _sum.push_back(m);
             }
        }
        return *this;
      }
      
      Sym operator*(const double x) const {
         return (*this)*Sym(x);
      }
      
      Sym operator+(const Sym& m) const {
         if((*this) == 0.0) {
            return m;
         } else if(m == 0.0) {
            return *this;
         } else {
            Sym result(*this);
            for(std::size_t i=0;i<m._sum.size();i++) {
                result += m._sum[i];
            }
            std::sort(result._sum.begin(), result._sum.end(), Sym::compByMult);         
            return result;
         }
         

            
/*            
            for(std::size_t i=0;i<_sum.size();i++) {
               result._
            }
            
            
            if(equal(_mult,m._mult)) {
               if(_sum.empty()) {
                  if(m._sum.empty()) {
                     a = Sym(*this);
                     a._v += m._v;
                     return a;
                  } else {
                     a = Sym(m);
                     a._sum.push_back(Sym(_v));
                     return a;
                  }
               } else if(m._sum.empty()) {
                  a = Sym(*this);
                  a._sum.push_back(Sym(m._v));
                  return a;
               } else {
                  a = Sym(*this);
                  a._sum.insert(a._sum.end(), m._sum.begin(), m._sum.end());
                  return a;
               }
            } else {
               a = Sym(*this);
               a._sum.push_back(m);
               return a;
            }
         }  */

      }
      
      bool operator==(double x) const {
         if(_sum.empty()) {
            return true;
         } else {
            for(std::size_t i=0;i<_sum.size();i++) {
               if( !(_sum[i] == 0) ) {
                  return false;
               }
            }
         }
         return true;
      }

      Sym& operator-()  {
          for(std::size_t i=0;i<_sum.size();i++) {
              _sum[i] *= -1.0;
          }
          std::sort(_sum.begin(), _sum.end(), Sym::compByMult);         
          return (*this);
      }
      
      Sym operator-(const Sym& m) const {
         return (*this)+(-Sym(m));
      }

      bool operator>=(double v) const {
          if(_sum.empty()) {
              return 0>=v;
          }
          
          /*for(std::size_t i=0;i<_sum.size();i++) {
              if(_sum[i] >= v) {
                  return true;
              }
          }*/
          
         return _sum[0] >= v;
      }

      bool operator<(double v) const {
          if(_sum.empty()) {
              return 0.0<v;
          }
          
          return _sum[0] < v;
          /*for(std::size_t i=0;i<_sum.size();i++) {
              if(_sum[i] > v) {
                  return false;
              }
          }
          
         return true;*/
      }
      
      bool operator>(double v) const {
          if(_sum.empty()) {
              return 0.0>v;
          }
          
          return _sum[0]> v;
          
          /*for(std::size_t i=0;i<_sum.size();i++) {
              if(_sum[i] > v) {
                  return true;
              }
          }*/
          
         return false;
      }     
            
      /*std::string show(int spaces=1) const {
         std::stringstream ss;
         
         if(spaces == 1) {
            ss << "\n";
         }
         
         for(int i=0;i<spaces;i++) {
            ss << ".";
         }
         
         ss << "_v:" << _v << std::endl;
         
         for(int i=0;i<spaces;i++) {
            ss << ".";
         }

         if(!_mult.empty()) {
            ss << "_mult: ";
            for(std::size_t i=0;i<_mult.size();i++) {
               ss << _mult[i].s;
            }
            ss << std::endl;
         }
         
         //for(std::size_t i=0;i<_mult.size();i++) {
         //   ss << _mult[i].show(spaces+1);
         }//

         if(!_sum.empty()) {
            
            for(int i=0;i<spaces;i++) {
               ss << ".";
            }

            ss << "_sum:" << std::endl;
         }
         
         for(std::size_t i=0;i<_sum.size();i++) {
            ss << _sum[i].show(spaces+1);
         }
         
         return ss.str();
         
      }*/
      
      std::string toString() const {
        std::stringstream ss;

        if(_sum.empty()) {
           ss << "0";
           return ss.str();
        }
        
        //if(_pow == -1) {
        //   ss << "/";
        //}
        
        //if(_v < 0.0) {
        //   ss << "-";
        //}

        int flip = 1;
        if(_sum.size() > 1) {
           if(_sum[0] < 0.0) {
               flip = -1;
               ss << "-";
           }
           ss << "(";
        }

        
        //if( _v == -1.0) {
        //   ss << "-";
        //}
         
        //if( (std::abs(_v) != 1.0) || (_mult.empty() && _sym.empty())) {
        //   ss << std::abs(_v);
        //}
        for(std::size_t i=0;i<_sum.size();i++) {
            Mult m = _sum[i] * flip;
           if(i > 0 && m > 0.0) {
              ss << "+";
           }
           ss << m;
        }
        
        if(_sum.size() > 1) {
           ss << ")";
        }
        
        return ss.str();
        
      }
      
      friend std::ostream& operator<<(std::ostream &out, const Sym &s) {      
         out << s.toString();
         //out << s.show();
         return out;      
      }
      
       //bool operator==(const Blade& b) const {
      
   private:
 
      static bool compByMult(const Mult &a, const Mult &b) {
          if(a < 0) {
              if(b > 0) {
                  return false;
              }
          } else if(b < 0) {
              return true;
          }
          
          return true;
      }       
       
      /*bool equal(const std::vector<syms_t>& a, const std::vector<syms_t>& b) const {
         
         if(a.size() != b.size()) {
            return false;
         }
         
         for(std::size_t i=0;i<a.size();i++) {
            if( (a[i].pow != b[i].pow) ||
                (a[i].s != b[i].s)) {
               return false;
            }
         }
         
         
         return true;
      }
      
      static bool compByString(const syms_t &a, const syms_t &b) {
         if(a.pow == b.pow) {
            return a.s < b.s;
         } else {
            return a.pow > b.pow;
         }
      }*/
      
      //std::string _sym;
      std::vector<Mult>     _sum;
   };
   
   inline Sym operator*(double v, const Sym& m) {
      return m*Sym(v);
   }    

   inline Sym operator+(double v, const Sym& m) {
      return m+Sym(v);
   }       
}
   
      

#endif	/* SYM_H */

