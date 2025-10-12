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

#ifndef GEOMETRICOBJECT_H
#define GEOMETRICOBJECT_H

#include "Coord.h"
#include "../Errors.h"
#include "../Tools.h"

//! \class     GeometricObject
//! \brief     Abstract class for geometric object
class GeometricObject
{
  public:
    GeometricObject();
    GeometricObject(TypeGO type, const Coord& vertex);
    virtual ~GeometricObject();

    //! \brief     Compute the distance between a vertex and the corresponding geometric object
    //! \param     vertex             coordinates of the vertex
    virtual double distancePoint(const Coord& /*vertex*/) const
    {
      Errors::errorMessage("distancePoint unknown for considered geometric object");
      return 0;
    };
    //! \brief     Compute the projection between a vertex and the corresponding geometric object
    //! \param     vertex             coordinates of the vertex
    virtual Coord projectionPoint(const Coord& /*vertex*/) const
    {
      Errors::errorMessage("projectionPoint unknown for considered geometric object");
      return 0;
    };

    //! \brief     Return the type of the geometric object
    TypeGO getType() { return m_type; };

    // Accessors
    //----------
    const Coord& getPoint() const { return m_point; };
    virtual void setPositionPoint(double posX, double posY, double posZ) { m_point.setXYZ(posX, posY, posZ); };

  protected:
    TypeGO m_type; //! Type of the geometric object
    Coord m_point; //! Point from the vertex, line or plan
};

#endif //GEOMETRICOBJECT_H
