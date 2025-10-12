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

#ifndef QAPCONDUCTIVITY_H
#define QAPCONDUCTIVITY_H

#include "../QuantitiesAddPhys.h"

//! \class     QAPConductivity
//! \brief     General class for thermal conductive quantities
class QAPConductivity : public QuantitiesAddPhys
{
  public:
    QAPConductivity(AddPhys* addPhys);
    ~QAPConductivity() override;

    void computeQuantities(Cell* cell) override;

    //Accessors
    void setGrad(const Coord& grad, const int& num = -1) override;
    const Coord& getGrad(const int& num = -1) const override { return m_gradTk[num]; };

  protected:
    std::vector<Coord> m_gradTk; //!< Vector of the temperature gradient of each phase of the cell

  private:
};

#endif // QAPCONDUCTIVITY_H
