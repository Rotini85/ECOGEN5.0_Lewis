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

#ifndef RELAXATIONPINFINITE_H
#define RELAXATIONPINFINITE_H

#include "RelaxationP.h"

//! \class     RelaxationPInfinite
//! \brief     Stiff (infinite) pressure relaxation
class RelaxationPInfinite : public RelaxationP
{
  public:
    RelaxationPInfinite();
    ~RelaxationPInfinite() override;

    //! \brief     Stiff (infinite) Pressure relaxation method
    //! \details   Call for this method computes the mechanical relaxed state in a given cell. Relaxed state is stored depending on the type enum.
    //! \param     cell           cell to relax
    //! \param     dt             time step (not used here)
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    void relaxation(Cell* cell, const double& /*dt*/, Prim type = vecPhases) override;
};

#endif // RELAXATIONPINFINITE_H
