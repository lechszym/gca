function make

mex -output gcamvec -v -I../gca -I../src CXXFLAGS="\$CXXFLAGS -std=c++0x" gcamvec.cpp mxMvec.cpp

end
