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

#ifndef CELLINTERFACEO2_H
#define CELLINTERFACEO2_H

#include "../Order1/CellInterface.h"

class CellInterfaceO2 : public CellInterface
{
  public:
    CellInterfaceO2();
    CellInterfaceO2(int lvl); //Pour AMR
    ~CellInterfaceO2() override;

    void allocateSlopes(int& allocateSlopeLocal) override;
    void computeFlux(double& dtMax,
                     Limiter& globalLimiter,
                     Limiter& interfaceLimiter,
                     Limiter& globalVolumeFractionLimiter,
                     Limiter& interfaceVolumeFractionLimiter,
                     Prim type = vecPhases) override;
    /*!< Specific Riemann problem for 2nd order */
    void solveRiemann(double& /*dtMax*/,
                      Limiter& /*globalLimiter*/,
                      Limiter& /*interfaceLimiter*/,
                      Limiter& /*globalVolumeFractionLimiter*/,
                      Limiter& /*interfaceVolumeFractionLimiter*/,
                      Prim /*type*/ = vecPhases) override = 0;

    // -- Cartesian --
    void computeSlopes(Prim /*type*/ = vecPhases) override{};

    //Accessors
    Phase* getSlopesPhase(const int& /*phaseNumber*/) const override { return nullptr; };
    Mixture* getSlopesMixture() const override { return nullptr; };
    Transport* getSlopesTransport(const int& /*numberTransport*/) const override { return nullptr; };

    //For AMR method
    void creerCellInterfaceChild() override {}; /*!< Creer un child cell interface (non initialize) */
    void creerCellInterfaceChildInterne(const int& /*lvl*/, std::vector<CellInterface*>* /*childrenInternalCellInterfaces*/) override {
    }; /*!< Create intern child cell interface (uninitialized) */
};

extern Phase** slopesPhasesLocal1;
extern Phase** slopesPhasesLocal2;
extern Mixture* slopesMixtureLocal1;
extern Mixture* slopesMixtureLocal2;
extern double* slopesTransportLocal1;
extern double* slopesTransportLocal2;

#endif // CELLINTERFACEO2_H
