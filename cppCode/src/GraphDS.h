//
// Created by Naga Satya Karthik on 11/16/17.
//

#ifndef CPPCODE_GRAPHDS_H
#define CPPCODE_GRAPHDS_H

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
#include "BBHash.h"
#include "RabinKarpHash.h"
#include <bitset>

using namespace std;
typedef long long ll;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<int> vi;
typedef vector<u_int64_t> vu64;



struct Component{
    u_int64_t heightIdx, rootIdx;
    int height, size;

    Component(u_int64_t heightIdx, u_int64_t rootIdx, int height, int size): heightIdx(heightIdx), rootIdx(rootIdx), height(height), size(size) {}
};

class GraphDS {
    u_int64_t N;
    int sigma, K;
    vector<vector<bool>> IN, OUT;
    vector<u_int64_t> parentPtrs;
    map<u_int64_t, Component> componentMap;
    BBHashExt *bbHash;
    RabinKarpHash *rkHash;
    u_int64_t desired_sz, min_ht, max_ht, mid_ht;
    bool isNoParent(u_int64_t idx){
        return parentPtrs[idx] == (N+1);
    }
public:
    GraphDS(u_int64_t n, int sigma, int K, BBHashExt *bbHashExt, RabinKarpHash *rkHashExt): N(n), sigma(sigma), K(K) {
        bbHash = bbHashExt;
        rkHash = rkHashExt;
        // IN OUT assigned both to false
        IN.assign(n, vector<bool>((size_t)sigma, false));
        OUT.assign(n, vector<bool>((size_t)sigma, false));
        // parent pointers point to -1 is root node.
        parentPtrs.assign(n, n+1);
        desired_sz = (u_int64_t)ceil(K*log2(sigma));
        min_ht = desired_sz;
        max_ht = 3*desired_sz;
        mid_ht = 2*desired_sz;
        printf("\nDesiredSize: %lu, minHeight:%lu, midHeight:%lu, maxHeight:%lu\n", desired_sz, min_ht, mid_ht, max_ht);
    }

    vu64 getNeighbours(u_int64_t vertexId){
        vu64 res;
        long long rkHashVertexId = bbHash->getRabinHash(vertexId);
        for(int i = 0 ; i < sigma; i++){
            if(IN[vertexId][i])
                res.push_back(bbHash->getMPHF(rkHash->computePrevValue(rkHashVertexId, i)));
            if(OUT[vertexId][i])
                res.push_back(bbHash->getMPHF(rkHash->computeNextHash(rkHashVertexId, i)));
        }

        return res;
    }

    u_int64_t getRoot(u_int64_t idx){
        u_int64_t curr = idx;
        while(curr < N && !isNoParent(curr)){
            curr = parentPtrs[curr];
        }
        return curr;
    }

    int getDepth(u_int64_t idx){
        u_int64_t curr = idx;
        int res= 1;
        while(curr < N && !isNoParent(curr)){
            curr = parentPtrs[curr];
            res++;
        }
        return res;
    }

    int getHeightModified(u_int64_t rootIdx){
//        printf("\n\n\n\nModified height is called with rootIdx: %lu\n", rootIdx);
        vu64 neighbours = getNeighbours(rootIdx);
//        for(u_int64_t v: neighbours){
//            printf("Parent Pointer of neighbour %lu is : %lu\n", v, parentPtrs[v]);
//        }
        vu64 neighbours_filtered = filter(neighbours, rootIdx);
//        for(u_int64_t v: neighbours_filtered){
//            printf("Parent Pointer of neighbour Filtered %lu is : %lu\n", v, parentPtrs[v]);
//        }
        int maxHt = 0, temp;
        for(u_int64_t vertex : neighbours_filtered){
            temp = getHeightModified(vertex);
            if(temp > maxHt){
                maxHt = temp;
            }
        }
        return (maxHt + 1);

    }

