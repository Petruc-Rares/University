package usefuldatastructures;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Graph {
    private Map<VertexFamily, List<VertexFamily>> adjVertices = new HashMap<>();

    public void addVertex(int id) {
        adjVertices.putIfAbsent(new VertexFamily(id), new ArrayList<>());
    }

    public void removeVertex(int id) {
        VertexFamily v = new VertexFamily(id);
        adjVertices.values().stream().forEach(e -> e.remove(v));
        adjVertices.remove(new VertexFamily(id));
    }

    public void addEdge(int id1, int id2) {
        VertexFamily v1 = new VertexFamily(id1);
        VertexFamily v2 = new VertexFamily(id2);
        adjVertices.get(v1).add(v2);
        adjVertices.get(v2).add(v1);
    }

    public void removeEdge(int id1, int id2) {
        VertexFamily v1 = new VertexFamily(id1);
        VertexFamily v2 = new VertexFamily(id2);
        List<VertexFamily> eV1 = adjVertices.get(v1);
        List<VertexFamily> eV2 = adjVertices.get(v2);
        if (eV1 != null)
            eV1.remove(v2);
        if (eV2 != null)
            eV2.remove(v1);
    }

    public List<VertexFamily> getAdjVertices(int id) {
        return adjVertices.get(new VertexFamily(id));
    }

    @Override
    public String toString() {
        return "Graph{" +
                "adjVertices=" + adjVertices +
                '}';
    }
}
