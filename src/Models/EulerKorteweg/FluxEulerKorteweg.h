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

#ifndef FLUXEULERKORTEWEG_H
#define FLUXEULERKORTEWEG_H

#include "../Flux.h"

//! \class     FluxEulerKorteweg
//! \brief     Model class for Augmented Euler--Korteweg Flux (single phase)
class FluxEulerKorteweg : public Flux
{
  public:
    FluxEulerKorteweg();
    ~FluxEulerKorteweg() override;

    void printFlux() const override;
    void addFlux(double coefA) override;
    void addFlux(Flux* flux) override;
    void subtractFlux(double coefA) override;
    void multiply(double scalar) override;
    void setBufferFlux(Cell& cell) override;
    void buildCons(Phase** phase, Mixture* /*mixture*/) override;
    void buildPrim(Phase** phase, Mixture* /*mixture*/) override;
    void setToZero() override;
    void addNonCons(double /*coefA*/, const Cell* /*cell*/, const Coord& /*normal*/, const Coord& /*tangent*/, const Coord& /*binormal*/) override {};
    void
    subtractNonCons(double /*coefA*/, const Cell* /*cell*/, const Coord& /*normal*/, const Coord& /*tangent*/, const Coord& /*binormal*/) override {};

    void addSymmetricTerms(Phase** /*phases*/, Mixture* /*mixture*/, const double& /*r*/, const double& /*v*/) override {};
    void prepSourceTermsGravity(const Coord& /*g*/) override {};
    void prepSourceTermsHeating(const double& /*q*/) override {};
    void prepSourceTermsMRF(Cell* /*cell*/, const Coord& /*omega*/) override {};
    void prepSourceTermsPlaneWave(const Cell* /*cell*/, const double& /*sourceMomentum*/, const Coord& /*pulseDirection*/) override {};

    // Accessors
    //----------
    const double& getMassMix() const override { return m_mass; };
    const double& getEqOmega() const override { return m_eqOmega; };
    const double& getEqEta() const override { return m_eqEta; };
    const Coord& getMomentum() const override { return m_momentum; };
    const Coord& getEqVectorP() const override { return m_eqVectorP; };
    void setCons(const Flux* cons) override;

  protected:
    double m_mass;     //!< Mass
    double m_eqOmega;  //!< Omega equation (omega: time derivative of eta)
    double m_eqEta;    //!< Eta equation
    Coord m_momentum;  //!< Momentum
    Coord m_eqVectorP; //!< Equation for vector P (P: gradient of eta)

  private:
    friend class ModEulerKorteweg;
    // To modify if needed, example: to add a class APEViscosity, add friend class APEViscosity.
};

extern double alphaEK, betaEK, temperatureEK, kappaEK; //!< Simulation variable for Euler-Korteweg (temperatureEK is a constant temperature)

#endif // FLUXEULERKORTEWEG_H
