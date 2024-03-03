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

#include "SolarSystemsMap.h"
#include "ShortestRoute_BFS.h"

namespace EVEIntelMonitor {

    class RoutePlanner {
    public:
        std::vector<SolarSystem *> getJumpsBetween(const std::string &start, const std::string &end);
        std::vector<SolarSystem*> getJumpsBetween(unsigned long start, unsigned long end);
        bool solarSystemExists(const std::string &name);
        [[nodiscard]] std::string getSolarSystemName(unsigned long id) const;
        [[nodiscard]] unsigned long getSolarSystemId(const std::string& solarSystemName);
        void loadSolarSystems();
        static RoutePlanner *getInstance();
        [[nodiscard]] SolarSystemsMap *getSolarSystemsMap() const;

    private:
        RoutePlanner() = default;
        void Initialize();
        bool m_bInitialized = false;
        std::unique_ptr<SolarSystemsMap> m_SolarSystemsMap;
        static RoutePlanner *m_Instance;
    };

}
