#!/bin/bash

THEDIR=$(dirname $(readlink -e ${BASH_SOURCE[0]}))

echo +++++++++++++++++++++++++ System prerequisites +++++++++++++++++++++++++++
sudo apt-get install -f
sudo dpkg --configure -a
sudo apt-get install -f 

sudo apt-get clean
sudo apt-get autoclean

sudo apt-get install -y aptitude

echo ========================= install Intel repository =======================
wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS-2023.PUB
sudo apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS-2023.PUB
rm GPG-PUB-KEY-INTEL-SW-PRODUCTS-2023.PUB
sudo add-apt-repository "deb https://apt.repos.intel.com/oneapi all main"
sudo apt-get update

echo ========================= make same libstdc++ ===========================
# echo ========================= remove GCC ===========================
# sudo apt remove -y gcc g++ gcc-6 g++-6 gcc-7 g++-7 gcc-8 g++-8 gcc-10 g++-10 gcc-11 g++-11 libstdc++6 libstdc++-7-dev libstdc++-8-dev libstdc++-10-dev libstdc++-11-dev
echo ========================= remove by aptitude =========
sudo aptitude remove -y gcc g++ gfortran
sudo aptitude remove -y lib32stdc++6
sudo aptitude remove -y gcc-6 g++-6 gcc-6-base gcc-7 g++-7 gcc-7-base gcc-8 g++-8 gcc-8-base gcc-10 g++-10 gcc-10-base gcc-11 g++-11 gcc-11-base
sudo aptitude remove -y libstdc++6 libstdc++-7-dev libstdc++-8-dev libstdc++-10-dev libstdc++-11-dev
sudo aptitude remove -y llvm-8 llvm-9 llvm-10 llvm-11 llvm-12

echo ========================= auto-remove GCC ===========================
sudo apt autoremove
echo ========================= install GCC ===========================
sudo aptitude install -y gcc-9 g++-9 libstdc++-9-dev

echo ========================= install Intel OneAPI ===========================
sudo aptitude install -y intel-oneapi-mkl                \
                         intel-oneapi-mkl-devel          \
                         intel-oneapi-dpcpp-cpp-compiler \
                         intel-oneapi-python

echo ========================= list /opt/intel/oneapi/ ========================
ls -l /opt/intel/oneapi/

#echo ========================= Change python3 to python executanble name ======
#sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 10
#echo ========================= Python version =================================
#python --version
#python3 --version

echo ========================= install extra packages ==========================
sudo aptitude install -y cmake valgrind libgtest-dev

#echo ========================= install/delete libstdc++-dev ===================
#sudo apt remove -y gcc-7 g++-7 gcc-8 g++-8 gcc-10 g++-10
# oneapi beta 10 can not work with libstdc++-10-dev 
#sudo apt remove -y libstdc++-10-dev
#sudo apt autoremove
#sudo apt install --reinstall -y gcc-9 g++-9 libstdc++-9-dev


echo ========================= SW versions ====================================
# sudo apt list --installed

gcc --version
which gcc
g++ --version
which g++

# clang++ --version
# which clang++

# dpcpp --version
# which dpcpp

# valgrind --version
# which valgrind
