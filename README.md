This is an academic project implemented in Fall 2017 at University of Florida in course CIS 6930: Big Data for Biological Applications/Bioinformatic Algorithms. This project is implemented under the guidance of [Dr.Christina Boucher](https://www.christinaboucher.com/). 

Contributers to this project are : 
[Amarnath Vaid](http://github.com/anv2amar)
[Akshat Bhardwaj](https://github.com/bhardwajat)
[Naga Satya Karthik, Narikimilli](http://github.com/NNSatyaKarthik)
[Sahil Tiwari](https://github.com/tiwarisahil91)

# Problem Statement
de Bruijn Graphs are succinct data structures. In bioinformatics, they are used for assembly of genome sequences obtained from a Next Generation Sequence (NGS) library. These de Bruijn graphs must be implemented using data structures that are space and time efficient. Djamal Bellazzougui laid out an approach to implement deBruijn graphs efficiently which supported dynamic insertion and deletion at the same time in his paper [Fully Dynamic De Bruijn Graphs](https://arxiv.org/abs/1607.04909). The aim of our project is to implement Dynamic addition and deletion of edges into the de Bruijn graph which is constructed using a similar compact data structure.

# Sample data
The sample datasets that are used in this project are found in this [folder](https://github.com/anv2amar/deBruijn/tree/master/datasets).

# Code & Executable. 
Source code of this project in the directory [cppCode](https://github.com/anv2amae/deBruijn/tree/master/cppCode). Follow the steps to build an executable. If you have an executable already refer to the section [Running an executable](#Running an Executable). 
## Build the Executable
### Prerequisites
- Cmake
  - We need [cmake](https://cmake.org/) pre installed on the host machine where you run the code. Follow the steps on this [site](https://cmake.org/download/) to download and install cmake on your machine OS. 
  - Follow these instructions to download a tarball and install cmake from source [Needs sudo access for this step]
  ```
  #install cmake

  #updates repo
  sudo apt-get update 
  sudo apt-get install -y --no-install-recommends apt-utils build-essential sudo git wget tree

  #Downloads cmake, install , make it available
  wget -c --no-check-certificate https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.sh && chmod +x cmake-3.10.0-Linux-x86_64.sh && ./cmake-3.10.0-Linux-x86_64.sh --skip-license --include-subdir && sudo ln -s `pwd`/cmake-3.10.0-Linux-x86_64/bin/* /usr/local/bin 
  echo `cmake --version`
  ```
  Output when you execute the last command shoud be: 
  ```
  cmake version 3.10.0 CMake suite maintained and supported by Kitware (kitware.com/cmake).
  ```
  
- Boost Installed
  - On mac run the following command:
  ```
  brew install boost
  ```
  - On ubuntu run the following command: 
  ```
  sudo apt-get install  -y libboost-all-dev aptitude && aptitude search boost
  ```
  - To check which version of boost is installed on your machine: 
  ```
  dpkg -s libboost-dev | grep 'Version'
  ```
  Sample output should be 
  ```
  Version: 1.58.0.1ubuntu1
  ```
### Clone the repo & Build an executable
Assuming now both boost and cmake are installed on your machine

#### Clone the repo
Run the following steps to get the executable of the project. Name of the executable is **cppCode**. 
```
git clone https://github.com/anv2amar/deBruijn.git
```

#### Build the executable on your machine
```
cd deBruijn/cppCode/
cmake . 
make

```

An executable file **cppCode** is residing in your machin in the directory DeBruijnGraph/cppCode/. 

Since we already in the DeBruijnGraph/cppCode, cppCode executable should be in current working directory.

#### Running the executable from your machine
Some of the paths in the code are hardcoded for now. So we need to run the executable from the top directory

Go to the folder where you were while executing the *git clone* step.
```
cd ../../
```

Once you are in the write directory Execute the executable: 
```
./deBruijn/cppCode/cppCode --help
```
This should output as follows: 
```

Allowed options:
  --help                       produce help message
  --choice arg                 select the methods to run: 
                               0: Run the test cases
                               1: preprocess the input file to the output file.
                               Please give the container paths here. (if using 
                               docker).2: Add dynamic edges from the file
  --input-file arg             input file, should be a fastq (used for choice 1
                               & 2)
  --output-file arg            output file, can be any file 
  --limit arg (=1000)          limit to number of sequence reads while 
                               preprocessing (Choice 1). Defaults to 1000 
                               sequence reqds from fastq files
  --k arg (=15)                value of K in the k-mer
  --append-mode arg (=0)       append-mode add dynamic edges to the existing 
                               graph
                                if appendMode is false then all operations are 
                               individual
  --static-fraction arg (=0.5) Fraction of static edges that needs to be added 
                               before adding dynamic edges

```

#### Compiling and getting an executable from the Dockerfile. 

Prerequisite : Install docker on your machine. Download it from [here](https://www.docker.com/community-edition#/download)
For Ubuntu : Follow instructions from [here](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-16-04)

Note: 
- Make sure you have an internet connection before building the image. 
- Add sudo to the following commands if you are running on linux machines.

cd to the this current directory: 
```
git clone https://github.com/anv2amar/deBruijn.git && cd deBruijn/
```
Steps to build the image: Builds an image all the cmake/boost pre installed.
```
docker build -t ub:latest .
```

Steps to run the image: This 
- compiles code
- builds an executable
- Run's the main command.

```
docker run -it -v "$(pwd)":/deBruijn  ub:latest /bin/bash /deBruijn/compile.sh
```

If you need to just run the executable (only execute this after atleast executing previous command)
```
docker run -it -v "$(pwd)":/deBruijn  db:latest  /deBruijn/cppCode/cppCode --help
```

Here is a sample help message from the executable: 
**help message below might be a bit old. Please run the help msg for the latest one.**
```
Invalid Choice selected. Please select the choice from 0 - 2, 
Allowed options:
  --help                       produce help message
  --choice arg                 select the methods to run: 
                               0: Run the test cases
                               1: preprocess the input file to the output file.
                               Please give the container paths here. (if using 
                               docker).2: Add dynamic edges from the file
  --input-file arg             input file, should be a fastq (used for choice 1
                               & 2)
  --output-file arg            output file, can be any file 
  --limit arg (=1000)          limit to number of sequence reads while 
                               preprocessing (Choice 1). Defaults to 1000 
                               sequence reqds from fastq files
  --k arg (=15)                value of K in the k-mer
  --append-mode arg (=0)       append-mode add dynamic edges to the existing 
                               graph
                                if appendMode is false then all operations are 
                               individual
  --static-fraction arg (=0.5) Fraction of static edges that needs to be added 
                               before adding dynamic edges


```

For running the Test Cases: 
```
docker run -it -v "$(pwd)":/deBruijn  db:latest  /deBruijn/cppCode/cppCode --choice 0
```

###  For Preprocessing the fastq to some other format 
**Data file needs to be present in the DeBruijnGraphs/ folder.**

Place the data files in the DeBruijnGraph and give the path as "/DeBruijnGraphs/\<*where the data file is*\>"
```
docker run -it -v "$(pwd)":/deBruijn  db:latest  /deBruijn/cppCode/cppCode \
 --choice 1 \
 --input-file /deBruijn/datasets/escherichia_coli_k12_mg1655.m130404_014004_sidney_c100506902550000001823076808221337_s1_p0.1.fastq \
 --output-file /deBruijn/datasets/escherichia_coli_k12_mg1655.m130404_014004_sidney_c100506902550000001823076808221337_s1_p0.1.fastq.out

```

### For adding dynamic edges with a static fraction of 0.5
```
docker run -it -v "$(pwd)":/deBruijn  db:latest /deBruijn/cppCode/cppCode \                              
--choice 2 \
--k 15  \             
--static-fraction 0.5 \
--input-file /deBruijn/datasets/escherichia_coli_k12_mg1655.m130404_014004_sidney_c100506902550000001823076808221337_s1_p0.1.fastq.out 
```
This build process is tested with the following version of docker on mac. 
```
➜  cppCode git:(master) ✗ docker --version
Docker version 17.09.0-ce, build afdb6d4
➜  cppCode git:(master) ✗ docker-compose --version
docker-compose version 1.16.1, build 6d1ac21
➜  cppCode git:(master) ✗ docker-machine --version
docker-machine version 0.12.2, build 9371605
➜  cppCode git:(master) ✗
```

### Possible Errors while installing and running the code 
- While installing Cmake: 
```
CMake Error at CMakeLists.txt:2 (project):
No CMAKE_CXX_COMPILER could be found.
```
On Ubuntu machine, run the following command to resolve the issue: 
```
sudo apt-get update && sudo apt-get install build-essential ```
```

For details Introduction to the flow of the algorithm, Refer to this [ReadMe.md](https://github.com/anv2amar/deBruijn/blob/master/cppCode/README.md)

