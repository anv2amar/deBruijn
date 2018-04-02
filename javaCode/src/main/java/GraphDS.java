import org.apache.commons.lang3.tuple.Pair;

import java.util.List;

/**
 * Created by nagasaty on 11/4/17.
 */
public interface GraphDS {
    void addOutGoingEdge(int vertexId, char c);
    void addIncomingEdge(int vertexId, char c);
    List<Integer> getOutgoingEdges(int vertexId);
    List<Integer> getIncomingdges(int vertexId);
    Pair<Integer, Integer> getHeightSize(int vertexId);
    void updateParentPointers(int vertexId);
}
