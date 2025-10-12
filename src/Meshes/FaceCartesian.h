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

#ifndef FACECARTESIAN_H
#define FACECARTESIAN_H

#include "Face.h"
class FaceCartesian : public Face
{
  public:
    FaceCartesian();
    ~FaceCartesian() override;

    void setSurface(const double& surface) override;
    void initializeOthers(const double& surface, const Coord& normal, const Coord& tangent, const Coord& binormal) override;
    void setPos(const double& X, const double& Y, const double& Z) override;
    void setNormal(const double& X, const double& Y, const double& Z) override;
    void setTangent(const double& X, const double& Y, const double& Z) override;
    void setBinormal(const double& X, const double& Y, const double& Z) override;
    void setSize(const double& sizeX, const double& sizeY, const double& sizeZ) override;
    void setSize(const Coord& size) override;

    const double& getSizeX() override { return m_size.getX(); };
    const double& getSizeY() override { return m_size.getY(); };
    const double& getSizeZ() override { return m_size.getZ(); };
    const Coord& getSize() override { return m_size; };

    //Pour methode AMR
    Face* creerNouvelleFace() override;

  protected:
    Coord m_size; //!< dimensions of Cartesian face
};

#endif // FACECARTESIAN_H
