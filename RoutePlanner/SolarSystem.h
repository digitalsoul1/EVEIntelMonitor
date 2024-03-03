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
#ifndef ROUTEPLANNER_EVE_SOLARSYSTEM_H
#define ROUTEPLANNER_EVE_SOLARSYSTEM_H

#include <string>
#include <vector>

namespace EVEIntelMonitor {
    class SolarSystem {
    public:
        SolarSystem(unsigned long id, std::string name, float security);

        SolarSystem() = default;

        [[nodiscard]] unsigned long getId() const;

        std::string getName();

        [[nodiscard]] float getSecurity() const;

        std::vector<unsigned long> getConnectedSystems();


        void setId(unsigned long id);

        void setName(std::string name);

        void setSecurity(float security);


        void addConnectedSystem(unsigned long connectedSystem);


    private:
        unsigned long m_id;
        std::string m_float;
        float m_security;
        std::vector<unsigned long> m_connectedSystems = {};
    };

}
#endif //ROUTEPLANNER_EVE_SOLARSYSTEM_H
