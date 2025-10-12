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

#ifndef BOUNDCONDWALLO2NS_H
#define BOUNDCONDWALLO2NS_H

#include "BoundCondWall.h"

class BoundCondWallO2NS : public BoundCondWall
{
  public:
    BoundCondWallO2NS(const BoundCondWallO2NS& Source, const int& lvl = 0); //Copy ctor (useful for AMR)
    BoundCondWallO2NS(int numPhysique, tinyxml2::XMLElement* element, std::string fileName);
    BoundCondWallO2NS(int numPhysique);
    ~BoundCondWallO2NS() override;

    void createBoundary(TypeMeshContainer<CellInterface*>& cellInterfaces) override;
    void solveRiemann(double& dtMax,
                      Limiter& /*globalLimiter*/,
                      Limiter& /*interfaceLimiter*/,
                      Limiter& /*globalVolumeFractionLimiter*/,
                      Limiter& /*interfaceVolumeFractionLimiter*/,
                      Prim type = vecPhases) override;

    int whoAmI() const override { return WALL; };
};

#endif // BOUNDCONDWALLO2NS_H
