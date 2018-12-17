set(Boost_USE_STATIC_LIBS   ON)
ENABLE_TESTING()
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES} ${PYTHON_LIBRARIES}) # Deprecated but so convenient!

include_directories(${gca_SOURCE_DIR}/gca)

#PYTHON_ADD_MODULE(gca gca.cpp)
Python3_add_library (gca MODULE gca.cpp)
set_property(TARGET gca PROPERTY CXX_STANDARD 11)
#FILE(COPY cosc343world.py DESTINATION .)
#FILE(COPY world.py DESTINATION .)
#FILE(COPY images DESTINATION .)
