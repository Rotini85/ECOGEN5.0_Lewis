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

#include "BoundCondPiston.h"

using namespace tinyxml2;

//****************************************************************************

BoundCondPiston::BoundCondPiston(const BoundCondPiston& Source, const int& lvl) : BoundCond(Source, lvl)
{
  m_heatCondition       = Source.m_heatCondition;
  m_imposedHeatQuantity = Source.m_imposedHeatQuantity;
  m_isMRFWall           = Source.m_isMRFWall;
  m_omegaWall           = Source.m_omegaWall;
  m_uPiston             = 0.;
}

//****************************************************************************

BoundCondPiston::BoundCondPiston(int numPhysique, XMLElement* element, std::string fileName) :
  BoundCond(numPhysique), m_heatCondition(TypeBCHeat::ADIABATIC), m_imposedHeatQuantity(0.), m_isMRFWall(false), m_omegaWall(0.), m_uPiston(0.)
{
  XMLElement* subElement(element->FirstChildElement("dataPiston"));
  if (subElement != NULL) {
    XMLError error;
    XMLElement* elementPhysPiston;

    // Heat transfer
    elementPhysPiston = subElement->FirstChildElement("dataPistonHeatTransfer");
    if (elementPhysPiston != NULL) {
      std::string heatCondition(elementPhysPiston->Attribute("heatCondition"));
      Tools::uppercase(heatCondition);
      // One could use Piston with imposed temperature, imposed flux density or adiabatic (default)
      // This option requires the conductivity additionnal physic
      if (heatCondition == "TEMPERATURE") {
        m_heatCondition = TypeBCHeat::IMPOSEDTEMP;
        error           = elementPhysPiston->QueryDoubleAttribute("temperature", &m_imposedHeatQuantity);
        if (error != XML_NO_ERROR) throw ErrorXMLAttribut("temperature", fileName, __FILE__, __LINE__);
      }
      else if (heatCondition == "FLUX") {
        m_heatCondition = TypeBCHeat::IMPOSEDFLUX;
        error           = elementPhysPiston->QueryDoubleAttribute("flux", &m_imposedHeatQuantity);
        if (error != XML_NO_ERROR) throw ErrorXMLAttribut("flux", fileName, __FILE__, __LINE__);
      }
    }

    // MRF Piston
    elementPhysPiston = subElement->FirstChildElement("dataPistonMRF");
    if (elementPhysPiston != NULL) {
      double omegaX(0.), omegaY(0.), omegaZ(0.);
      error = elementPhysPiston->QueryDoubleAttribute("omegaX", &omegaX);
      if (error != XML_NO_ERROR) throw ErrorXMLAttribut("omegaX", fileName, __FILE__, __LINE__);
      error = elementPhysPiston->QueryDoubleAttribute("omegaY", &omegaY);
      if (error != XML_NO_ERROR) throw ErrorXMLAttribut("omegaY", fileName, __FILE__, __LINE__);
      error = elementPhysPiston->QueryDoubleAttribute("omegaZ", &omegaZ);
      if (error != XML_NO_ERROR) throw ErrorXMLAttribut("omegaZ", fileName, __FILE__, __LINE__);
      m_omegaWall.setXYZ(omegaX, omegaY, omegaZ);
      m_isMRFWall = true;
    }
  }
}

//****************************************************************************

BoundCondPiston::BoundCondPiston(int numPhysique) :
  BoundCond(numPhysique), m_heatCondition(TypeBCHeat::ADIABATIC), m_imposedHeatQuantity(0.), m_isMRFWall(false), m_omegaWall(0.), m_uPiston(0.)
{}

//****************************************************************************

BoundCondPiston::~BoundCondPiston() {}

//****************************************************************************

void BoundCondPiston::createBoundary(TypeMeshContainer<CellInterface*>& cellInterfaces) { cellInterfaces.push_back(new BoundCondPiston(*(this))); }

//****************************************************************************

void BoundCondPiston::solveRiemannBoundary(Cell& cellLeft, const double& dxLeft, double& dtMax)
{
  //NF// Allows you to do cycles in 1D
  //Compute piston velocity. Careful, if piston on the left, the sign of the velocity is the opposite
  m_uPiston = 1.;
  //if (TB->physicalTime > 1.e-6) m_uPiston = -1.;
  //if (TB->physicalTime > 2.e-6) m_uPiston = 1.;

  //Solve the half Riemann problem
  model->solveRiemannPiston(cellLeft, dxLeft, dtMax, m_boundData, m_uPiston);
}

//****************************************************************************

void BoundCondPiston::solveRiemannTransportBoundary(Cell& cellLeft) const { model->solveRiemannTransportPiston(cellLeft, m_uPiston); }

//****************************************************************************
//***************************** AMR Method ***********************************
//****************************************************************************

void BoundCondPiston::creerCellInterfaceChild() { m_cellInterfacesChildren.push_back(new BoundCondPiston(*this, m_lvl + 1)); }

//****************************************************************************
