function make

mex -largeArrayDims CXXFLAGS="\$CXXFLAGS -std=c++11" -output gcamvec -v -I../gca -I../src gcamvec.cpp

end
