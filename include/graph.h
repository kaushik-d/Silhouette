#include <iostream>
#include <vector>
#include <stack>

class Graph {
public:
    int V;
    std::vector<std::vector<int>> adj;

    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int x, int u, int v) {
        adj[x].push_back(v);
        adj[x].push_back(u);
    }

    void DFS(int v, std::vector<bool>& visited, std::vector<int>& component) {
        std::stack<int> stack;
        stack.push(v);
        while (!stack.empty()) {
            int node = stack.top();
            stack.pop();
            if (!visited[node]) {
                visited[node] = true;
                component.push_back(node);
                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        stack.push(neighbor);
                    }
                }
            }
        }
    }

    std::vector<std::vector<int>> getConnectedComponents() {
        std::vector<bool> visited(V, false);
        std::vector<std::vector<int>> components;

        for (int v = 0; v < V; ++v) {
            if (!visited[v]) {
                std::vector<int> component;
                DFS(v, visited, component);
                components.push_back(component);
            }
        }

        return components;
    }
};

//int main() {
//    Graph graph(8);
//    graph.addEdge(0, 1);
//    graph.addEdge(0, 2);
//    graph.addEdge(1, 2);
//    graph.addEdge(3, 4);
//    graph.addEdge(5, 6);
//    graph.addEdge(5, 7);
//
//    std::vector<std::vector<int>> components = graph.getConnectedComponents();
//
//    cout << "Connected components:\n";
//    for (const auto& component : components) {
//        for (int node : component) {
//            cout << node << " ";
//        }
//        cout << endl;
//    }
//
//    return 0;
//}
