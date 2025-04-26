#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "graph.h"

class EagleMaps {
private:
    Graph campusMap;
    std::vector<std::string> buildingNames;

    // Helper methods
    void initializeMap();
    void displayMenu();
    void displayBuildings();
    int getBuildingSelection();
    int calculateWalkingTime(double distance);
    void addNewBuilding();
    void removeBuilding();
    void manageAliases(); // New method for managing aliases
    void saveLoadMap();   // New method for saving/loading maps

public:
    // Constructor and main methods
    EagleMaps();
    void run();
    void findShortestPath();
    void planScheduleRoute();
    void displayMapConnections();
    void findNearbyBuildings();
    void modifyMap();
    Graph& getCampusMap() { return campusMap; }
};
