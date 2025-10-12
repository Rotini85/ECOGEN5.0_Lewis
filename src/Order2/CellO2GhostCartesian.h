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

#ifndef CELLO2GHOSTCARTESIAN_H
#define CELLO2GHOSTCARTESIAN_H

#include "CellO2Cartesian.h"

class CellInterface;

class CellO2GhostCartesian : public CellO2Cartesian
{
  public:
    CellO2GhostCartesian();
    CellO2GhostCartesian(int lvl); //Pour AMR
    ~CellO2GhostCartesian() override;

    virtual void deleteSlopes(int s);
    void pushBackSlope() override;
    void popBackSlope() override;
    void allocate(const std::vector<AddPhys*>& addPhys) override;
    int getRankOfNeighborCPU() const override;
    void setRankOfNeighborCPU(int rank) override;

    void computeLocalSlopes(CellInterface& cellInterfaceRef,
                            Limiter& globalLimiter,
                            Limiter& interfaceLimiter,
                            Limiter& globalVolumeFractionLimiter,
                            Limiter& interfaceVolumeFractionLimiter,
                            double& alphaCellAfterOppositeSide,
                            double& alphaCell,
                            double& alphaCellOtherInterfaceSide,
                            double& epsInterface) override;
    void createChildCell(const int& lvl) override;
    void getBufferSlopes(double* buffer, int& counter, const int& lvl) override;
    bool isCellGhost() const override { return true; };

  protected:
    int m_rankOfNeighborCPU;                          /*!< Rank of the neighbor CPU corresponding to this ghost cell */
    std::vector<int> m_indexCellInterface;            /*!< Index of the corresponding cell interface for vectors below*/
    std::vector<Phase**> m_vecPhasesSlopesGhost;      /*!< To store slopes of phases */
    std::vector<Mixture*> m_mixtureSlopesGhost;       /*!< To store slopes of mixtures */
    std::vector<double*> m_vecTransportsSlopesGhost;  /*!< To store slopes of transports */
    std::vector<double> m_alphaCellAfterOppositeSide; /*!< To store volume fractions after ghost cell */

  private:
};

#endif // CELLO2GHOSTCARTESIAN_H
