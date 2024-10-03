#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
#include <cmath>
#include <limits.h>

using namespace std;

#define V 5

const int N = 5;


// Funcion para encontrar el camino mas corto entre todos los nodos y que vuelva al nodo inicial con fuerza bruta
void finMinWeight(vector<vector<int>>& graph, string* nameLocations)
{
	// Inicializar todos los nodos como no visitados
	vector<bool> visited(V, false);
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Nodo actual
	int currPos = 0;

	// Marcar el nodo actual como visitado
	visited[currPos] = true;

	// Numero de nodos visitados
	int count = 1;

	// Almacenar el camino mas corto
	int path[V + 1];
	path[0] = currPos;

	// Almacenar el peso minimo
	int minWeight = 0;

	// Mientras no se hayan visitado todos los nodos
	while (count < V) {
		// Inicializar el peso minimo
		int min = INT_MAX;
		int nextNode = -1;
		// Recorrer los nodos
		for (int i = 0; i < V; i++) {
			if (graph[currPos][i] != 0 && visited[i] == false) {
				if (graph[currPos][i] < min) {
					min = graph[currPos][i];
					nextNode = i;
				}
			}
		}

		// Actualizar el peso minimo
		minWeight += min;
		currPos = nextNode;

		// Marcar el nodo actual como visitado
		visited[currPos] = 1;
		path[count] = currPos;
		count++;
	}

	// Sumar el ultimo nodo con el primero para completar el ciclo
	minWeight += graph[currPos][0];

	// Imprimir el camino mas corto
	cout << "El camino mas corto es: ";
	for (int i = 0; i < V; i++)
		cout << nameLocations[path[i]] << " -> ";
	cout << nameLocations[path[0]] << endl;

	// Imprimir el peso total del camino mas corto
	cout << "El peso total del camino mas corto es: " << minWeight << endl;
}

// Algoritmo vecino mas cercano 
void nearestNeighbor(vector<vector<int>>& graph, string* nameLocations)
{
	// Inicializar todos los nodos como no visitados 
	vector<bool> visited(V, false);

	// Nodo actual 
	int currPos = 0;
	// Marcar el nodo actual como visitado 

	visited[currPos] = true;

	// Numero de nodos visitados 
	int count = 1;

	//Encontrar el vecino mas cercano 
	int min = INT_MAX;
	int	nextNode = -1;
	
	cout << nameLocations[0];

	for (int i = 0; i < V; i++) {
		if (count == V) {
			visited[0] = false;
		}


		for (int j = 0; j < V; j++) {
			if (graph[currPos][j] != 0 && visited[j] == false) {
				if (graph[currPos][j] < min) {
					min = graph[currPos][j];
					nextNode = j;
				}
			}
		}
		visited[nextNode] = true;
		currPos = nextNode;
		count++;
		cout << " -> " << nameLocations[nextNode] ;
		min = INT_MAX;

	}



}

// Algoritmo Bellman-Held-Karp
int BellmanHeldKarpTSP(const vector<vector<int>>& graph) {
	int n = graph.size();
	int numSubsets = 1 << n; // 2^n, para representar todos los subconjuntos posibles de nodos
	vector<vector<int>> dp(numSubsets, vector<int>(n, INT_MAX));

	// El costo de empezar en el nodo 0 es 0
	dp[1][0] = 0;

	// Recorremos todos los subconjuntos posibles
	for (int subset = 1; subset < numSubsets; ++subset) {
		for (int u = 0; u < n; ++u) {
			// Si el nodo u no está en el subconjunto, continuamos
			if (!(subset & (1 << u))) continue;

			// Recorremos todos los nodos v para encontrar la mejor forma de llegar al nodo u desde el nodo v
			for (int v = 0; v < n; ++v) {
				if (u == v || !(subset & (1 << v))) continue;

				int prevSubset = subset & ~(1 << u);
				if (dp[prevSubset][v] != INT_MAX && graph[v][u] != 0) {
					dp[subset][u] = min(dp[subset][u], dp[prevSubset][v] + graph[v][u]);
				}
			}
		}
	}

	// Buscamos el ciclo de regreso al nodo 0 desde cualquier nodo
	int minCost = INT_MAX;
	for (int i = 1; i < n; ++i) {
		if (dp[numSubsets - 1][i] != INT_MAX && graph[i][0] != 0) {
			minCost = min(minCost, dp[numSubsets - 1][i] + graph[i][0]);
		}
	}

	return minCost == INT_MAX ? -1 : minCost;
}

