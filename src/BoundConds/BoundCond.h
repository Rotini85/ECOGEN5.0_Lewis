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

#ifndef BOUNDCOND_H
#define BOUNDCOND_H

#include "../Order1/CellInterface.h"
#include "../Order2/CellInterfaceO2Cartesian.h" //Add for AMR, a priori does not pose a problem
#include "../libTierces/tinyxml2.h"
#include "../Errors.h"
#include "../Tools.h"

class BoundCond : public CellInterface
{
  public:
    BoundCond();
    BoundCond(int numPhysique);
    BoundCond(const BoundCond& Source, const int& lvl = 0);
    ~BoundCond() override;

    virtual void createBoundary(TypeMeshContainer<CellInterface*>& /*cellInterfaces*/)
    {
      Errors::errorMessage("Impossible to create the limit in createBoundary");
    };
    virtual void createBoundary(TypeMeshContainer<CellInterface*>& /*cellInterfaces*/, std::string /*ordreCalcul*/)
    {
      Errors::errorMessage("Impossible to create the limit in createBoundary");
    };
    void initialize(Cell* cellLeft, Cell* /*cellRight*/) override;

    void computeFlux(double& dtMax,
                     Limiter& globalLimiter,
                     Limiter& interfaceLimiter,
                     Limiter& globalVolumeFractionLimiter,
                     Limiter& interfaceVolumeFractionLimiter,
                     Prim type = vecPhases) override;
    void computeFluxAddPhys(AddPhys& addPhys) override;
    void solveRiemann(double& dtMax,
                      Limiter& /*globalLimiter*/,
                      Limiter& /*interfaceLimiter*/,
                      Limiter& /*globalVolumeFractionLimiter*/,
                      Limiter& /*interfaceVolumeFractionLimiter*/,
                      Prim type = vecPhases) override;
    void addFlux(const double& /*coefAMR*/) override {}; //Since it is a boundary there is nothing to add to 'right' cell
    virtual void solveRiemannBoundary(Cell& /*cellLeft*/, const double& /*dxLeft*/, double& /*dtMax*/)
    {
      Errors::errorMessage("Warning solveRiemannLimite not provided for boundary used");
    };
    virtual void solveRiemannTransportBoundary(Cell& /*cellLeft*/) const
    {
      Errors::errorMessage("Warning solveRiemannTransportLimite not provided for boundary used");
    };

    int whoAmI() const override
    {
      Errors::errorMessage("whoAmI not available for boundary used");
      return 0;
    };
    int whoAmIHeat() const override
    {
      Errors::errorMessage("whoAmIHeat not available for boundary used");
      return ADIABATIC;
    };
    virtual void printInfo() {};

    const int& getNumPhys() const override { return m_numPhysique; };
    double getBoundData(VarBoundary var) const override;
    double getBoundaryHeatQuantity() const override
    {
      Errors::errorMessage("getBoundaryHeatQuantity not available for boundary used");
      return 0.;
    }

    void checkMrfInterface(Source* /*sourceMRF*/) override {};

    //Pour methode AMR
    void
    computeXi(const double& /*criteriaVar*/, const bool& /*varRho*/, const bool& /*varP*/, const bool& /*varU*/, const bool& /*varAlpha*/) override {};
    void computeFluxXi() override;
    void raffineCellInterfaceExterne(const int& nbCellsY,
                                     const int& nbCellsZ,
                                     const double& dXParent,
                                     const double& dYParent,
                                     const double& dZParent,
                                     Cell* cellRef,
                                     const int& dim) override;
    void deraffineCellInterfaceExterne(Cell* cellRef) override;

  protected:
    int m_numPhysique;
    std::vector<double> m_boundData; //!< Boundary dataset for boundary output
};

#endif // BOUNDCOND_H
