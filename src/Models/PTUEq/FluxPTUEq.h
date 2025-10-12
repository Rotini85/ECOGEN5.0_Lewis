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

#ifndef FLUXPTUEQ_H
#define FLUXPTUEQ_H

#include "../Flux.h"

//! \class     FluxPTUEq
//! \brief     Flux class for pressure-temperature-velocity (mechanical and thermal equilibrium) system of equations
class FluxPTUEq : public Flux
{
  public:
    FluxPTUEq(const int& numbPhases);
    ~FluxPTUEq() override;

    void printFlux() const override;
    void addFlux(double coefA) override;
    void addFlux(Flux* flux) override;
    void subtractFlux(double coefA) override;
    void multiply(double scalar) override;
    void setBufferFlux(Cell& cell) override;
    void buildCons(Phase** phases, Mixture* mixture) override;
    void buildPrim(Phase** phases, Mixture* mixture) override;
    void setToZero() override;
    void addNonCons(double /*coefA*/, const Cell* /*cell*/, const Coord& /*normal*/, const Coord& /*tangent*/, const Coord& /*binormal*/) override {};
    void
    subtractNonCons(double /*coefA*/, const Cell* /*cell*/, const Coord& /*normal*/, const Coord& /*tangent*/, const Coord& /*binormal*/) override {};

    void prepSourceTermsHeating(const double& q) override;

    // Accessors
    //----------
    const double& getMass(const int& numPhase) const override { return m_mass[numPhase]; };
    const Coord& getMomentum() const override { return m_momentum; };
    const double& getEnergyMix() const override { return m_energMixture; };
    void setCons(const Flux* cons) override;

  protected:
    double* m_mass;        //!< mass array
    Coord m_momentum;      //!< momentum array
    double m_energMixture; //!< mixture energy

  private:
    friend class ModPTUEq;
    // To modify if needed, example: To add a class APPTUEqViscosity, add friend class APPTUEqViscosity.
};

#endif // FLUXPTUEQ_H