// Algoritmo Branch Bound
// final_path[] almacena la solución final, es decir,
// el recorrido del viajante.

int final_res = INT_MAX;
vector<int> final_path(N + 1);
vector<bool> visited(N);

// Función para copiar la solución temporal a la solución final.
void copyToFinal(vector<int>& curr_path) {
	for (int i = 0; i < N; i++)
		final_path[i] = curr_path[i];
	final_path[N] = curr_path[0]; // Regresa a la ciudad de origen.
}

// Función para encontrar el costo mínimo de arista que termina en el vértice i.
int firstMin(const vector<vector<int>>& adj, int i) {
	int min = INT_MAX;
	for (int k = 0; k < N; k++) {
		if (adj[i][k] < min && i != k)
			min = adj[i][k];
	}
	return min;
}

// Función para encontrar el segundo costo mínimo de arista que termina en el vértice i.
int secondMin(const vector<vector<int>>& adj, int i) {
	int first = INT_MAX, second = INT_MAX;
	for (int j = 0; j < N; j++) {
		if (i == j)
			continue;

		if (adj[i][j] <= first) {
			second = first;
			first = adj[i][j];
		}
		else if (adj[i][j] <= second && adj[i][j] != first) {
			second = adj[i][j];
		}
	}
	return second;
}

// Función recursiva principal del algoritmo TSP.
void TSPRec(const vector<vector<int>>& adj, int curr_bound, int curr_weight,
	int level, vector<int>& curr_path) {
	if (level == N) {
		if (adj[curr_path[level - 1]][curr_path[0]] != 0) {
			int curr_res = curr_weight + adj[curr_path[level - 1]][curr_path[0]];
			if (curr_res < final_res) {
				copyToFinal(curr_path);
				final_res = curr_res;
			}
		}
		return;
	}

	for (int i = 0; i < N; i++) {
		if (adj[curr_path[level - 1]][i] != 0 && !visited[i]) {
			int temp = curr_bound;
			curr_weight += adj[curr_path[level - 1]][i];

			if (level == 1)
				curr_bound -= ((firstMin(adj, curr_path[level - 1]) + firstMin(adj, i)) / 2);
			else
				curr_bound -= ((secondMin(adj, curr_path[level - 1]) + firstMin(adj, i)) / 2);

			if (curr_bound + curr_weight < final_res) {
				curr_path[level] = i;
				visited[i] = true;
				TSPRec(adj, curr_bound, curr_weight, level + 1, curr_path);
			}

			curr_weight -= adj[curr_path[level - 1]][i];
			curr_bound = temp;
			fill(visited.begin(), visited.end(), false);
			for (int j = 0; j <= level - 1; j++)
				visited[curr_path[j]] = true;
		}
	}
}

// Función principal para iniciar el TSP.
void TSP(const vector<vector<int>>& adj, string* nameLocation) {
	vector<int> curr_path(N + 1, -1);
	fill(visited.begin(), visited.end(), false);

	int curr_bound = 0;

	for (int i = 0; i < N; i++)
		curr_bound += (firstMin(adj, i) + secondMin(adj, i));

	curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 : curr_bound / 2;

	visited[0] = true;
	curr_path[0] = 0;

	TSPRec(adj, curr_bound, 0, 1, curr_path);

	cout << "Costo minimo: " << final_res << endl;
	cout << "Ruta: ";
	for (int i = 0; i <= N; i++)
		cout  << nameLocation[i] << " -> ";
	cout << endl;
}
int main() {
	vector<vector<int>> graph(V, vector<int>(V, 0));
	string nameLocations[] = { "Cordoba", "Santiago del estero", "Catamarca", "Tucuman", "Salta" };
	graph =
	{
		//	Cor		San		Cat		Tuc		Sal 
			{0,		506,	441,	0,		891},
			{506,	0,		221,	112,	639},
			{441,	221,    0,      232,    0},
			{0,     112,    232,    0,      302},
			{891,   639,    0,		302,	0}
	};

	// encontrar el camino más corto TSP
	cout << "FUERZA BRUTA " << endl;
	finMinWeight(graph, nameLocations);
	cout << endl;

	cout << "VECINO MAS CERCANO" << endl;
	nearestNeighbor(graph, nameLocations);
	cout << endl;
	cout << endl;

	cout << "BELLMAN-HELD-KARP" << endl;
	int result = BellmanHeldKarpTSP(graph);
	cout << result << endl;
	cout << endl;

	cout << "BRANCH & BOUND" << endl;
	TSP(graph, nameLocations);
	cout << endl;

	return 0;
}