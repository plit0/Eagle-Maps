#include "api_server.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "httplib.h" 
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Rest of the code remains the same as in the original file

// Constructor
APIServer::APIServer(Graph& map) : campusMap(map) {
    // Populate building names from the campus map
    for (Node* node : campusMap.nodes) {
        buildingNames.push_back(node->node_name);
    }
}

// Method to start the server
void APIServer::startServer(int port) {
    std::cout << "Starting Eagle Maps API Server on port " << port << std::endl;
    
    // Create HTTP server
    httplib::Server svr;
    
    // Enable CORS (important for web apps)
    svr.set_base_dir("./public"); // Serve static files from this directory
    
    // Define CORS headers middleware
    auto enableCors = [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };
    
    // Add middleware to all responses
    svr.set_logger([&](const httplib::Request& req, const httplib::Response& res) {
        std::cout << "[" << req.method << "] " << req.path << " -> " << res.status << std::endl;
    });
    
    // GET /buildings - Get all buildings
    svr.Get("/buildings", [&](const httplib::Request& req, httplib::Response& res) {
        enableCors(req, res);
        
        json j = buildingNames;
        res.set_content(j.dump(), "application/json");
    });
    
    // GET /path - Find path between two buildings
    svr.Get("/path", [&](const httplib::Request& req, httplib::Response& res) {
        enableCors(req, res);
        
        // Get query parameters
        if (!req.has_param("start") || !req.has_param("end")) {
            res.status = 400; // Bad Request
            res.set_content("{\"error\":\"Missing start or end parameters\"}", "application/json");
            return;
        }
        
        std::string startBuilding = req.get_param_value("start");
        std::string endBuilding = req.get_param_value("end");
        
        // Check if buildings exist
        if (!buildingExists(startBuilding) || !buildingExists(endBuilding)) {
            res.status = 404; // Not Found
            res.set_content("{\"error\":\"One or both buildings do not exist\"}", "application/json");
            return;
        }
        
        // Find the nodes for start and end buildings
        Node* startNode = campusMap.nameToNode[startBuilding];
        Node* endNode = campusMap.nameToNode[endBuilding];
        
        // Run Dijkstra's algorithm
        campusMap.dijkstra(startNode);
        
        // Get the path
        std::vector<std::string> path = campusMap.getPath(endNode);
        
        // Return as JSON
        json j = path;
        res.set_content(j.dump(), "application/json");
    });
    
    // GET /nearby - Find nearby buildings
    svr.Get("/nearby", [&](const httplib::Request& req, httplib::Response& res) {
        enableCors(req, res);
        
        // Get query parameters
        if (!req.has_param("building") || !req.has_param("distance")) {
            res.status = 400; // Bad Request
            res.set_content("{\"error\":\"Missing building or distance parameters\"}", "application/json");
            return;
        }
        
        std::string referenceBuilding = req.get_param_value("building");
        double maxDistance = std::stod(req.get_param_value("distance"));
        
        // Check if reference building exists
        if (!buildingExists(referenceBuilding)) {
            res.status = 404; // Not Found
            res.set_content("{\"error\":\"Reference building does not exist\"}", "application/json");
            return;
        }
        
        // Find the reference node
        Node* referenceNode = campusMap.nameToNode[referenceBuilding];
        
        // Run Dijkstra's algorithm
        campusMap.dijkstra(referenceNode);
        
        // Find nearby buildings
        json nearbyBuildings = json::array();
        for (Node* node : campusMap.nodes) {
            if (node != referenceNode && node->distance <= maxDistance) {
                json building;
                building["name"] = node->node_name;
                building["distance"] = node->distance;
                nearbyBuildings.push_back(building);
            }
        }
        
        // Sort by distance
        std::sort(nearbyBuildings.begin(), nearbyBuildings.end(), 
            [](const json& a, const json& b) {
                return a["distance"] < b["distance"];
            }
        );
        
        res.set_content(nearbyBuildings.dump(), "application/json");
    });
    
    // Listen on the specified port
    std::cout << "Server listening on port " << port << std::endl;
    svr.listen("0.0.0.0", port);
}