    int getHeight(u_int64_t rootIdx){
        auto it = componentMap.find(rootIdx);
        if(it == componentMap.end()) return -1;
        return (it->second).height;
    }

    int getSize(u_int64_t rootIdx){
        auto it = componentMap.find(rootIdx);
        if(it == componentMap.end()) return -1;
        return (it->second).size;
    }

    vu64 getLeaves(u_int64_t rootVertexIdx){
        vu64 res;
        if(rootVertexIdx >= N) return res;
        queue<u_int64_t> q;
        u_int64_t vidx;
        q.push(rootVertexIdx);
        vu64 neighbours;
        while(q.size() > 0 ){
            vidx = q.front();
            neighbours = getNeighbours(vidx);
            neighbours = filter(neighbours, vidx);
            if(neighbours.size() > 0){
                for(u_int64_t neigbour: neighbours) q.push(neigbour);
            }else{
                res.push_back(vidx);
            }
        }
        return res;
    }

    vu64 filter(vu64 neighbours, u_int64_t parentPtr){
        vu64 res;
        for(u_int64_t neighbour : neighbours){
            if(parentPtrs[neighbour] == parentPtr){
                res.push_back(neighbour);
            }
        }
        return res;
    }

    enum STATE {VISITED, VISITING, UNVISITED};

//    Updates parent Pointers & Populates componentMap with the
//    root of each Componenet.
    void buildForest(){
        Component *temp;
        int height, size;

        u_int64_t heightIdx;
        u_int64_t maxHeight = max_ht;
        vector<STATE > states(N, UNVISITED);
        for( u_int64_t i = 0 ; i < N ; i++){
            if(states[i]==UNVISITED){
                height = 0;
                size = 0 ;
                heightIdx = N+1;
                dfs(i, N+1, maxHeight, &height, &size, &heightIdx, states);
                temp = new Component(heightIdx, i, height, size+1);
                componentMap.insert(make_pair(i, *temp));
            }
        }
    }

    void dfs(u_int64_t root, u_int64_t parentPtr, u_int64_t maxHeight, int *height, int *size, u_int64_t *heightIdx, vector<STATE> &states){
        if(maxHeight <=  0){
            return;
        }
        u_int64_t tempHeightIdx;
        int tempHeight;
        states[root] = VISITING;
        vu64 neighbours = getNeighbours(root);
        for(u_int64_t neighbour : neighbours){
            if(states[neighbour] == UNVISITED){
                tempHeight = 0;
                tempHeightIdx = N+1;
                dfs(neighbour, root, maxHeight - 1, &tempHeight, size, &tempHeightIdx, states);
                if((maxHeight-1) > 0) *size += 1;
                if(tempHeight > *height){
                    *height = tempHeight;
                    *heightIdx = neighbour;
                }
            }
        }
//        *size += 1;
        *height += 1;
        parentPtrs[root] = parentPtr;
        states[root] = VISITED;
    }

    int getWholeSize(u_int64_t root){
        int res=0;
        if(root >= N) return res;
        queue<u_int64_t> q;
        u_int64_t vidx;
        q.push(root);
        vu64 neighbours;
        while(q.size() > 0 ){
            vidx = q.front(); q.pop();
            res++;
            neighbours = getNeighbours(vidx);
            neighbours = filter(neighbours, vidx);
            if(neighbours.size() > 0){
                for(u_int64_t neigbour: neighbours) q.push(neigbour);
            }
        }
        return res;
    }


//    Update Parent Pointers updates all the pointers to map to idx. but doesn't change the
//    ComponentMap to new idx. This part is done in merge code.
//    Reason why we are not doing the updation of componentMap here is anyhow this old component will
//    be deleted so not performing unnecessary operation here.
    void updateParentPointers(u_int64_t idx){
        u_int64_t parent;
        if(isNoParent(idx)) return; // Current idx is the root and no change is needed.
        stack<u_int64_t> stack1;
        while(!isNoParent(idx)){ // if there is a parent of the current idx. swap the direction
            stack1.push(idx);
            if(isNoParent(idx)) break; // this conditino is always false as we are not changing the idx from while loop
            idx = (u_int64_t)parentPtrs[idx];
        }
        u_int64_t curr = idx;
        while(stack1.size() > 0){
            parentPtrs[curr] = stack1.top();
            curr = stack1.top();
            stack1.pop();
        }
        parentPtrs[curr] = N+1;
    }

