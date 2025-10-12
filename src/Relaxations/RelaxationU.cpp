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

#include "RelaxationU.h"

//***********************************************************************

RelaxationU::RelaxationU() {}

//***********************************************************************

RelaxationU::~RelaxationU() {}

//***********************************************************************

void RelaxationU::relaxation(Cell* cell, const double& /*dt*/, Prim type)
{
  Coord uI = this->computeInterfaceVelocity(cell, type);
  for (int k = 0; k < numberPhases; k++) {
    TB->ek[k]  = cell->getPhase(k, type)->getEnergy();
    TB->ek[k] += 0.5 * (uI - cell->getPhase(k, type)->getVelocity()).squaredNorm();
    TB->pk[k]  = TB->eos[k]->computePressure(cell->getPhase(k, type)->getDensity(), TB->ek[k]);
    //Physical pressure?
    // TB->eos[k]->verifyAndModifyPressure(TB->pk[k]);
    cell->getPhase(k, type)->setPressure(TB->pk[k]);
    cell->getPhase(k, type)->setVelocity(uI);
  }
}

//***********************************************************************

Coord RelaxationU::computeInterfaceVelocity(Cell* cell, Prim type)
{
  //uI = sum_k (u_k Y_k) ;
  Coord uI(0.);
  for (int k = 0; k < numberPhases; k++) {
    uI += cell->getPhase(k, type)->getMassFraction() * cell->getPhase(k, type)->getVelocity();
  }
  return uI;
}

//***********************************************************************
