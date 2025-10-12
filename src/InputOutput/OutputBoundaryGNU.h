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

#ifndef OutputBoundaryGNU_H
#define OutputBoundaryGNU_H

#include "OutputGNU.h"

class OutputBoundaryGNU : public OutputGNU
{
  public:
    OutputBoundaryGNU(std::string casTest, std::string run, tinyxml2::XMLElement* element, std::string fileName, Input* entree);
    ~OutputBoundaryGNU() override;

    void initializeSpecificOutput(std::vector<CellInterface*>* cellInterfacesLvl) override;
    virtual void initializeSpecificOutputBound() { Errors::errorMessage("initializeSpecificOutputBound not defined for required output"); };
    void writeResults(std::vector<CellInterface*>* /*cellInterfacesLvl*/) override
    {
      Errors::errorMessage("writeResults not defined for required output");
    };

    // Accessors
    double getNextTime() override { return m_nextAcq; };

  protected:
    int m_numPhys;                           //!< Physical number of the boundary to record
    std::vector<int> m_cellInterfaceIndexes; //!< Indexes of cellInterfaces on the boundary (speed up searching process)

    double m_acqFreq; //!< Acquisition time frequency
    double m_nextAcq; //!< Next acquisition time
};

#endif // OutputBoundaryGNU_H
