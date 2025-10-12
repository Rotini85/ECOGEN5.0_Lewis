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

#ifndef CELLINTERFACEO2CARTESIAN_H
#define CELLINTERFACEO2CARTESIAN_H

#include "CellInterfaceO2.h"

class CellInterfaceO2Cartesian : public CellInterfaceO2
{
  public:
    CellInterfaceO2Cartesian();
    CellInterfaceO2Cartesian(int lvl);
    ~CellInterfaceO2Cartesian() override;

    void allocateSlopes(int& allocateSlopeLocal) override;
    void computeSlopes(Prim type = vecPhases) override;
    /*!< Specific Riemann problem for 2nd order */
    void solveRiemann(double& dtMax,
                      Limiter& globalLimiter,
                      Limiter& interfaceLimiter,
                      Limiter& globalVolumeFractionLimiter,
                      Limiter& interfaceVolumeFractionLimiter,
                      Prim type = vecPhases) override;

    //Accessors
    Phase* getSlopesPhase(const int& phaseNumber) const override;
    Mixture* getSlopesMixture() const override;
    Transport* getSlopesTransport(const int& numberTransport) const override;

    //For AMR method
    void creerCellInterfaceChild() override; /*!< Creer un child cell interface (non initialize) */
    void creerCellInterfaceChildInterne(const int& lvl, std::vector<CellInterface*>* childrenInternalCellInterfaces)
      override; /*!< Creer un intern child cell interface (non initialize) */

  protected:
    Phase** m_vecPhasesSlopes;        /*!< Model based array of phase slopes */
    Mixture* m_mixtureSlopes;         /*!< Model based mixture slopes */
    Transport* m_vecTransportsSlopes; /*!< Model based transport slopes */
};

#endif
