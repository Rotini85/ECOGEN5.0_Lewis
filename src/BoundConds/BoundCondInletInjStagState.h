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

#ifndef BOUNDCONDINLETINJSTAGSTATE_H
#define BOUNDCONDINLETINJSTAGSTATE_H

#include "BoundCond.h"

class BoundCondInletInjStagState : public BoundCond
{
  public:
    BoundCondInletInjStagState(int numPhysique,
                               tinyxml2::XMLElement* element,
                               const int& numbPhases,
                               const int& numbTransports,
                               std::vector<std::string> nameTransports,
                               Eos** eos,
                               std::string fileName = "Unknown file");
    BoundCondInletInjStagState(const BoundCondInletInjStagState& Source, const int& lvl = 0); //Copy ctor (useful for AMR)
    ~BoundCondInletInjStagState() override;

    void createBoundary(TypeMeshContainer<CellInterface*>& cellInterfaces) override;
    void solveRiemannBoundary(Cell& cellLeft, const double& dxLeft, double& dtMax) override;
    void solveRiemannTransportBoundary(Cell& cellLeft) const override;

    int whoAmI() const override { return INLETINJSTAGSTATE; };
    void printInfo() override;

    //For AMR method
    void creerCellInterfaceChild() override; //!< Create a child cell interface (not initialized) */

  protected:
  private:
    double m_m0; //!< Specific massflow (kg.s-1.m-2)
    double m_T0;
    double* m_ak0;
    double* m_rhok0;
    double* m_pk0;
    double* m_valueTransport;
};

#endif
