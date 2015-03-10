mkdir deps

cd deps

git clone https://github.com/h2o/h2o
cd h2o
cmake .
make libh2o
cd ..

git clone https://github.com/Mindwerks/plate-tectonics
cd plate-tectonics
mkdir build
cd build
cmake ..
make
