/* 
 * File:   gca.h
 * Author: lechszym
 *
 * Created on 17 March 2015, 9:25 AM
 */

#ifndef GCA_H
#define	GCA_H



namespace gca {
    
#ifndef GCA_ZERO
#define GCA_ZERO  1e-12
#endif

inline  double check_precision_for_zero(double x) {
    if( (x < GCA_ZERO) && (x > -GCA_ZERO)) {
        return 0.0;
    } else {
        return x;
    }
}
    
    
}

#endif	/* GCA_H */

