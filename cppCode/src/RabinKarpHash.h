//
// Created by Naga Satya Karthik on 11/10/17.
//

#ifndef CPPCODE_RABINKARPHASH_H
#define CPPCODE_RABINKARPHASH_H

#include <vector>
#include "iostream"
#include "math.h"
using namespace std;

class RabinKarpHash {
    int powk1;
    int k;
public:

    RabinKarpHash(int k) : k(k), powk1((int)pow(4, k-1)) {}

    int genToI(char c){
        switch(c){
            case 'A': return 0;
            case 'T': return 1;
            case 'G': return 2;
            case 'C': return 3;
            default: return -1;
        }
    }

    char genCToI(int i){
        switch(i){
            case 0: return 'A';
            case 1: return 'T';
            case 2: return 'G';
            case 3: return 'C';
        }
        return 'X';
    }

    void getMin(vector<vector<int>> tarr, int* mini, int* minj, int curri, int tidx){
        int minLength = (*minj - *mini + 1);
        for(int i = 0 ; i < tarr[tidx].size(); i++){
            if(tidx== tarr.size()-1){
                if((tarr[tidx][i]-curri+1)  < (minLength)){
                    *mini = curri;
                    *minj = tarr[tidx][i];
                }
            }else if(tidx == 0){
                if((tarr[tidx][i]-curri+1)  < (minLength)){
                    getMin(tarr, mini, minj, tarr[tidx][i], tidx+1);
                }
            }else{
                if((tarr[tidx][i]-curri+1)  < (minLength)){
                    getMin(tarr, mini, minj, curri, tidx+1);
                }
            }
        }
    }

    long long rabinkarpHash(string s){
        //ATGC - 0123 all elase -1
        long long res = 0 ;
        int intval;
        for(char c: s){
            intval = genToI(c);
            if(intval == -1) return -1;
            res = res*4 + intval;
        }
        return res;
    }

    long long computeNextHash(long long initialHash, char c){
        if(genToI(c) == -1) return -1;
        return initialHash%powk1 * 4 + genToI(c);
    }

    long long computeNextHash(long long initialHash, int id){
        if(id == -1) return -1;
        return initialHash%powk1 * 4 + id;
    }

    long long computePrevValue(long long initialHash, char c){
        if(genToI(c)==-1) return -1;
        return genToI(c)*powk1 + initialHash/4;
    }

    long long computePrevValue(long long initialHash, int id){
        if(id==-1) return -1;
        return id*powk1 + initialHash/4;
    }

    char getFirstChar(long long initialHash){
        return genCToI((int)(initialHash/powk1));
    }

    char getLastChar(long long initalHash){
        return genCToI((int)(initalHash%4));
    }

    int getFirstCharI(long long initialHash){
        return genToI(getFirstChar(initialHash));
    }

    int getLastCharI(long long initalHash){
        return genToI(getLastChar(initalHash));
    }
};


#endif //CPPCODE_RABINKARPHASH_H
