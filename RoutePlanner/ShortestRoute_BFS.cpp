/*
* Copyright (c) 2023 github.com/digitalsoul1
*
* This program is free software: you can redistribute it and/or modify
        * it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        * General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "ShortestRoute_BFS.h"

#include "RoutePlanner.h"
#include <queue>

std::vector<EVEIntelMonitor::SolarSystem *>
EVEIntelMonitor::ShortestRoute_BFS::GetRoute(const SolarSystem *start, const SolarSystem *end) {
    return bfs_search(start->getId(), end->getId());
}

std::vector<EVEIntelMonitor::SolarSystem *> EVEIntelMonitor::ShortestRoute_BFS::bfs_search(unsigned long start, unsigned long end) {
    auto solarSystems = RoutePlanner::getInstance()->getSolarSystemsMap()->getSolarSystems().lock();

    int adjacentList = static_cast<int>(solarSystems->size());
    std::queue<unsigned long> queue;
    std::vector<bool> visited(adjacentList);
    std::vector<int> pred(adjacentList);
    std::vector<int> dist(adjacentList);

    for (auto i = 0; i < adjacentList; i++) {
        visited[i] = false;
        pred[i] = -1;
        dist[i] = INT_MAX;
    }

    unsigned int startVertexIndex = getVertexIndex(start, solarSystems);
    unsigned int endVertexIndex = getVertexIndex(end, solarSystems);

    visited[startVertexIndex] = true;
    dist[startVertexIndex] = 0;
    queue.push(startVertexIndex);

    while (!queue.empty()) {
        unsigned long u = queue.front();
        queue.pop();
        for (size_t i = 0; i < solarSystems->at(u).getConnectedSystems().size(); i++) {
            unsigned long v = getVertexIndex(solarSystems->at(u).getConnectedSystems().at(i),
                                             solarSystems);
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                pred[v] = static_cast<int>(u);
                queue.push(v);

                if (v == endVertexIndex) {
                    std::vector<SolarSystem *> shortestPath;
                    unsigned long crawl = endVertexIndex;
                    while (pred[crawl] != -1) {
                        shortestPath.push_back(&solarSystems->at(crawl));
                        crawl = pred[crawl];
                    }

                    std::reverse(shortestPath.begin(), shortestPath.end());

                    return shortestPath;
                }
            }
        }
    }

    return {};
}


unsigned int EVEIntelMonitor::ShortestRoute_BFS::getVertexIndex(unsigned long id,
                                                                const std::shared_ptr<std::vector<SolarSystem>>& solarSystems) {
    for (size_t i = 0; i < solarSystems->size(); i++) {
        if (solarSystems->at(i).getId() == id) {
            return i;
        }
    }
    return -1;
}