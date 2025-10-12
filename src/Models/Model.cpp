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

#include "Model.h"

//***********************************************************************

Model::Model(const std::string& name, const int& numbTransports) : m_name(name), m_lowMach(false), m_machRefMin(1.e-2), m_smoothCrossSection1d(false)
{
  fluxBufferTransport = 0;
  if (numbTransports > 0) {
    fluxBufferTransport = new Transport[numbTransports];
  }
}

//***********************************************************************

Model::~Model()
{
  if (fluxBufferTransport != 0) delete[] fluxBufferTransport;
  for (unsigned int r = 0; r < m_relaxations.size(); r++) {
    delete m_relaxations[r];
  }
}

//***********************************************************************

void Model::allocateEos(Cell& cell) const
{
  for (int k = 0; k < numberPhases; k++) {
    TB->eos[k] = cell.getPhase(k)->getEos();
  }
}

//***********************************************************************

void Model::initializeRelaxation(Cell* cell) const
{
  for (unsigned int r = 0; r < m_relaxations.size(); r++) {
    m_relaxations[r]->initializeCriticalPressure(cell);
  }
}

//***********************************************************************

void Model::relaxations(Cell* cell, const double& dt, Prim type) const
{
  for (unsigned int r = 0; r < m_relaxations.size(); r++) {
    m_relaxations[r]->relaxation(cell, dt, type);
  }
}

//***********************************************************************

void Model::printInfo() const { std::cout << "Model : " << m_name << std::endl; }

//***********************************************************************
