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

#ifndef RELAXATIONU_H
#define RELAXATIONU_H

#include "Relaxation.h"

//! \class     RelaxationU
//! \brief     Velocity relaxation
class RelaxationU : public Relaxation
{
  public:
    RelaxationU();
    ~RelaxationU() override;

    //! \brief     Return the velocity-relaxation type
    int getType() const override { return U; }

    //! \brief     Stiff velocity relaxation methods
    //! \details   Call for this method computes the relaxed velocity state in a given cell.
    //! \param     cell           cell to relax
    //! \param     dt             time step
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    void relaxation(Cell* cell, const double& /*dt*/, Prim type = vecPhases) override;

    //! \brief     Compute interface velocity
    //! \details   Call for this method computes the interface velocity in a cell.
    //! \param     cell           cell
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    //! \return    interface velocity
    Coord computeInterfaceVelocity(Cell* cell, Prim type = vecPhases);
};

#endif // RELAXATIONU_H
