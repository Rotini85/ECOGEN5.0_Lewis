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

#ifndef RELAXATIONPFINITE_H
#define RELAXATIONPFINITE_H

#include "RelaxationP.h"
#include "../libTierces/LSODA.h"

//! \brief     Enumeration for the primitive-variable type (usefull for second order, slopes, etc.)
enum IntegrationSolver
{
  Euler,
  RK45,
  LSODA_Enum
};

//! \class     RelaxationPFinite
//! \brief     Finite pressure relaxation
class RelaxationPFinite : public RelaxationP
{
  public:
    RelaxationPFinite();
    RelaxationPFinite(tinyxml2::XMLElement* element, std::string fileName = "Unknown file");
    ~RelaxationPFinite() override;

    //! \brief     Finite Pressure relaxation method
    //! \details   Call for this method computes the mechanical relaxed state in a given cell. Relaxed state is stored depending on the type enum.
    //! \param     cell           cell to relax
    //! \param     dt             time step
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    void relaxation(Cell* cell, const double& dt, Prim type = vecPhases) override;

    //! \brief     Compute the pressure differences
    //! \details   Call for this method computes pressure differences used for relaxation terms.
    //! \param     cell           cell to relax
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    void computePressureDifferences(Cell* cell, Prim type = vecPhases);

    //! \brief     System of equations to integrate for the LSODA solver
    //! \details   This method is called by LSODA solver and it computes the time derivatives of the system in function of the primitive variables.
    //! \param     t              time
    //! \param     y              primitive variables
    //! \param     ydot           time derivatives
    //! \param     data           pointer to data not contained in t and y
    static void system_relaxation(double /*t*/, double* y, double* ydot, void* /*data*/);

  protected:
    double m_muFactor; //!< Factor for the relaxation coefficient. Herein, the relaxation coefficient is identical for all phase_k--phase_j combinations.
    int m_integrationSolver; //!< Int to choose between Euler, RK45 and LSODA solvers.
};

//Externalized for LSODA solver
extern double mu; //!< Relaxation coefficient. Herein, the relaxation coefficient is identical for all phase_k--phase_j combinations.

#endif // RELAXATIONPFINITE_H
