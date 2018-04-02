#/bin/bash
git config --global http.sslVerify false
cd /DeBruijnGraph/cppCode/
cmake . && make && ./cppCode