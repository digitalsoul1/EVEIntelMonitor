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
#pragma once
#include "SolarSystem.h"

namespace EVEIntelMonitor {

    class SolarSystem;
    class ShortestRoute_BFS {
    public:
        static std::vector<SolarSystem *> GetRoute(const SolarSystem *start, const SolarSystem *end);

    private:
        static std::vector<SolarSystem *> bfs_search(unsigned long start, unsigned long end);
        static unsigned int getVertexIndex(unsigned long id, const std::shared_ptr<std::vector<SolarSystem>>& solarSystems);
    };
}
