## Install LLVM by Source ##

```
mkdir -p ~/LLVM && cd ~/LLVM
wget -O - http://llvm.org/releases/3.7.1/llvm-3.7.1.src.tar.xz | tar Jxf -
wget -O - http://llvm.org/releases/3.7.1/cfe-3.7.1.src.tar.xz | \
	tar Jxf - -C llvm-3.7.1.src/tools
```

```
mkdir -p ~/LLVM/hw1/build
```

```
cd ~/LLVM/llvm-3.7.1.src
mkdir build && cd build
cmake ..
make -j12
sudo cmake -DCMAKE_INSTALL_PREFIX=/home/morris1028/LLVM/hw1/build -P cmake_install.cmake
cd /home/morris1028/LLVM/hw1/build && cmake .. && make
```

```
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
```

### Test Dead Code Elimination Algorithm ###

#### build DCE.cpp source & compile testcases ###

```
cd ~/LLVM/hw1/test
sh test.sh
```

#### clean ####

```
sh clean.sh
```
