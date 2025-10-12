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

#include "GradMixture.h"
#include "../Order2/CellO2NS.h"

std::vector<Variable> variableNamesMixture;
std::vector<int> numeratorMixture;

//***************************************************************************

GradMixture::GradMixture() {}

//***************************************************************************

GradMixture::~GradMixture() {}

//***************************************************************************

void GradMixture::initializeGradsVariablesNamesNumerators()
{
  for (unsigned int i = 0; i < m_grads.size(); ++i) { //KS//Supposed to be already done in initializeGradientVectors()
    m_grads[i] = 0.;
  }

  numeratorMixture.resize(m_grads.size());
  for (unsigned int i = 0; i < m_grads.size(); ++i) {
    numeratorMixture[i] = -1;
  }

  variableNamesMixture.resize(m_grads.size());
}

//***************************************************************************

void GradMixture::computeGradients(Cell* cell) { cell->computeGradients(m_grads, variableNamesMixture, numeratorMixture); }

//***************************************************************************
//************************** ORDER 2 PARALLEL *******************************
//***************************************************************************

void GradMixture::getBufferGradients(double* buffer, int& counter)
{
  for (unsigned int i = 0; i < m_grads.size(); ++i) {
    m_grads[i].setX(buffer[++counter]);
    m_grads[i].setY(buffer[++counter]);
    m_grads[i].setZ(buffer[++counter]);
  }
}

//***************************************************************************

void GradMixture::fillBufferGradients(double* buffer, int& counter)
{
  for (unsigned int i = 0; i < m_grads.size(); ++i) {
    buffer[++counter] = m_grads[i].getX();
    buffer[++counter] = m_grads[i].getY();
    buffer[++counter] = m_grads[i].getZ();
  }
}

//***************************************************************************
