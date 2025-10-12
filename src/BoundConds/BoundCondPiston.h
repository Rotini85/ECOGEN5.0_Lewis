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

#ifndef BOUNDCONDPISTON_H
#define BOUNDCONDPISTON_H

#include "BoundCond.h"

class BoundCondPiston : public BoundCond
{
  public:
    BoundCondPiston(const BoundCondPiston& Source, const int& lvl = 0); //Copy ctor (useful for AMR)
    BoundCondPiston(int numPhysique, tinyxml2::XMLElement* element, std::string fileName);
    BoundCondPiston(int numPhysique);
    ~BoundCondPiston() override;

    void createBoundary(TypeMeshContainer<CellInterface*>& cellInterfaces) override;
    void solveRiemannBoundary(Cell& cellLeft, const double& dxLeft, double& dtMax) override;
    void solveRiemannTransportBoundary(Cell& cellLeft) const override;

    int whoAmI() const override { return PISTON; };
    int whoAmIHeat() const override { return m_heatCondition; }
    bool isMRFWall() const override { return m_isMRFWall; }

    double getBoundaryHeatQuantity() const override { return m_imposedHeatQuantity; }
    Coord& getWallRotationalVelocityMRF() override { return m_omegaWall; }

    //For AMR method
    void creerCellInterfaceChild() override; /*!< Create a child cell interface (not initialized) */

  protected:
    TypeBCHeat m_heatCondition;   //!< Specific heat boundary condition, could be imposed temperature or flux density (default is adiabatic)
    double m_imposedHeatQuantity; //!< Imposed heat quantity on the boundary. Depending on input (m_heatCondition) could be imposed
                                  //!< temperature or flux density. This option requires conductivity additionnal physics to work
    bool m_isMRFWall;             //!< Flag to check if a special boundary treatment is required with MRF
    Coord m_omegaWall;            //!< Rotational boundary velocity for MRF computation with viscous additionnal physic
    double m_uPiston;             //!< Velocity of the piston
  private:
};

#endif // BOUNDCONDPISTON_H
