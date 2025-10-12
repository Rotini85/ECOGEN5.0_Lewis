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

#include "SourceNumAcousticWavePlaneGaussianPulse.h"

using namespace tinyxml2;

//***********************************************************************

SourceNumAcousticWavePlaneGaussianPulse::SourceNumAcousticWavePlaneGaussianPulse(XMLElement* element,
                                                                                 int order,
                                                                                 int physicalEntity,
                                                                                 std::string fileName) :
  SourceNumAcousticWave(order, physicalEntity)
{
  XMLElement* subElement(element->FirstChildElement("dataPlaneGaussianPulse"));
  if (subElement == NULL) throw ErrorXMLElement("dataPlaneGaussianPulse", fileName, __FILE__, __LINE__);

  //Collecting attributes
  //---------------------
  XMLError error;
  //Pressure amplitude
  error = subElement->QueryDoubleAttribute("pressureAmplitude", &m_pressureAmplitude);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("pressureAmplitude", fileName, __FILE__, __LINE__);
  //Delayed time
  error = subElement->QueryDoubleAttribute("delayedTime", &m_delayedTime);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("delayedTime", fileName, __FILE__, __LINE__);
  //Pulse width
  error = subElement->QueryDoubleAttribute("pulseWidth", &m_pulseWidth);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("pulseWidth", fileName, __FILE__, __LINE__);
  //Source location
  XMLElement* subSubElement(subElement->FirstChildElement("sourceLocation"));
  if (subSubElement == NULL) throw ErrorXMLElement("sourceLocation", fileName, __FILE__, __LINE__);
  double x, y, z;
  error = subSubElement->QueryDoubleAttribute("x", &x);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("x", fileName, __FILE__, __LINE__);
  error = subSubElement->QueryDoubleAttribute("y", &y);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("y", fileName, __FILE__, __LINE__);
  error = subSubElement->QueryDoubleAttribute("z", &z);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("z", fileName, __FILE__, __LINE__);
  m_sourceLocation.setXYZ(x, y, z);
  //Direction of the pulse
  subSubElement = subElement->FirstChildElement("pulseDirection");
  if (subSubElement == NULL) throw ErrorXMLElement("pulseDirection", fileName, __FILE__, __LINE__);
  error = subSubElement->QueryDoubleAttribute("x", &x);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("x", fileName, __FILE__, __LINE__);
  error = subSubElement->QueryDoubleAttribute("y", &y);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("y", fileName, __FILE__, __LINE__);
  error = subSubElement->QueryDoubleAttribute("z", &z);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("z", fileName, __FILE__, __LINE__);
  m_pulseDirection.setXYZ(x, y, z);
}

//***********************************************************************

SourceNumAcousticWavePlaneGaussianPulse::~SourceNumAcousticWavePlaneGaussianPulse() {}

//***********************************************************************

void SourceNumAcousticWavePlaneGaussianPulse::prepSourceTerms(Cell* cell, const int& i)
{
  double deltaFunction = this->computeDeltaFunction(cell);
  double gFunction     = this->computeGFunction();
  m_sourceMomentum     = deltaFunction * gFunction;

  sourceCons[i]->prepSourceTermsPlaneWave(cell, m_sourceMomentum, m_pulseDirection);
}

//***********************************************************************

double SourceNumAcousticWavePlaneGaussianPulse::computeDeltaFunction(const Cell* cell)
{
  //The source location must be placed at: cell_center + 1/4 * cell_size_in_wanted_direction

  //Compute the distance of the source to the cell center, in the direction of the pulse
  double h(0.), hx(0.), hy(0.), hz(0.);
  hx = (m_sourceLocation.getX() - cell->getPosition().getX()) * m_pulseDirection.getX();
  hy = (m_sourceLocation.getY() - cell->getPosition().getY()) * m_pulseDirection.getY();
  hz = (m_sourceLocation.getZ() - cell->getPosition().getZ()) * m_pulseDirection.getZ();
  h  = std::sqrt(hx * hx + hy * hy + hz * hz);

  //Support width of the Gaussian function. Of the order of the cell size
  double sigma = 0.0625 * cell->getElement()->getSizeX();

  //Delta function
  //AF//TODO// Precompute sqrt(2M_PI)
  double deltaFunction = 1. / (std::sqrt(2. * M_PI) * sigma) * std::exp(-0.5 * h * h / (sigma * sigma));
  return deltaFunction;
}

//***********************************************************************

double SourceNumAcousticWavePlaneGaussianPulse::computeGFunction()
{
  double gFunction = m_pressureAmplitude / (std::sqrt(2. * M_PI) * m_pulseWidth) *
                     std::exp(-0.5 * (TB->physicalTime - m_delayedTime) * (TB->physicalTime - m_delayedTime) / (m_pulseWidth * m_pulseWidth));
  return gFunction;
}

//***********************************************************************
