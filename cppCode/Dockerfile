# Steps to run the docker file
# use sudo based on your host machine (for linux)
# docker build -t ub:latest .

FROM ubuntu:latest
RUN apt-get update
RUN apt-get install -y --no-install-recommends apt-utils build-essential sudo git wget libboost-all-dev aptitude
RUN wget --no-check-certificate https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.sh && chmod +x cmake-3.10.0-Linux-x86_64.sh && ./cmake-3.10.0-Linux-x86_64.sh --skip-license --include-subdir && ln -s `pwd`/cmake-3.10.0-Linux-x86_64/bin/* /usr/local/bin 
RUN aptitude search boost
RUN cmake --version