// Helper method to check if a building exists
bool APIServer::buildingExists(const std::string& buildingName) {
    return campusMap.nameToNode.find(buildingName) != campusMap.nameToNode.end();
}

// Get connected buildings for a given building
std::vector<std::string> APIServer::getConnectedBuildings(const std::string& buildingName) {
    std::vector<std::string> connectedBuildings;
    
    if (!buildingExists(buildingName)) {
        return connectedBuildings;
    }

    Node* building = campusMap.nameToNode[buildingName];
    for (Node* neighbor : building->neighbors) {
        connectedBuildings.push_back(neighbor->node_name);
    }

    return connectedBuildings;
}

// Convert path to a displayable string
std::string APIServer::convertPathToString(const std::vector<std::string>& path) {
    if (path.empty()) return "No path found";

    std::stringstream ss;
    for (size_t i = 0; i < path.size(); ++i) {
        ss << path[i];
        if (i < path.size() - 1) {
            ss << " → ";
        }
    }
    return ss.str();
}

// Error logging method
void APIServer::logError(const std::string& errorMessage) {
    std::cerr << "Error: " << errorMessage << std::endl;
}

// The remaining methods don't need to be modified as they're used internally
void APIServer::handleGetBuildings() {
    std::cout << "Retrieving all buildings:" << std::endl;
    for (const auto& building : buildingNames) {
        std::cout << building << std::endl;
    }
}

void APIServer::handleFindPath(const std::string& startBuilding, const std::string& endBuilding) {
    // Implementation remains the same (used for CLI)
    // Check if buildings exist
    if (!buildingExists(startBuilding) || !buildingExists(endBuilding)) {
        logError("One or both buildings do not exist");
        return;
    }

    // Find the nodes for start and end buildings
    Node* startNode = campusMap.nameToNode[startBuilding];
    Node* endNode = campusMap.nameToNode[endBuilding];

    // Run Dijkstra's algorithm
    campusMap.dijkstra(startNode);

    // Get the path
    std::vector<std::string> path = campusMap.getPath(endNode);

    // Convert path to a displayable string
    std::string pathString = convertPathToString(path);

    std::cout << "Path from " << startBuilding << " to " << endBuilding << ":" << std::endl;
    std::cout << pathString << std::endl;
    std::cout << "Total distance: " << endNode->distance << " miles" << std::endl;
}

void APIServer::handleFindNearbyBuildings(const std::string& referenceBuilding, double maxDistance) {
    // Implementation remains the same (used for CLI)
    // Check if reference building exists
    if (!buildingExists(referenceBuilding)) {
        logError("Reference building does not exist");
        return;
    }

    // Find the reference node
    Node* referenceNode = campusMap.nameToNode[referenceBuilding];

    // Run Dijkstra's algorithm
    campusMap.dijkstra(referenceNode);

    std::cout << "Nearby buildings from " << referenceBuilding << " within " 
              << maxDistance << " miles:" << std::endl;

    // Find and display nearby buildings
    for (Node* node : campusMap.nodes) {
        if (node != referenceNode && node->distance <= maxDistance) {
            std::cout << node->node_name << " (Distance: " 
                      << node->distance << " miles)" << std::endl;
        }
    }
}

void APIServer::handleAddBuilding(const std::string& buildingName) {
    // Implementation remains the same (used for CLI)
    // Check if building already exists
    if (buildingExists(buildingName)) {
        logError("Building already exists");
        return;
    }

    // Add the new building to the campus map
    Node* newBuilding = campusMap.addNode(buildingName);
    buildingNames.push_back(buildingName);

    std::cout << "Added new building: " << buildingName << std::endl;
}

void APIServer::handleRemoveBuilding(const std::string& buildingName) {
    // Implementation remains the same (used for CLI)
    // Check if building exists
    if (!buildingExists(buildingName)) {
        logError("Building does not exist");
        return;
    }

    // Remove the building from the campus map
    campusMap.removeNode(buildingName);

    // Remove from building names
    auto it = std::find(buildingNames.begin(), buildingNames.end(), buildingName);
    if (it != buildingNames.end()) {
        buildingNames.erase(it);
    }

    std::cout << "Removed building: " << buildingName << std::endl;
}