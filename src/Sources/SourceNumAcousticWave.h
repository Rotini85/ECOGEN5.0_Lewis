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

#ifndef SOURCENUMACOUSTICWAVE_H
#define SOURCENUMACOUSTICWAVE_H

#include "SourceNum.h"

//! \class     SourceNumAcousticWave
//! \brief     Class for acoustic-wave source terms
class SourceNumAcousticWave : public SourceNum
{
  public:
    //! \brief    SourceNumAcousticWave constructor
    //! \param    order           integration order (could EULER, RK2 or RK4 scheme)
    //! \param    physicalEntity  the entity to which the source term is applied (default whole domain)
    SourceNumAcousticWave(int order, int physicalEntity = 0);
    ~SourceNumAcousticWave() override;

  protected:
    double m_sourceMomentum;    //!Source term on the momentum equation
    double m_pressureAmplitude; //!Pressure amplitude of the wave (Pa)
    double m_delayedTime;       //!Delayed time at which the first pulse starts (s)
    Coord m_sourceLocation;     //!Location of the source (m)
    Coord m_pulseDirection;     //!Direction of the pulse
};

#endif //SOURCENUMACOUSTICWAVE_H
