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

#ifndef MODEULERKORTEWEG_H
#define MODEULERKORTEWEG_H

#include "../Model.h"
#include "../../Order1/Cell.h"
#include "FluxEulerKorteweg.h"
#include "MixEulerKorteweg.h"

//! \class     ModEulerKorteweg
//! \brief     Model class for Augmented Euler--Korteweg mathematical system of equations (single phase)
class ModEulerKorteweg : public Model
{
  public:
    //! \brief     EulerKorteweg model constructor
    //! \param     numbTransports    number of additional transport equations
    ModEulerKorteweg(const int& numbTransports);
    //! \brief     EulerKorteweg model constructor
    //! \param     numbTransports       number of additional transport equations
    //! \param     alpha                parameter alpha of Euler-Korteweg equations
    //! \param     beta                 parameter beta of Euler-Korteweg equations
    //! \param     temperature          constant temperature of Euler-Korteweg equations
    //! \param     kappa                constant surface-tension coefficient of Euler-Korteweg equations
    ModEulerKorteweg(const int& numbTransports, const double& alpha, const double& beta, const double& temperature, const double& kappa);
    //! \brief     EulerKorteweg model constructor
    //! \param     name                 model name
    //! \param     numbTransports       number of additional transport equations
    //! \param     alpha                parameter alpha of Euler-Korteweg equations
    //! \param     beta                 parameter beta of Euler-Korteweg equations
    //! \param     temperature          constant temperature of Euler-Korteweg equations
    //! \param     kappa                constant surface-tension coefficient of Euler-Korteweg equations
    ModEulerKorteweg(
      const std::string& name, const int& numbTransports, const double& alpha, const double& beta, const double& temperature, const double& kappa);
    ~ModEulerKorteweg() override;

    void allocateCons(Flux** cons) override;
    void allocatePhase(Phase** phase) override;
    void allocateMixture(Mixture** mixture) override;

    //! \details    Does nothing for this model
    void fulfillState(Phase** /*phases*/, Mixture* /*mixture*/) override {};

    //! \details    Does nothing for this model
    void fulfillStateResume(Phase** /*phases*/, Mixture* /*mixture*/) override {};

    //! \brief     Complete the augmented variables (such as the ones of Euler-Korteweg model)
    //! \param     cell           cell
    void initializeAugmentedVariables(Cell* cell) override;

    //Fluid-flow Riemann solvers
    //--------------------------
    void solveRiemannIntern(Cell& cellLeft,
                            Cell& cellRight,
                            const double& dxLeft,
                            const double& dxRight,
                            double& dtMax,
                            std::vector<double>& /*boundData*/ = DEFAULT_VEC_INTERFACE_DATA) const override;

    void reverseProjection(const Coord normal, const Coord tangent, const Coord binormal) const override;

    //Accessors
    //---------
    //! \brief  Select a specific scalar variable
    //! \param  phases         phases array variables
    //! \param  mixture        mixture variables
    //! \param  vecTransports  vector of transport variables
    //! \param  nameVariables  Name of the variable to select
    //! \param  numPhases      Phases number's
    double selectScalar(Phase** phases, Mixture* /*mixture*/, Transport* transports, Variable nameVariable, int num = 0) const override;
    const double& getSM() override
    {
      Errors::errorMessage("getSM not available for required model");
      return Errors::defaultDouble;
    };
    const Coord& getVelocity(const Cell* cell) const override { return cell->getPhase(0)->getVelocity(); };
    Coord& getVelocity(Cell* cell) override { return cell->getPhase(0)->getVelocity(); };

    const std::string& whoAmI() const override { return m_name; };

    //Methods specific to Euler-Korteweg
    //----------------------------------
    virtual double kappa(const double& /*density*/) const;
    virtual double kappaPrime(const double& /*density*/) const;
    virtual double kappaSecond(const double& /*density*/) const;
    virtual double epsilonPrime(Cell& cell, const double& density) const;
    virtual double epsilonSecond(Cell& cell, const double& density) const;
    //! \brief     Compute maximal wave speed using Davis approximation and eigenvalues of hyperbolic equations
    //! \param     cellLeft     cell left
    //! \param     cellRight    cell right
    //! \param     rhoL         density left
    //! \param     rhoR         density right
    //! \param     uL           velocity left
    //! \param     uR           velocity right
    //! \param     etaL         eta left
    //! \param     etaR         eta right
    //! \param     vecPxL       x-component of vectorP left
    //! \param     vecPxR       x-component of vectorP right
    //! \param     vecPyL       y-component of vectorP left
    //! \param     vecPyR       y-component of vectorP right
    virtual double computeMaxWaveSpeed(Cell& cellLeft,
                                       Cell& cellRight,
                                       const double& rhoL,
                                       const double& rhoR,
                                       const double& uL,
                                       const double& uR,
                                       const double& etaL,
                                       const double& etaR,
                                       const double& vecPxL,
                                       const double& vecPxR,
                                       const double& vecPyL,
                                       const double& vecPyR) const;

  private:
    static const std::string NAME;
};

#endif // MODEULERKORTEWEG_H
