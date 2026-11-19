#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

// Point structure representing spatial coordinates
struct Point {
    int id;
    double x, y;

    // Default constructor
    Point() : id(-1), x(0.0), y(0.0) {}

    // Constructor for target points (just coordinates)
    Point(double px, double py) : id(-1), x(px), y(py) {}

    // Full constructor for dataset points (ID + coordinates)
    Point(int pid, double px, double py) : id(pid), x(px), y(py) {}
};

// Bounding Box for Quadtree spatial partitioning
struct BoundingBox {
    double xMin, yMin, xMax, yMax;

    BoundingBox(double x0, double y0, double x1, double y1)
        : xMin(x0), yMin(y0), xMax(x1), yMax(y1) {}

    bool contains(const Point& p) const {
        return (p.x >= xMin && p.x <= xMax && p.y >= yMin && p.y <= yMax);
    }
};

// Quadtree Node Class
class Quadtree {
private:
    BoundingBox boundary;
    int capacity;
    std::vector<Point> points;
    bool divided;

    Quadtree* northwest;
    Quadtree* northeast;
    Quadtree* southwest;
    Quadtree* southeast;

    void subdivide() {
        double xMid = (boundary.xMin + boundary.xMax) / 2.0;
        double yMid = (boundary.yMin + boundary.yMax) / 2.0;

        northwest = new Quadtree(BoundingBox(boundary.xMin, yMid, xMid, boundary.yMax), capacity);
        northeast = new Quadtree(BoundingBox(xMid, yMid, boundary.xMax, boundary.yMax), capacity);
        southwest = new Quadtree(BoundingBox(boundary.xMin, boundary.yMin, xMid, yMid), capacity);
        southeast = new Quadtree(BoundingBox(xMid, boundary.yMin, boundary.xMax, yMid), capacity);

        divided = true;
    }

public:
    Quadtree(BoundingBox box, int cap)
        : boundary(box), capacity(cap), divided(false),
          northwest(nullptr), northeast(nullptr), southwest(nullptr), southeast(nullptr) {}

    ~Quadtree() {
        if (divided) {
            delete northwest;
            delete northeast;
            delete southwest;
            delete southeast;
        }
    }

    bool insert(const Point& p) {
        if (!boundary.contains(p)) return false;

        if (points.size() < (size_t)capacity) {
            points.push_back(p);
            return true;
        }

        if (!divided) {
            subdivide();
        }

        if (northwest->insert(p)) return true;
        if (northeast->insert(p)) return true;
        if (southwest->insert(p)) return true;
        if (southeast->insert(p)) return true;

        return false;
    }

    // k-Nearest Neighbor search
    void findNearest(const Point& target, Point& bestMatch, double& minSqDist) const {
        // Check points in current node
        for (const auto& p : points) {
            double dx = p.x - target.x;
            double dy = p.y - target.y;
            double distSq = (dx * dx) + (dy * dy);

            std::cout << "[TEST] Evaluating Point ID " << p.id << " at (" << p.x << ", " << p.y << ") -> DistSq: " << distSq << "\n";

            if (distSq < minSqDist) {
                minSqDist = distSq;
                bestMatch = p;
                std::cout << "  --> NEW CHAMPION! ID " << p.id << " with DistSq: " << minSqDist << "\n";
            }
        }

        if (!divided) return;

        // Recursively search child quadrants
        northwest->findNearest(target, bestMatch, minSqDist);
        northeast->findNearest(target, bestMatch, minSqDist);
        southwest->findNearest(target, bestMatch, minSqDist);
        southeast->findNearest(target, bestMatch, minSqDist);
    }
};

#endif