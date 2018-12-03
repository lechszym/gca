#ifndef TEST_GENB_H
#define TEST_GENB_H

#include <Mvec>

gca::Blade<float> generate_blade(unsigned int maxGrade,unsigned int minGrade=0, unsigned int maxBase=0);
gca::Mvec<float> generate_mvec(unsigned int maxBlades,unsigned int maxGrade, unsigned int minGrade=0);

#endif