    void addStaticEdge(u_int64_t i, u_int64_t j, int a, int b){
        OUT[i][a] = 1;
        IN[j][b] = 1;
    }

//  hash(U) -> i hash(V)->j
//  b = firstChar(U), a = lastChar(V)
    void addDynamicEdge(u_int64_t i, u_int64_t j, int a, int b){ //u and v are the vertex id's
        OUT[i][a] = 1;
        IN[j][b]= 1;
        u_int64_t Ci, Cj, big_c, small_c, big_i, small_j;
        int ht_Ci, ht_Cj, sz_Ci, sz_Cj, depth;
        Ci = getRoot(i);
        Cj = getRoot(j);
        if(Ci != Cj) {
            //TODO keep the component outside
//            ht_Ci = getHeight(Ci);
//            ht_Cj = getHeight(Cj);
            sz_Ci = getSize(Ci);
            sz_Cj = getSize(Cj);
            if((sz_Ci > desired_sz) ^ (sz_Cj > desired_sz)){
                // if only one of the components is greater.
                big_c = (sz_Ci > sz_Cj)?Ci:Cj;
                small_c = (sz_Ci > sz_Cj)?Cj:Ci;
                big_i = (big_c==Ci)?i:j;
                small_j = (small_c==Ci)?i:j;
                depth = getDepth(big_i);
                if(depth < mid_ht) {
                    merge(small_j, big_i, small_c, big_c, false);
                }else{
                    merge(small_j, big_i, small_c, big_c, true);
                }
            }else{
                if(sz_Ci <= desired_sz && sz_Cj <= desired_sz) {
                    merge(i, j, Ci, Cj, false);
                }else{
                    //TODO ask Dr.Christina about this case.
                }
            }
        }
    }

    void merge(u_int64_t small_j, u_int64_t big_i, u_int64_t small_root, u_int64_t big_root, bool isBreak){
        updateParentPointers(small_j);
        //update the componentMap..
        auto smaller_component_it = componentMap.find(small_root);
        auto bigger_component_it = componentMap.find(big_root);
        if(smaller_component_it == componentMap.end() || bigger_component_it == componentMap.end()){
            printf("Something is seriously wrong with the component Map updation. \n"
                           "Root of the smaller component or Root of bigger Component is not found in the componentMap.\n");
            exit(0);
        }
        Component small_component = smaller_component_it->second;
        Component bigger_component = bigger_component_it->second;
//        printComponents();
        if(!isBreak){
            parentPtrs[small_j] = big_i;
            componentMap.erase(bigger_component_it);
            //Update bigger componets size by adding the smaller components size
            componentMap.insert(make_pair(big_root, *(new Component(bigger_component.heightIdx, bigger_component.rootIdx, bigger_component.height, bigger_component.size+small_component.size))));
//            printComponents();
        }else{
            //First I need to break the bigger into 2 halves.
            // store the root of the chunk.... in chunk_root
            u_int64_t d = 0, idx = big_i, chunk_root = N+1;
            while(d++ < min_ht){
                chunk_root = idx;
                idx = parentPtrs[idx];
            }
            parentPtrs[chunk_root] = N+1; //breaking the component into chunk.
            // calculate the size of the chunk broken from the bigger part..... getWholeSize
            int chunk_size = getWholeSize(chunk_root);
            // update the bigger_component chunk size
            bigger_component.size = bigger_component.size-chunk_size;
            // add teh parent pointer of small_j is pointed to big_i;
            parentPtrs[small_j] = big_i;
            // update the chunk size by adding the smaller_component size
            chunk_size += small_component.size;
            // add the chunk root and chunk component into the componentMap.
            componentMap.insert(make_pair(chunk_root, *(new Component(small_component.heightIdx, chunk_root, small_component.height, chunk_size))));
        }
        componentMap.erase(smaller_component_it);
    }

