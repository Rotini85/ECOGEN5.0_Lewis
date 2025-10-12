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

#ifndef CELLINTERFACEO2NS_H
#define CELLINTERFACEO2NS_H

#include "CellInterfaceO2.h"

class CellInterfaceO2NS : public CellInterfaceO2
{
  public:
    CellInterfaceO2NS();
    ~CellInterfaceO2NS() override;

    void solveRiemann(double& dtMax,
                      Limiter& /*globalLimiter*/,
                      Limiter& /*interfaceLimiter*/,
                      Limiter& /*globalVolumeFractionLimiter*/,
                      Limiter& /*interfaceVolumeFractionLimiter*/,
                      Prim type = vecPhases) override; /*!< probleme de Riemann special ordre 2 */
};

#endif
