if [ ! -d "deps" ]; then
	echo " * ./deps does not exist, creating it"
	mkdir deps
fi

cd deps

git clone git@github.com:libuv/libuv.git
cd libuv
sh autogen.sh
./configure
make
make check
make install
cd ..

if [ ! -d "h2o" ]; then
	echo " * cloning h2o"
	git clone https://github.com/h2o/h2o > h2o_git.log
	cd h2o
else
	echo " * updating h2o"
	cd h2o
	git pull origin master > h2o_git.log
fi
cmake . > h2o_cmake.log && make libh2o > h2o_make.log
echo "res "$?
cd ..

#if [ ! -d "plate-tectonics" ]; then
#	git clone https://github.com/Mindwerks/plate-tectonics
#	cd plate-tectonics
#else
#	cd plate-tectonics
#	git pull origin master
#fi
#mkdir build
#cd build
#cmake ..
#make
