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

#include "GradPhaseEulerHomogeneous.h"

//***************************************************************************

GradPhaseEulerHomogeneous::GradPhaseEulerHomogeneous()
{
  m_grads.resize(1);
  for (unsigned int i = 0; i < m_grads.size(); ++i) {
    m_grads[i] = 0.;
  }
}

//***************************************************************************

GradPhaseEulerHomogeneous::~GradPhaseEulerHomogeneous() {}

//***************************************************************************

void GradPhaseEulerHomogeneous::initializeGradientVectors()
{
  this->initializeGradsVariablesNamesNumerators();

  variableNamesPhases[VarLocal::alpha] = Variable::alpha;
}

//***************************************************************************

void GradPhaseEulerHomogeneous::computeDistanceGradientScalarProduct(Coord const& distance, Phase* phase) const
{
  static_cast<PhaseEulerHomogeneous*>(phase)->setAlpha(distance.scalar(m_grads[VarLocal::alpha]));
}

//***************************************************************************

void GradPhaseEulerHomogeneous::limitGradients(const Phase& gradientLimiter)
{
  m_grads[VarLocal::alpha].setX(m_grads[VarLocal::alpha].getX() * gradientLimiter.getAlpha());
  m_grads[VarLocal::alpha].setY(m_grads[VarLocal::alpha].getY() * gradientLimiter.getAlpha());
  m_grads[VarLocal::alpha].setZ(m_grads[VarLocal::alpha].getZ() * gradientLimiter.getAlpha());
}

//****************************************************************************
//************************** ORDER 2 PARALLEL ********************************
//****************************************************************************

int GradPhaseEulerHomogeneous::numberOfTransmittedGradients() const { return 3; }

//****************************************************************************
