#include "eaglemaps.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// Constructor
EagleMaps::EagleMaps() {
    initializeMap();
}

// Initialize the campus map with building data
void EagleMaps::initializeMap() {
    // Add all campus buildings as nodes
    Node* vann = campusMap.addNode("Vann");
    Node* scott = campusMap.addNode("Scott");
    Node* luckyday = campusMap.addNode("Luckyday");
    Node* cpn = campusMap.addNode("CPN");
    Node* mccarty = campusMap.addNode("Mccarty Residence Hall");
    Node* wilber = campusMap.addNode("Wilber");
    Node* hillcrest = campusMap.addNode("Hillcrest");
    Node* thad = campusMap.addNode("Thad Cochran Center");
    Node* library = campusMap.addNode("Cook Library");
    Node* mathZone = campusMap.addNode("MathZone");
    Node* liberalArts = campusMap.addNode("Liberal Arts Building");
    Node* asbury = campusMap.addNode("Asbury Hall");
    Node* lucas = campusMap.addNode("Lucas Administration Building");
    Node* elizabeth = campusMap.addNode("Elizabeth Harkins");
    Node* honorBusiness = campusMap.addNode("Honor Business (HB)");

    // Store building names for display purposes
    buildingNames = {
        "Vann", "Scott", "Luckyday", "CPN", "Mccarty Residence Hall",
        "Wilber", "Hillcrest", "Thad Cochran Center", "Cook Library",
        "MathZone", "Liberal Arts Building", "Asbury Hall",
        "Lucas Administration Building", "Elizabeth Harkins", "Honor Business (HB)"
    };

    // Add building aliases for easier lookup
    campusMap.addAlias("Thad", "Thad Cochran Center");
    campusMap.addAlias("Student Union", "Thad Cochran Center");
    campusMap.addAlias("Library", "Cook Library");
    campusMap.addAlias("Mccarty", "Mccarty Residence Hall");
    campusMap.addAlias("HB", "Honor Business (HB)");
    campusMap.addAlias("LAB", "Liberal Arts Building");

    // Add edges between buildings with distances in miles
    // Vann connections
    campusMap.addEdge(vann, scott, 0.4, 10.0);
    campusMap.addEdge(vann, luckyday, 0.4, 9.0);
    campusMap.addEdge(vann, cpn, 0.5, 12.0);
    campusMap.addEdge(vann, mccarty, 0.3, 5.0);
    campusMap.addEdge(vann, wilber, 0.3, 12.0);
    campusMap.addEdge(vann, hillcrest, 0.6, 10.0);
    campusMap.addEdge(vann, thad, 0.4, 3.0);
    campusMap.addEdge(vann, library, 0.2, 10.0);
    campusMap.addEdge(vann, mathZone, 0.2, 10.5);
    campusMap.addEdge(vann, liberalArts, 0.4, 15.0);
    campusMap.addEdge(vann, asbury, 0.4, 20.0);
    campusMap.addEdge(vann, lucas, 0.1, 17.0);
    campusMap.addEdge(vann, elizabeth, 0.4, 20.0);
    campusMap.addEdge(vann, honorBusiness, 0.2, 25.0);

    // Scott connections
    campusMap.addEdge(scott, luckyday, 0.2, 5.0);
    campusMap.addEdge(scott, cpn, 0.3, 6.0);
    campusMap.addEdge(scott, mccarty, 0.2, 4.0);
    campusMap.addEdge(scott, wilber, 0.6, 12.0);
    campusMap.addEdge(scott, hillcrest, 0.3, 6.0);
    campusMap.addEdge(scott, thad, 0.3, 5.0);
    campusMap.addEdge(scott, library, 0.4, 8.0);
    campusMap.addEdge(scott, mathZone, 0.4, 8.0);
    campusMap.addEdge(scott, liberalArts, 0.6, 12.0);
    campusMap.addEdge(scott, asbury, 0.7, 14.0);
    campusMap.addEdge(scott, lucas, 0.6, 12.0);
    campusMap.addEdge(scott, elizabeth, 0.7, 14.0);
    campusMap.addEdge(scott, honorBusiness, 0.5, 10.0);

    // Luckyday connections
    campusMap.addEdge(luckyday, cpn, 0.4, 8.0);
    campusMap.addEdge(luckyday, mccarty, 0.2, 4.0);
    campusMap.addEdge(luckyday, wilber, 0.5, 10.0);
    campusMap.addEdge(luckyday, hillcrest, 0.4, 8.0);
    campusMap.addEdge(luckyday, thad, 0.1, 2.0);
    campusMap.addEdge(luckyday, library, 0.3, 6.0);
    campusMap.addEdge(luckyday, mathZone, 0.3, 6.0);
    campusMap.addEdge(luckyday, liberalArts, 0.5, 10.0);
    campusMap.addEdge(luckyday, asbury, 0.6, 12.0);
    campusMap.addEdge(luckyday, lucas, 0.5, 10.0);
    campusMap.addEdge(luckyday, elizabeth, 0.6, 12.0);
    campusMap.addEdge(luckyday, honorBusiness, 0.5, 10.0);

    // CPN connections
    campusMap.addEdge(cpn, mccarty, 0.2, 4.0);
    campusMap.addEdge(cpn, wilber, 0.7, 14.0);
    campusMap.addEdge(cpn, hillcrest, 0.3, 6.0);
    campusMap.addEdge(cpn, thad, 0.4, 8.0);
    campusMap.addEdge(cpn, library, 0.5, 10.0);
    campusMap.addEdge(cpn, mathZone, 0.4, 8.0);
    campusMap.addEdge(cpn, liberalArts, 0.6, 12.0);
    campusMap.addEdge(cpn, asbury, 0.7, 14.0);
    campusMap.addEdge(cpn, lucas, 0.6, 12.0);
    campusMap.addEdge(cpn, elizabeth, 0.7, 14.0);
    campusMap.addEdge(cpn, honorBusiness, 0.6, 25.0);

    // Wilber connections
    campusMap.addEdge(wilber, hillcrest, 0.5, 20.0);
    campusMap.addEdge(wilber, thad, 0.6, 7.0);
    campusMap.addEdge(wilber, library, 0.4, 12.0);
    campusMap.addEdge(wilber, mathZone, 0.5, 13.0);
    campusMap.addEdge(wilber, liberalArts, 0.6, 15.0);
    campusMap.addEdge(wilber, asbury, 0.7, 17.0);
    campusMap.addEdge(wilber, lucas, 0.4, 25.0);
    campusMap.addEdge(wilber, elizabeth, 0.7, 23.0);
    campusMap.addEdge(wilber, honorBusiness, 0.1, 30.0);

    // Library - MathZone is very close
    campusMap.addEdge(library, mathZone, 0.05, 0.30); // approx 259 feet
}

