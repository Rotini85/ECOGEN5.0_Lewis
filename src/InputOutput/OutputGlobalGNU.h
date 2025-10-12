//
//       ,---.     ,--,    .---.     ,--,    ,---.    .-. .-.
//       | .-'   .' .')   / .-. )  .' .'     | .-'    |  \| |
//       | `-.   |  |(_)  | | |(_) |  |  __  | `-.    |   | |
//       | .-'   \  \     | | | |  \  \ ( _) | .-'    | |\  |
//       |  `--.  \  `-.  \ `-' /   \  `-) ) |  `--.  | | |)|
//       /( __.'   \____\  )---'    )\____/  /( __.'  /(  (_)
//      (__)              (_)      (__)     (__)     (__)
//      Official webSite: https://code-mphi.github.io/ECOGEN/
//
//  This file is part of ECOGEN.
//
//  ECOGEN is the legal property of its developers, whose names
//  are listed in the copyright file included with this source
//  distribution.
//
//  ECOGEN is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published
//  by the Free Software Foundation, either version 3 of the License,
//  or (at your option) any later version.
//
//  ECOGEN is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ECOGEN (file LICENSE).
//  If not, see <http://www.gnu.org/licenses/>.

#ifndef OUTPUTGLOBALGNU_H
#define OUTPUTGLOBALGNU_H

#include "OutputGNU.h"

class OutputGlobalGNU : public OutputGNU
{
  public:
    OutputGlobalGNU();
    OutputGlobalGNU(std::string casTest, std::string run, tinyxml2::XMLElement* element, Input* entree, std::string quantity);
    ~OutputGlobalGNU() override;

    void initializeSpecificOutput() override;

    void writeResults(Mesh* /*mesh*/, std::vector<Cell*>* cellsLvl) override;

  protected:
    double m_quantity; //!< Physical quantity recorded (mass or total energy)

    void extractTotalQuantity(std::vector<Cell*>* cellsLvl);
};

#endif //OUTPUTGLOBALGNU_H
