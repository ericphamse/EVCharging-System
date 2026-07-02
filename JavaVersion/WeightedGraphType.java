import java.io.*;
import java.util.*;

class WeightedGraphType {
    int gSize;
    List<Integer>[] graph;
    double[][] weights;

    @SuppressWarnings("unchecked")
    public WeightedGraphType(int size) {
        gSize = size;
        graph = new ArrayList[gSize];
        weights = new double[gSize][gSize];

        for (int i = 0; i < gSize; i++)
            graph[i] = new ArrayList<>();

        try (Scanner sc = new Scanner(new File("Weights.txt"))) {
            for (int i = 0; i < gSize; i++) {
                for (int j = 0; j < gSize; j++) {
                    double value = sc.nextDouble();
                    if (value == 0)
                        weights[i][j] = Double.MAX_VALUE;
                    else {
                        weights[i][j] = value;
                        graph[i].add(j);
                    }
                }
            }
        } catch (Exception e) {
            System.out.println("Cannot open input file.");
        }
    }

    public List<Integer> getAdjancencyList(int index) {
        return graph[index];
    }

    public double getWeight(int i, int j) {
        return weights[i][j];
    }

    public List<Integer> breadthFirstTraversal() {
        List<Integer> result = new ArrayList<>();
        boolean[] visited = new boolean[gSize];
        Queue<Integer> queue = new LinkedList<>();

        for (int i = 0; i < gSize; i++) {
            if (!visited[i]) {
                queue.add(i);
                visited[i] = true;
                result.add(i);

                while (!queue.isEmpty()) {
                    int u = queue.poll();
                    for (int w : graph[u]) {
                        if (!visited[w]) {
                            queue.add(w);
                            visited[w] = true;
                            result.add(w);
                        }
                    }
                }
            }
        }

        return result;
    }
}