// Display the main menu
void EagleMaps::displayMenu() {
    cout << "\n===== USM EAGLE MAPS NAVIGATION SYSTEM =====\n";
    cout << "1. Display All Buildings\n";
    cout << "2. Find Shortest Path Between Two Buildings\n";
    cout << "3. Plan a Schedule Route\n";
    cout << "4. Display Campus Map Connections\n";
    cout << "5. Find Nearby Buildings\n";
    cout << "6. Add or Remove a Building\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

// Display all buildings with numbers for selection
void EagleMaps::displayBuildings() {
    cout << "\n===== CAMPUS BUILDINGS =====\n";
    for (size_t i = 0; i < buildingNames.size(); i++) {
        cout << i + 1 << ". " << buildingNames[i] << endl;
    }
    cout << "============================\n";
}

// Get building selection from user
int EagleMaps::getBuildingSelection() {
    int selection;
    displayBuildings();
    cout << "Enter the number of the building (1-" << buildingNames.size() << "): ";
    cin >> selection;

    // Validate input
    while (selection < 1 || selection > static_cast<int>(buildingNames.size())) {
        cout << "Invalid selection. Please enter a number between 1 and " << buildingNames.size() << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> selection;
    }

    return selection - 1; // Convert to 0-based index
}

// Calculate estimated walking time in minutes
int EagleMaps::calculateWalkingTime(double distance) {
    // Average walking speed is roughly 3-4 mph, we'll use 3.5 mph
    // Time = distance / speed * 60 (to convert to minutes)
    return static_cast<int>(distance / 3.5 * 60);
}

// Find shortest path between two buildings
void EagleMaps::findShortestPath() {
    cout << "\n===== FIND SHORTEST PATH =====\n";
    cout << "Select starting building:\n";
    int startIdx = getBuildingSelection();

    cout << "Select destination building:\n";
    int endIdx = getBuildingSelection();

    string startName = buildingNames[startIdx];
    string endName = buildingNames[endIdx];

    if (startName == endName) {
        cout << "Start and destination buildings are the same.\n";
        return;
    }

    Node* startNode = campusMap.nameToNode[startName];
    Node* endNode = campusMap.nameToNode[endName];

    campusMap.dijkstra(startNode);
    vector<string> path = campusMap.getPath(endNode);

    cout << "\nShortest path from " << startName << " to " << endName << ":\n";
    cout << "Distance: " << fixed << setprecision(2) << endNode->distance << " miles\n";
    cout << "Estimated walking time: " << calculateWalkingTime(endNode->distance) << " minutes\n";

    cout << "Route: ";
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " → ";
        }
    }
    cout << endl;
}

// Plan a route for a schedule of multiple buildings
void EagleMaps::planScheduleRoute() {
    cout << "\n===== PLAN SCHEDULE ROUTE =====\n";
    cout << "How many buildings are in your schedule? ";
    int numBuildings;
    cin >> numBuildings;

    while (numBuildings < 2) {
        cout << "Please enter at least 2 buildings for a route: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> numBuildings;
    }

    vector<string> schedule;
    for (int i = 0; i < numBuildings; i++) {
        cout << "Select building " << (i + 1) << ":\n";
        int idx = getBuildingSelection();
        schedule.push_back(buildingNames[idx]);
    }

    cout << "\nPlanning route for your schedule...\n";
    campusMap.planSchedule(schedule, campusMap);
}

