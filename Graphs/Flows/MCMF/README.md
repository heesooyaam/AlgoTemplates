# **MCMF Class and How to Use It**

This guide explains how to utilize the **MCMF** class for solving Minimum Cost Maximum Flow problems.

---

## **Steps to Use the MCMF Class**

### 1. Define a Template Structure for Edges
Create a template structure for edges, specifying:
- **First template**: The data type for the edge weight.
- **Second template**: The data type for the flow.

### 2. Include Mandatory Fields in the Structure
The edge structure must include the following fields:
- `from`: The starting vertex of the edge.
- `to`: The ending vertex of the edge.
- `w`: The weight of the edge.
- `c`: The capacity of the edge.

### 3. Add Edges to a Vector
Use the static method provided by the MCMF class to populate a vector of edges:
```cpp
MCMF<TCost, TFlow, TEdge>::add_edge(vector_of_edges, from, to, weight, capacity);
```

### 4. Create an Instance of the MCMF Class
Once all edges are added to the vector, initialize the **MCMF** class with the following parameters:
```cpp
MCMF(vector_of_edges, largest_vertex, source, sink, indexation_of_vertex, eps);
```
- `vector_of_edges`: The vector containg all edges
- `largest_vertex`: The largest vertex index in the graph.
- `source`: The source vertex.
- `sink`: The sink vertex.
- `indexation_of_vertexes`: Specifies whether the vertex indices are 0-based or 1-based.
- `eps`: A small value to handle floating-point precision issues (only for fractional implementations).

### 5. Retrieve Results
After initializing the **MCMF** class, you can retrieve the results using the following methods:

- **`get_max_flow()`**
    - Retrieves the **maximum flow** in the network.

- **`get_min_cost()`**
    - Retrieves the **minimum cost** associated with the maximum flow.

- **`as_pair()`**
    - Returns a `std::pair<TCost, TFlow>` containing both the **minimum cost** and the **maximum flow**.

#### Example:
```cpp
auto max_flow = mcmf.get_max_flow();
auto min_cost = mcmf.get_min_cost();
auto [mn_cost, mn_flow] = mcmf.as_pair();

std::cout << "Maximum Flow: " << max_flow << "\n";
std::cout << "Minimum Cost: " << min_cost << "\n";
std::cout << "Cost and Flow: (" << mn_cost << ", " << mx_flow << ")\n";
```