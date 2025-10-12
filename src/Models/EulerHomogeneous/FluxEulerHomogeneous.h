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

#ifndef FLUXEULERHOMOGENEOUS_H
#define FLUXEULERHOMOGENEOUS_H

#include "../Flux.h"

class FluxEulerHomogeneous;

#include "ModEulerHomogeneous.h"

//! \class     FluxEulerHomogeneous
//! \brief     Model class for Homogeneous Euler Flux (liquid-vapor in thermodynamical equilibrium)
class FluxEulerHomogeneous : public Flux
{
  public:
    FluxEulerHomogeneous();
    ~FluxEulerHomogeneous() override;

    void printFlux() const override;
    void addFlux(double coefA) override;
    void addFlux(Flux* flux) override;
    void subtractFlux(double coefA) override;
    void multiply(double scalar) override;
    void setBufferFlux(Cell& cell) override;
    void buildCons(Phase** phase, Mixture* mixture) override;
    void buildPrim(Phase** phase, Mixture* mixture) override;
    void setToZero() override;
    void addNonCons(double /*coefA*/, const Cell* /*cell*/, const Coord& /*normal*/, const Coord& /*tangent*/, const Coord& /*binormal*/) override {};
    void
    subtractNonCons(double /*coefA*/, const Cell* /*cell*/, const Coord& /*normal*/, const Coord& /*tangent*/, const Coord& /*binormal*/) override {};

    void addFluxSmooth1D(double coefA, const Coord& normal, Cell* cell) override;
    void substractFluxSmooth1D(double coefA, const Coord& normal, Cell* cell) override;

    // Accessors
    //----------
    const Coord& getMomentum() const override { return m_momentum; };
    const double& getMassMix() const override { return m_mass; };
    const double& getEnergyMix() const override { return m_energ; };
    void setCons(const Flux* cons) override;

  protected:
    double m_mass;    //!< mass
    Coord m_momentum; //!< momentum
    double m_energ;   //!< total energy

  private:
    friend class ModEulerHomogeneous;
    //friend class PAEHuler;  // To modify if needed, example: to add a class PAEHViscosity, add friend class PAEHViscosity.
};

#endif // FLUXEULERHOMOGENEOUS_H