    void printInOut(){
        printf("\nIN Matrix: \t\t OUT Matrix\n");
        for(int i = 0 ; i < IN.size(); i++){
            printf("%2d :: ", i);
            for(int j = 0 ; j < sigma; j++) printf(IN[i][j]? "1, ":"0, ");
            printf("\t");
            for(int j = 0 ; j < sigma; j++) printf(OUT[i][j]? "1, ":"0, ");
            printf("\n");
        }
//        printf("\nOut Matrix: \n");
//        for(int i = 0 ; i < IN.size(); i++){
//            for(int j = 0 ; j < sigma; j++) printf(OUT[i][j]? "1, ":"0, ");
//            printf("\n");
//        }
    }

    void printParentPointers(){
        printf("\nParentPointers: \n");
        for(int i = 0 ; i < parentPtrs.size(); i++){
            if(parentPtrs[i] != N+1){
                printf("%2d --> %lu\n", i, parentPtrs[i]);
            }else{
                printf("Root %2d --> %lu\n", i, parentPtrs[i]);
            }
        }
    }

    void printComponents(){
        printf("\nPrintComponent Objects\n");
        printf("Number of components found : %lu\n", componentMap.size());
        int i = 1;
        for(auto it = componentMap.begin(); it != componentMap.end(); it++){
            printf("Component: %2d's Root is : %lu\n", i++, it->first);
            printf("\tSize: %2d, Height:%2d, ModifiedHeight: %2d, HeightIdx: %lu\n", it->second.size, it->second.height, getHeightModified(it->first), it->second.heightIdx);
        }
    }

    void printComponentsMetrics(){
        printf("\nPrintComponentMetrics Objects\n");
        printf("Number of components found : %lu\n", componentMap.size());
        vector<int> heights;
        vector<int> sizes;
        int i = 1;
        for(auto it = componentMap.begin(); it != componentMap.end(); it++){
//            printf("Component: %2d's Root is : %lu\n", i++, it->first);
//            printf("\tSize: %2d, Height:%2d, ModifiedHeight: %2d, HeightIdx: %lu\n", it->second.size, it->second.height, getHeightModified(it->first), it->second.heightIdx);
            heights.push_back(getHeightModified(it->first));
            sizes.push_back(it->second.size);
        }
        double avgSize, avgHt, sz = sizes.size();
        for(int i = 0 ; i < sizes.size(); i++){
            avgSize += (sizes[i]/sz);
            avgHt += (heights[i]/sz);
        }
        printf("Avg Height of all components: %.2f\n", avgHt);
        printf("Avg Size of all components: %.2f\n", avgSize);
    }

    bool invariantCheck(){
        int heightNotSatisfied = 0 , sizesNotSatisifed = 0;
        for(auto it = componentMap.begin(); it != componentMap.end(); it++){
//            printf("Component: %2d's Root is : %lu\n", i++, it->first);
//            printf("\tSize: %2d, Height:%2d, ModifiedHeight: %2d, HeightIdx: %lu\n", it->second.size, it->second.height, getHeightModified(it->first), it->second.heightIdx);
            if(getHeightModified(it->first) > max_ht) heightNotSatisfied++;
            if(it->second.size < desired_sz) sizesNotSatisifed++;
        }
        printf("No of Components Found: %lu\n", componentMap.size());
        printf("Height Invariant is not satisfied in %d components.\n"
                       "Size Invariant is not satisfied in %d components.\n", heightNotSatisfied, sizesNotSatisifed);
    }
};


#endif //CPPCODE_GRAPHDS_H
