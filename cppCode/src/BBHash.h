//
// Created by Naga Satya Karthik on 11/14/17.
//

#ifndef CPPCODE_BBHASH_H
#define CPPCODE_BBHASH_H

#include "iostream"
#include "map"
#include "vector"
#include "string"
#include "stack"
#include "queue"
#include "set"
#include "unistd.h"
#include "sstream"
#include "fstream"
#include "sys/wait.h"
#include <bitset>

using namespace std;

#include "BooPHF.h"
typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<  long long, hasher_t  > boophf_t;

class BBHashExt {
    boophf_t * bphf;
    vector<long long> *rkeys; // reverse mapping.. index is the mphf lookup value..
    // element value is the rk hahs value
    int n;
public:
    BBHashExt(vector<long long> values) {
        set<long long> valuesSet(values.begin(), values.end());
        vector<long long> tempValues(valuesSet.begin(), valuesSet.end());
        bphf = new boomphf::mphf<long long,hasher_t>(valuesSet.size(),tempValues,1);
        u_int64_t idx;
//        int max = 0;
//        for(long long value: values){
//            idx = bphf->lookup(value);
//            if(idx > max) max = (int)idx;
//        }
//        max = max+1;
        n = valuesSet.size();
        rkeys = new vector<long long>(valuesSet.size(), 0);
        for(long long value: values){
            idx = bphf->lookup(value);
            (*rkeys).at(idx) = value;
        }
        printf("size of the unique hash Keys : is : %d for Kmers of size: %d\n", (int)(*rkeys).size(), (int)values.size());
    }

    BBHashExt(vector<vector<long long>>  values){
        set<long long> valuesSet;
        for(vector<long long> val: values){
            valuesSet.insert(val.begin(), val.end());
        }
        vector<long long> tempValues(valuesSet.begin(), valuesSet.end());
        bphf = new boomphf::mphf<long long,hasher_t>(valuesSet.size(),tempValues,1);
        u_int64_t idx;
        n = valuesSet.size();
        rkeys = new vector<long long>(valuesSet.size(), 0);
        for(vector<long long> val: values){
            for(long long value : val){
                idx = bphf->lookup(value);
                (*rkeys).at(idx) = value;
            }
        }
        printf("size of the unique hash Keys : is : %d for Kmers of size: %d\n", (int)(*rkeys).size(), (int)(values.size()*values[0].size()));
    }

    long long getRabinHash(u_int64_t vertexId){
        if(vertexId >= (*rkeys).size()) return -1;
        return (*rkeys).at(vertexId);
    }

    int getSize(){
        return n;
    }

// Assuming that rabinKarpHash is always present
    u_int64_t getMPHF(long long rabinKarphHash){
        u_int64_t idx = bphf->lookup(rabinKarphHash);
        return idx;
    }
};
#endif //CPPCODE_BBHASH_H
