#include "graph.h"
#include "eaglemaps.h"
#include "api_server.h"

int main() {
    // Create the campus map
    EagleMaps eagleMaps;
    Graph& campusMap = eagleMaps.getCampusMap();

    // Create API server
    APIServer apiServer(campusMap);

    // Example API calls (you can modify or remove these)
    apiServer.handleGetBuildings();
    apiServer.handleFindPath("Vann Hall", "Cook Library");
    apiServer.handleFindNearbyBuildings("Vann Hall", 0.5);

    // Start the server (currently just prints a message)
    apiServer.startServer(8080);

    return 0;

}