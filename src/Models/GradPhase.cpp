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

#include "GradPhase.h"
#include "../Order2/CellO2NS.h"

std::vector<Variable> variableNamesPhases;
std::vector<std::vector<int>> numeratorPhases;

//***************************************************************************

GradPhase::GradPhase() {}

//***************************************************************************

GradPhase::~GradPhase() {}

//***************************************************************************

void GradPhase::initializeGradsVariablesNamesNumerators()
{
  numeratorPhases.resize(numberPhases);
  for (int i = 0; i < numberPhases; ++i) {
    numeratorPhases[i].resize(m_grads.size());
    for (unsigned int j = 0; j < m_grads.size(); ++j) {
      numeratorPhases[i][j] = i;
    }
  }

  variableNamesPhases.resize(m_grads.size());
}

//***************************************************************************

void GradPhase::computeGradients(Cell* cell, int const& phase) { cell->computeGradients(m_grads, variableNamesPhases, numeratorPhases[phase]); }

//***************************************************************************
//************************** ORDER 2 PARALLEL *******************************
//***************************************************************************

void GradPhase::getBufferGradients(double* buffer, int& counter)
{
  for (unsigned int i = 0; i < m_grads.size(); ++i) {
    m_grads[i].setX(buffer[++counter]);
    m_grads[i].setY(buffer[++counter]);
    m_grads[i].setZ(buffer[++counter]);
  }
}

//***************************************************************************

void GradPhase::fillBufferGradients(double* buffer, int& counter)
{
  for (unsigned int i = 0; i < m_grads.size(); ++i) {
    buffer[++counter] = m_grads[i].getX();
    buffer[++counter] = m_grads[i].getY();
    buffer[++counter] = m_grads[i].getZ();
  }
}

//***************************************************************************
