#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void portData(std::vector<std::string>& buildings, std::vector<std::vector<std::string>>& distances, std::vector<std::vector<std::string>>& walkingTimes) {

    std::ifstream file("Buildings.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open Buildings.txt\n";
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line[0] == '-') {
            break;
        }
        buildings.push_back(line);
    }

    int N = buildings.size();

    std::vector<std::string> flatDistances;
    std::vector<std::string> flatWalkingTimes;

    bool isDistance = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (isDistance)
            flatDistances.push_back(line);
        else
            flatWalkingTimes.push_back(line);

        isDistance = !isDistance;
    }

    file.close();

    distances.resize(N, std::vector<std::string>(N));
    walkingTimes.resize(N, std::vector<std::string>(N));

    int index = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            distances[i][j] = flatDistances[index++];

    index = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            walkingTimes[i][j] = flatWalkingTimes[index++];
}

void updateDatabase(const std::vector<std::string>& buildings,
    const std::vector<std::vector<std::string>>& distances,
    const std::vector<std::vector<std::string>>& walkingTimes) {

    std::ofstream file("Buildings.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open Buildings.txt for writing\n";
        return;
    }

    for (const std::string& building : buildings) {
        file << building << "\n";
    }

    file << "--done---\n";

    int N = buildings.size();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            file << distances[i][j] << "\n";
            file << walkingTimes[i][j] << "\n";
        }
    }

    file.close();
}

void addBuilding(std::vector<std::string>& buildings,
    std::vector<std::vector<std::string>>& distances,
    std::vector<std::vector<std::string>>& walkingTimes) {

    std::string newBuilding;
    std::cout << "Enter the name of the new building: ";
    std::getline(std::cin, newBuilding);

    buildings.push_back(newBuilding);
    int N = buildings.size();

    for (auto& row : distances) {
        row.push_back("");
    }
    for (auto& row : walkingTimes) {
        row.push_back("");
    }

    distances.push_back(std::vector<std::string>(N));
    walkingTimes.push_back(std::vector<std::string>(N));

    for (int i = 0; i < N; ++i) {
        if (i == N - 1) {
            distances[N - 1][i] = "0";
            walkingTimes[N - 1][i] = "0";
            distances[i][N - 1] = "0";
            walkingTimes[i][N - 1] = "0";
        }
        else {
            std::string dist, timee;

            std::cout << "Enter distance between " << buildings[i] << " and " << newBuilding << ": ";
            std::getline(std::cin, dist);
            std::cout << "Enter walking time between " << buildings[i] << " and " << newBuilding << ": ";
            std::getline(std::cin, timee);

            distances[i][N - 1] = dist;
            distances[N - 1][i] = dist;
            walkingTimes[i][N - 1] = timee;
            walkingTimes[N - 1][i] = timee;
        }
    }

    updateDatabase(buildings, distances, walkingTimes);
}

void removeBuilding(std::vector<std::string>& buildings,
    std::vector<std::vector<std::string>>& distances,
    std::vector<std::vector<std::string>>& walkingTimes) {

    if (buildings.empty()) {
        std::cout << "No buildings to remove.\n";
        return;
    }

    std::cout << "Current buildings:\n";
    for (size_t i = 0; i < buildings.size(); ++i) {
        std::cout << i << ": " << buildings[i] << "\n";
    }

    std::cout << "Enter the index of the building to remove: ";
    int index;
    std::cin >> index;
    std::cin.ignore();

    if (index < 0 || index >= static_cast<int>(buildings.size())) {
        std::cout << "Invalid index.\n";
        return;
    }

    buildings.erase(buildings.begin() + index);

    distances.erase(distances.begin() + index);
    walkingTimes.erase(walkingTimes.begin() + index);

    for (auto& row : distances) {
        row.erase(row.begin() + index);
    }
    for (auto& row : walkingTimes) {
        row.erase(row.begin() + index);
    }

    std::cout << "Building removed successfully.\n";

    updateDatabase(buildings, distances, walkingTimes);
}

int main() {

    std::vector<std::string> buildings;
    std::vector<std::vector<std::string>> distances;
    std::vector<std::vector<std::string>> walkingTimes;

    portData(buildings, distances, walkingTimes);


    std::cout << "1: Add a new building\n";
    std::cout << "2: Remove a building\n";
    std::cout << "3: Exit\n";
    std::cout << "Enter your choice (1-2): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        addBuilding(buildings, distances, walkingTimes);
    }
    else if (choice == 2) {
        removeBuilding(buildings, distances, walkingTimes);
    }
    else {
        std::cout << "Invalid choice, please try again.\n";
    }




    return 0;
}