// Display all connections in the graph
void EagleMaps::displayMapConnections() {
    cout << "\n===== CAMPUS MAP CONNECTIONS =====\n";
    campusMap.displayGraph();
}

// Find buildings near a selected building
void EagleMaps::findNearbyBuildings() {
    cout << "\n===== FIND NEARBY BUILDINGS =====\n";
    cout << "Select reference building:\n";
    int buildingIdx = getBuildingSelection();

    cout << "Enter maximum distance (in miles): ";
    double maxDistance;
    cin >> maxDistance;

    string buildingName = buildingNames[buildingIdx];
    Node* buildingNode = campusMap.nameToNode[buildingName];

    campusMap.dijkstra(buildingNode);

    cout << "\nBuildings within " << fixed << setprecision(2) << maxDistance << " miles of " << buildingName << ":\n";

    vector<pair<string, double>> nearbyBuildings;
    for (Node* node : campusMap.nodes) {
        if (node != buildingNode && node->distance <= maxDistance) {
            nearbyBuildings.push_back({ node->node_name, node->distance });
        }
    }

    // Sort by distance
    sort(nearbyBuildings.begin(), nearbyBuildings.end(),
        [](const pair<string, double>& a, const pair<string, double>& b) {
            return a.second < b.second;
        });

    if (nearbyBuildings.empty()) {
        cout << "No buildings found within the specified distance.\n";
    }
    else {
        cout << left << setw(30) << "Building" << "Distance (miles)" << endl;
        cout << string(50, '-') << endl;

        for (const auto& building : nearbyBuildings) {
            cout << left << setw(30) << building.first
                << fixed << setprecision(2) << building.second << endl;
        }
    }
}

// Add or remove a building
void EagleMaps::modifyMap() {
    cout << "\n===== MODIFY CAMPUS MAP =====\n";
    cout << "1. Add a new building\n";
    cout << "2. Remove a building\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        addNewBuilding();
    }
    else if (choice == 2) {
        removeBuilding();
    }
    else {
        cout << "Invalid choice.\n";
    }
}

// Add a new building to the map
void EagleMaps::addNewBuilding() {
    string name;
    cout << "Enter the name of the new building: ";
    cin.ignore();
    getline(cin, name);

    // Check if building already exists
    if (campusMap.nameToNode.find(name) != campusMap.nameToNode.end()) {
        cout << "A building with this name already exists.\n";
        return;
    }

    // Create the new building
    Building newBuilding(name);

    // Add aliases to the new building
    cout << "Add aliases for " << name << ". Enter one at a time. Type 'done' when finished:\n";
    string alias;
    while (true) {
        cout << "Alias: ";
        getline(cin, alias);

        // Trim whitespace
        alias.erase(0, alias.find_first_not_of(" \t\n\r"));
        alias.erase(alias.find_last_not_of(" \t\n\r") + 1);

        if (alias == "done") break;
        if (!alias.empty()) {
            newBuilding.addAlias(alias); // assumes addAlias() is implemented
            cout << "Added alias: " << alias << "\n";
        }
    }
    
    Node* newBuilding = campusMap.addNode(name);
    buildingNames.push_back(name);

    cout << "Building added. Would you like to add connections? (y/n): ";
    char addConn;
    cin >> addConn;

    if (addConn == 'y' || addConn == 'Y') {
        int numConnections;
        cout << "How many connections would you like to add? ";
        cin >> numConnections;

        for (int i = 0; i < numConnections; i++) {
            cout << "Select building to connect to:\n";
            int idx = getBuildingSelection();
            Node* otherBuilding = campusMap.nameToNode[buildingNames[idx]];

            double distance;
            double time;
            cout << "Enter distance to " << buildingNames[idx] << " (in miles): ";
            cin >> distance;
            cout << "Enter time to " << buildingNames[idx] << " (in minutes): ";
            cin >> time;
            campusMap.addEdge(newBuilding, otherBuilding, distance, time);
        }
    }

    cout << "Building '" << name << "' added successfully.\n";
}

// Remove a building from the map
void EagleMaps::removeBuilding() {
    cout << "Select building to remove:\n";
    int idx = getBuildingSelection();
    string name = buildingNames[idx];

    campusMap.removeNode(name);
    buildingNames.erase(buildingNames.begin() + idx);

    cout << "Building '" << name << "' removed successfully.\n";
}

// Main application loop
void EagleMaps::run() {
    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            displayBuildings();
            break;
        case 2:
            findShortestPath();
            break;
        case 3:
            planScheduleRoute();
            break;
        case 4:
            displayMapConnections();
            break;
        case 5:
            findNearbyBuildings();
            break;
        case 6:
            modifyMap();
            break;
        case 0:
            cout << "Exiting Eagle Maps. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    } while (choice != 0);
}
