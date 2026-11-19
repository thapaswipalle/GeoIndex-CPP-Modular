#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include "Quadtree.hpp"

// Helper function to safely load points from CSV and skip headers
std::vector<Point> loadPointsFromCSV(const std::string& filename) {
    std::vector<Point> pts;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open " << filename << "\n";
        return pts;
    }
    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        // Skip header row
        if (isHeader) {
            isHeader = false;
            continue;
        }

        try {
            std::stringstream ss(line);
            std::string segment;
            int id;
            double x, y;
            
            if (std::getline(ss, segment, ',')) id = std::stoi(segment);
            if (std::getline(ss, segment, ',')) x = std::stod(segment);
            if (std::getline(ss, segment, ',')) y = std::stod(segment);

            pts.emplace_back(id, x, y);
        } catch (const std::exception& e) {
            continue;
        }
    }
    std::cout << "[SUCCESS] Loaded " << pts.size() << " points from " << filename << "\n";
    return pts;
}

int main() {
    std::cout << ">>> GEOQUERY K-NN ENGINE INITIALIZED <<<\n\n";

    // Widen domain bounds to include Point 5 at (-2000, -1500)
    BoundingBox domain(-3000.0, -3000.0, 3000.0, 3000.0);
    Quadtree tree(domain, 4);
    // 2. Load points from points.csv
    std::vector<Point> points = loadPointsFromCSV("points.csv");
    if (points.empty()) {
        std::cerr << "[ERROR] No points loaded. Check points.csv path.\n";
        return 1;
    }

    // 3. Insert points into Quadtree
    for (const auto& p : points) {
        tree.insert(p);
    }
    std::cout << "[SUCCESS] All CSV points inserted into Quadtree index!\n\n";

    // 4. Take interactive terminal input for target coordinates
    double targetX, targetY;
    std::cout << "----------------------------------------\n";
    std::cout << " Enter target X coordinate: ";
    std::cin >> targetX;
    std::cout << " Enter target Y coordinate: ";
    std::cin >> targetY;
    std::cout << "----------------------------------------\n\n";

    Point target(targetX, targetY);
    Point bestMatch;
    double minSqDist = std::numeric_limits<double>::max();

    // 5. Run k-NN Search
    tree.findNearest(target, bestMatch, minSqDist);

    // 6. Print final verified results
    std::cout << "\n--- k-NN Search Results ---\n";
    std::cout << "Target Location: (" << target.x << ", " << target.y << ")\n";
    std::cout << "Nearest Point Found -> ID: " << bestMatch.id 
              << " at (" << bestMatch.x << ", " << bestMatch.y << ")\n";

    return 0;
}