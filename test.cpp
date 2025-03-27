#include <iostream>
#include "graph.h"

int main()
{
	Graph campusMap;

	Node* library = campusMap.addNode("Cook Library");
	Node* dorm = campusMap.addNode("Mccarty Residence Hall");
	Node* cafateria = campusMap.addNode("Thad Cochran Center");

	campusMap.addEdge(library, cafateria, 0.13);
	campusMap.addEdge(dorm, cafateria, 0.3);
	campusMap.addEdge(library, dorm, 0.7);

	campusMap.dijkstra(library);

	campusMap.displayGraph();

}