#!/bin/bash

wget http://llvm.org/releases/3.8.0/llvm-3.8.0.src.tar.xz
tar Jxvf llvm-3.8.0.src.tar.xz


# ubuntu 14.04
# $ wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key|sudo apt-key add -
# $ vim /etc/apt/sources.lst
#   ## add into last line 
#   deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.8 main
# $ sudo apt-get update
sudo apt-get install g++-multilib clang-3.8
