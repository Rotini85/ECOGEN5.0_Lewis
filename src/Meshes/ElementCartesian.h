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

#ifndef ELEMENTCARTESIAN_H
#define ELEMENTCARTESIAN_H

#include "Element.h"

class ElementCartesian : public Element
{
  public:
    ElementCartesian();
    ~ElementCartesian() override;

    void setVolume(const double& volume) override;
    void setLCFL(const double& lCFL) override;
    void setPos(const double& X, const double& Y, const double& Z) override;
    void setPos(const Coord& pos) override;
    void setPosX(const double& X) override;
    void setPosY(const double& Y) override;
    void setPosZ(const double& Z) override;
    void setSize(const double& sizeX, const double& sizeY, const double& sizeZ) override;
    void setSize(const Coord& size) override;

    const double& getSizeX() override { return m_size.getX(); };
    const double& getSizeY() override { return m_size.getY(); };
    const double& getSizeZ() override { return m_size.getZ(); };
    const Coord& getSize() override { return m_size; };

    //Pour methode AMR
    void creerElementChild() override;
    Element* getElementChild(const int& numberChild) override;
    Element* getElementChildBack() override;
    void finalizeElementsChildren() override;

  protected:
    Coord m_size; //!< dimensions of Cartesian cell

    //Attributs pour methode AMR
    std::vector<ElementCartesian*> m_elementsChildren; /*!< Vector d'elements enfants */

  private:
};

#endif // ELEMENTCARTESIAN_H
