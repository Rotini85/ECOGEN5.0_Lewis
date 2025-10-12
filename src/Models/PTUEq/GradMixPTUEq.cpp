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

#include "GradMixPTUEq.h"

//***************************************************************************

GradMixPTUEq::GradMixPTUEq()
{
  m_grads.resize(5);
  for (unsigned int i = 0; i < m_grads.size(); ++i) {
    m_grads[i] = 0.;
  }
}

//***************************************************************************

GradMixPTUEq::~GradMixPTUEq() {}

//***************************************************************************

void GradMixPTUEq::initializeGradientVectors()
{
  this->initializeGradsVariablesNamesNumerators();

  variableNamesMixture[VarLocal::pressure]    = Variable::pressure;
  variableNamesMixture[VarLocal::temperature] = Variable::temperature;
  variableNamesMixture[VarLocal::velocityU]   = Variable::velocityU;
  variableNamesMixture[VarLocal::velocityV]   = Variable::velocityV;
  variableNamesMixture[VarLocal::velocityW]   = Variable::velocityW;
}

//***************************************************************************

void GradMixPTUEq::computeDistanceGradientScalarProduct(Coord const& distance, Mixture* mixture) const
{
  static_cast<MixPTUEq*>(mixture)->setPressure(distance.scalar(m_grads[VarLocal::pressure]));
  static_cast<MixPTUEq*>(mixture)->setTemperature(distance.scalar(m_grads[VarLocal::temperature]));
  static_cast<MixPTUEq*>(mixture)->setVelocity(distance.scalar(m_grads[VarLocal::velocityU]),
                                               distance.scalar(m_grads[VarLocal::velocityV]),
                                               distance.scalar(m_grads[VarLocal::velocityW]));
}

//***************************************************************************

void GradMixPTUEq::limitGradients(const Mixture& gradientLimiter)
{
  m_grads[VarLocal::pressure].setX(m_grads[VarLocal::pressure].getX() * gradientLimiter.getPressure());
  m_grads[VarLocal::pressure].setY(m_grads[VarLocal::pressure].getY() * gradientLimiter.getPressure());
  m_grads[VarLocal::pressure].setZ(m_grads[VarLocal::pressure].getZ() * gradientLimiter.getPressure());

  m_grads[VarLocal::temperature].setX(m_grads[VarLocal::temperature].getX() * gradientLimiter.getTemperature());
  m_grads[VarLocal::temperature].setY(m_grads[VarLocal::temperature].getY() * gradientLimiter.getTemperature());
  m_grads[VarLocal::temperature].setZ(m_grads[VarLocal::temperature].getZ() * gradientLimiter.getTemperature());

  m_grads[VarLocal::velocityU].setX(m_grads[VarLocal::velocityU].getX() * gradientLimiter.getVelocity().getX());
  m_grads[VarLocal::velocityU].setY(m_grads[VarLocal::velocityU].getY() * gradientLimiter.getVelocity().getX());
  m_grads[VarLocal::velocityU].setZ(m_grads[VarLocal::velocityU].getZ() * gradientLimiter.getVelocity().getX());

  m_grads[VarLocal::velocityV].setX(m_grads[VarLocal::velocityV].getX() * gradientLimiter.getVelocity().getY());
  m_grads[VarLocal::velocityV].setY(m_grads[VarLocal::velocityV].getY() * gradientLimiter.getVelocity().getY());
  m_grads[VarLocal::velocityV].setZ(m_grads[VarLocal::velocityV].getZ() * gradientLimiter.getVelocity().getY());

  m_grads[VarLocal::velocityW].setX(m_grads[VarLocal::velocityW].getX() * gradientLimiter.getVelocity().getZ());
  m_grads[VarLocal::velocityW].setY(m_grads[VarLocal::velocityW].getY() * gradientLimiter.getVelocity().getZ());
  m_grads[VarLocal::velocityW].setZ(m_grads[VarLocal::velocityW].getZ() * gradientLimiter.getVelocity().getZ());
}

//***************************************************************************
//************************** ORDER 2 PARALLEL *******************************
//***************************************************************************

int GradMixPTUEq::numberOfTransmittedGradients() const { return 15; }

//***************************************************************************
