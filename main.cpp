#include <iostream>
#include <vector>
#include <queue>
#include <string.h>

using namespace std;

#define V 5

// Funcion para encontrar el camino mas corto entre todos los nodos y que vuelva al nodo inicial con fuerza bruta
void finMinWeight(vector<vector<int>> graph, string* nameLocations)
{
	// Inicializar todos los nodos como no visitados
	vector<bool> visited (V,false); 
	for (int i = 0; i < V; i++)
		visited[i] = 0;

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
		cout << nameLocations[path[i]] << " ";
	cout << nameLocations[path[0]] << endl;

	// Imprimir el peso total del camino mas corto
	cout << "El peso total del camino mas corto es: " << minWeight << endl;
}


int main() 
{
	cout << "Algoritmo fuerza bruta" << endl;
	vector<vector<int>> graph(V, vector<int>(V ,0));
	string nameLocations[] = { "Cordoba", "Santiago del estero", "Catamarca", "Tucuman", "Salta" };
	graph =
	{
		//	Cor		San		Cat		Tuc		Sal 
			{0,		506,	441,	0,		877},
			{506,	0,		221,	112,	639},
			{441,	221,    0,      232,    0},
			{0,     112,    232,    0,      302},
			{877,   639,    0,		302,	0}
	};

	// encontrar el camino más corto TSP
	finMinWeight(graph, nameLocations);	

	return 0;
}