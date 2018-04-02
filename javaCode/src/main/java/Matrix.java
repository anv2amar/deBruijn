import org.apache.commons.lang3.tuple.Pair;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by nagasaty on 11/4/17.
 */
public class Matrix implements GraphDS {

    private final int K;
    boolean[][] in,out;
    long[] parents;
    int sigma;
    public Matrix(int N, int sigma, int K) {
        in = new boolean[N][sigma];
        out = new boolean[N][sigma];
        parents = new long[N];
        this.sigma = sigma;
        this.K = K;
    }

    public void addOutGoingEdge(int vertexId, char c) {
        this.out[vertexId][RabinKarpHashing.getIndex(c)] = true;
    }

    public void addIncomingEdge(int vertexId, char c) {
        this.in[vertexId][RabinKarpHashing.getIndex(c)] = true;
    }

    public List<Integer> getOutgoingEdges(int vertexId) {
        List<Integer> outGoingVertices = new ArrayList<Integer>();
        for(int i = 0 ; i< sigma; i++){
            if(out[vertexId][i]){
                outGoingVertices.add(RabinKarpHashing.computeNextHash(vertexId, i, this.K, this.sigma));
            }
        }
        return outGoingVertices;
    }

    public List<Integer> getIncomingdges(int vertexId) {
        List<Integer> incomingVertices = new ArrayList<Integer>();
        for(int i = 0 ;i < sigma; i++){
            if(this.in[vertexId][i]) 
                incomingVertices.add(RabinKarpHashing.computePrevHash(vertexId, i, this.K, this.sigma));
        }
        return incomingVertices;
    }

    public Pair<Integer, Integer> getHeightSize(int vertexId) {
        return null;
    }

    public void updateParentPointers(int vertexId) {

    }
}
