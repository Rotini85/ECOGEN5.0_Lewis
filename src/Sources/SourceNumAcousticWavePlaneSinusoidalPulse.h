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

#ifndef SOURCENUMACOUSTICWAVEPLANESINUSOIDALPULSE_H
#define SOURCENUMACOUSTICWAVEPLANESINUSOIDALPULSE_H

#include "SourceNumAcousticWave.h"

//! \class     SourceNumAcousticWavePlaneSinusoidalPulse
//! \brief     SubClass of SourceNumAcousticWave for a plane sinusoidal function
class SourceNumAcousticWavePlaneSinusoidalPulse : public SourceNumAcousticWave
{
  public:
    //! \brief     SourceNumAcousticWavePlaneSinusoidalPulse constructor from a XML format reading
    //! \details   Reading data from XML file under the following format:
    //!            ex: <dataHeating q="1.e3"/>
    //! \param     element          XML element to read for source term
    //! \param     order            integration order (could EULER, RK2 or RK4 scheme)
    //! \param     physicalEntity   the entity to which the source term is applied (default whole domain)
    //! \param     fileName         string name of readed XML file
    SourceNumAcousticWavePlaneSinusoidalPulse(tinyxml2::XMLElement* element, int order, int physicalEntity = 0, std::string fileName = "Unknown file");
    ~SourceNumAcousticWavePlaneSinusoidalPulse() override;

    void prepSourceTerms(Cell* cell, const int& i = 0) override;

    //! \brief     Compute the function delta
    //! \param     cell            current cell
    double computeDeltaFunction(const Cell* cell);
    //! \brief     Compute the function g
    double computeGFunction();

  private:
    double m_numberOfPulse; //!Number of pulse (sinusoidal wave)
    double m_frequency;     //!Frequency of the sinusoidal pulse (Hz)
};

#endif //SOURCENUMACOUSTICWAVEPLANESINUSOIDALPULSE_H
