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

#ifndef RELAXATIONPT_H
#define RELAXATIONPT_H

#include "Relaxation.h"

//! \class     RelaxationPT
//! \brief     Pressure-Temperature relaxation
class RelaxationPT : public Relaxation
{
  public:
    RelaxationPT();
    ~RelaxationPT() override;

    //! \brief     Stiff Pressure-Temperature relaxation method
    //! \details   call for this method computes the mechanical and thermal relaxed state in a given cell. Relaxed state is stored depending on the type enum
    //! \param     cell           cell to relax
    //! \param     dt             time step (not used here)
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    void relaxation(Cell* cell, const double& /*dt*/, Prim type = vecPhases) override;

    //! \brief     Pressure determination with analytical formulae for 2 phases governed by SG or IG EOS
    //! \details   call for this method determines the pressure using the analytical formulae only valid in the specific case of 2 phases governed by SG or IG EOS
    //! \param     cell           cell to relax
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    //! \return    pressure
    double analyticalPressure(Cell* cell, Prim type = vecPhases) const;

    //! \brief     Temperature calculus with analytical formulae for N phases governed by SG or IG EOS
    //! \details   call for this method computes the temprerature for N phases governed by SG or IG EOS using the known relaxed pressure
    //! \param     pressure       the value of relaxed pressure
    //! \param     cell           cell to relax
    //! \param     type           enumeration allowing to relax either state in the cell or second order half time step state
    //! \return    temperature
    double analyticalTemperature(double pressure, Cell* cell, Prim type = vecPhases) const;

    //! \brief     Return the pressure- and temperature-relaxation type
    int getType() const override { return PT; }
};

#endif // RELAXATIONPT_H
