cd build || exit 1
del -r ./*
cmake ..
cmake --build .

exit 0