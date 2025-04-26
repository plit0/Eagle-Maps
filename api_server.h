#include <iostream>
#include <vector>
#include <string>
#include "graph.h"

class APIServer {
private:
    // Reference to the campus map
    Graph& campusMap;

    // List of building names
    std::vector<std::string> buildingNames;

public:
    // Constructor taking a reference to the campus map
    APIServer(Graph& map);

    // Start the server on a specified port
    void startServer(int port = 8080);  // Keep the default argument here

    // Get all buildings
    void handleGetBuildings();

    // Find path between two buildings
    void handleFindPath(const std::string& startBuilding, const std::string& endBuilding);

    // Find nearby buildings
    void handleFindNearbyBuildings(const std::string& referenceBuilding, double maxDistance);

    // Add a new building
    void handleAddBuilding(const std::string& buildingName);

    // Remove a building
    void handleRemoveBuilding(const std::string& buildingName);

private:
    // Check if a building exists
    bool buildingExists(const std::string& buildingName);

    // Get connected buildings for a given building
    std::vector<std::string> getConnectedBuildings(const std::string& buildingName);

    // Convert path to a displayable string
    std::string convertPathToString(const std::vector<std::string>& path);

    // Error logging method
    void logError(const std::string& errorMessage);
};