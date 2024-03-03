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
#ifndef ROUTEPLANNER_SOLARSYSTEMSMAP_H
#define ROUTEPLANNER_SOLARSYSTEMSMAP_H

#include <vector>
#include "SolarSystem.h"

namespace EVEIntelMonitor {
    class SolarSystemsMap {
    public:
        SolarSystemsMap();
        bool addSolarSystem(SolarSystem &&solarSystem);

        SolarSystem *getSolarSystem(unsigned long id);

        SolarSystem *getSolarSystem(const std::string &name);

        std::weak_ptr<std::vector<SolarSystem>> getSolarSystems();

        [[nodiscard]] int getSolarSystemCount() const;

        [[maybe_unused]] void dumpToTerminal();

        [[maybe_unused]] void clearMap();

        ~SolarSystemsMap();

    private:
        std::shared_ptr<std::vector<SolarSystem>> m_SolarSystems;
    };

}
#endif //ROUTEPLANNER_SOLARSYSTEMSMAP_H
