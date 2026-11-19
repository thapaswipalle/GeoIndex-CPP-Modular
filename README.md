# GeoIndex-CPP-Modular

Modular, header-based C++17 quadtree spatial indexing engine designed for reusable integration and CSV-backed spatial dataset querying.

## Project Structure
- **`Quadtree.hpp`:** Reusable header-only quadtree implementation with recursive 2D axis-aligned bounding box (AABB) partitioning and spatial range query support.
- **`main.cpp`:** Driver program demonstrating dataset ingestion, quadtree construction, and spatial query execution.
- **`points.csv`:** Sample 2D spatial coordinate dataset for indexing and benchmarking.

## Build & Run
```bash
g++ -std=c++17 -O2 main.cpp -o geo_modular
./geo_modular
