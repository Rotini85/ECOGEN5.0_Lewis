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

#ifndef CELLO2_H
#define CELLO2_H

#include "../Order1/Cell.h"
#include "CellInterfaceO2.h"

class CellO2 : public Cell
{
  public:
    CellO2();
    CellO2(int lvl); //Pour AMR
    ~CellO2() override;
    void allocate(const std::vector<AddPhys*>& addPhys) override;
    void copyPhase(const int& phaseNumber, Phase* phase) override;
    void saveCons() override;
    void getBackCons() override;
    void predictionOrdre2(const double& dt, Symmetry* symmetry) override;
    void correctionEnergy(Prim type = vecPhases) override;
    void fulfillState(Prim type = vecPhases) override;

    //Set/Get
    Phase* getPhase(const int& phaseNumber, Prim type = vecPhases) const override;
    Phase** getPhases(Prim type = vecPhases) const override;
    Mixture* getMixture(Prim type = vecPhases) const override;
    Transport& getTransport(const int& numTransport, Prim type = vecPhases) const override;
    Transport* getTransports(Prim type = vecPhases) const override;
    void setTransport(double value, int& numTransport, Prim type = vecPhases) override;

    // For 2nd order with parallel
    void getBufferSlopes(double* /*buffer*/, int& /*counter*/, const int& /*lvl*/) override
    {
      Errors::errorMessage("getBufferSlopes not available for CellO2");
    };
    void fillBufferSlopes(double* /*buffer*/, int& /*counter*/, const int& /*lvl*/, const int& /*neighbour*/) const override
    {
      Errors::errorMessage("fillBufferSlopes not available for CellO2");
    };

    // -- Cartesian --
    void computeLocalSlopes(CellInterface& /*cellInterfaceRef*/,
                            Limiter& /*globalLimiter*/,
                            Limiter& /*interfaceLimiter*/,
                            Limiter& /*globalVolumeFractionLimiter*/,
                            Limiter& /*interfaceVolumeFractionLimiter*/,
                            double& /*alphaCellAfterOppositeSide*/,
                            double& /*alphaCell*/,
                            double& /*alphaCellOtherInterfaceSide*/,
                            double& /*epsInterface*/) override {};

    void computeLocalSlopesLimite(CellInterface& /*cellInterfaceRef*/,
                                  Limiter& /*globalLimiter*/,
                                  Limiter& /*interfaceLimiter*/,
                                  Limiter& /*globalVolumeFractionLimiter*/,
                                  Limiter& /*interfaceVolumeFractionLimiter*/,
                                  double& /*epsInterface*/) override {};

    //Pour methode AMR
    void createChildCell(const int& /*lvl*/) override {}; /*!< Create a child cell (non initialized) */

    // -- NS --
    GradPhase* getGradPhase(const int& /*phaseNumber*/) const override
    {
      Errors::errorMessage("getGradPhase not available for Cell");
      return nullptr;
    };
    GradMixture* getGradMixture() const override
    {
      Errors::errorMessage("getGradMixture not available for Cell");
      return nullptr;
    };
    GradTransport* getGradTransport(const int& /*transportNumber*/) const override
    {
      Errors::errorMessage("getGradTransport not available for Cell");
      return nullptr;
    };
    void allocateSecondOrderBuffersAndGradientVectors(Phase** /*phases*/, Mixture* /*mixture*/) override {};
    void computeGradientsO2() override {};
    void limitGradientsO2(Limiter& /*globalLimiter*/) override {};

    void computeLocalSlopes(CellInterface& /*cellInterfaceRef*/) override {};

  protected:
    Phase** m_vecPhasesO2;        /*!< pour stocker les values predites a l ordre 2 */
    Mixture* m_mixtureO2;         /*!< pour stocker les values predites a l ordre 2 */
    Transport* m_vecTransportsO2; /*!< pour stocker les values predites a l ordre 2 */
    Flux* m_consSauvegarde;       /*!< Vector de save des variables conservatives. De type flux car recueille la sum des flux sur l objet cell */
    Transport* m_consTransportsSauvegarde; /*!< Vector de saugevarde des grandeurs passives
                                                permettant de recueillir la sum des flux des grandeurs transportees */

  private:
};

#endif // CELLO2_H
