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
    void addAliasesToBuilding();
    void printAliasesForBuilding();

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
