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

#ifndef MIXUEQTOTENERGY_H
#define MIXUEQTOTENERGY_H

#include <vector>
#include "../Mixture.h"

//! \class     MixUEqTotEnergy
//! \brief     Mixture variables for the velocity-equilibrium system of equations
class MixUEqTotEnergy : public Mixture
{
  public:
    MixUEqTotEnergy();
    //! \brief     Mixture constructor from a XML format reading
    //! \details   Reading data from XML file under the following format:
    //!           ex: <mixture>
    //!                 <velocity x = "0." y = "0." z = "0." />
    //!               </mixture>
    //! \param     state           XML element to read for mixture data
    //! \param     fileName       string name of readed XML file
    MixUEqTotEnergy(tinyxml2::XMLElement* state, std::string fileName);
    ~MixUEqTotEnergy() override;

    void allocateAndCopyMixture(Mixture** mixture) override;
    void copyMixture(Mixture& mixture) override;
    double computeDensity(const double* alphak, const double* rhok) override;
    double computePressure(const double* alphak, const double* pk) override;
    double computeInternalEnergy(const double* Yk, const double* ek) override;
    double computeFrozenSoundSpeed(const double* Yk, const double* ck) override;

    void computeMixtureVariables(Phase** vecPhase) override;
    void computeTotalEnergy(std::vector<QuantitiesAddPhys*>& /*vecGPA*/) override {};
    void totalEnergyToInternalEnergy(std::vector<QuantitiesAddPhys*>& /*vecGPA*/) override {};

    void localProjection(const Coord& normal, const Coord& tangent, const Coord& binormal) override;
    void reverseProjection(const Coord& normal, const Coord& tangent, const Coord& binormal) override;

    //Data printing
    int getNumberScalars() const override { return numberScalarsMixture; };
    int getNumberVectors() const override { return 1; };
    double returnScalar(const int& numVar) const override;
    Coord returnVector(const int& numVar) const override;
    std::string returnNameScalar(const int& numVar) const override;
    std::string returnNameVector(const int& numVar) const override;

    //Data reading
    void setScalar(const int& numVar, const double& value) override;
    void setVector(const int& numVar, const Coord& value) override;

    //Parallel
    int numberOfTransmittedVariables() const override;
    void fillBuffer(double* buffer, int& counter) const override;
    void fillBuffer(std::vector<double>& dataToSend) const override;
    void getBuffer(double* buffer, int& counter) override;
    void getBuffer(std::vector<double>& dataToReceive, int& counter) override;

    //Second order
    void computeSlopesMixture(const Mixture& sLeft, const Mixture& sRight, const double& distance) override;
    void setToZero() override;
    void extrapolate(const Mixture& slope, const double& distance) override;
    void limitSlopes(const Mixture& slopeGauche, const Mixture& slopeDroite, Limiter& globalLimiter) override;

    //Parallel second order
    int numberOfTransmittedSlopes() const override;
    void fillBufferSlopes(double* buffer, int& counter) const override;
    void getBufferSlopes(double* buffer, int& counter) override;

    //Accessors
    const double& getDensity() const override { return m_density; };
    const double& getPressure() const override { return m_pressure; };
    const double& getU() const override { return m_velocity.getX(); };
    const double& getV() const override { return m_velocity.getY(); };
    const double& getW() const override { return m_velocity.getZ(); };
    const Coord& getVelocity() const override { return m_velocity; };
    Coord& getVelocity() override { return m_velocity; };
    const double& getFrozenSoundSpeed() const override { return m_frozenSoundSpeed; };
    const double& getWoodSoundSpeed() const override { return m_woodSoundSpeed; };

    void setPressure(const double& p) override;
    void setVelocity(const double& u, const double& v, const double& w) override;
    void setVelocity(const Coord& vit) override;
    void setU(const double& u) override;
    void setV(const double& v) override;
    void setW(const double& w) override;

    //Operators
    void changeSign() override;
    void multiplyAndAdd(const Mixture& slopesMixtureTemp, const double& coeff) override;
    void divide(const double& coeff) override;

  protected:
    double m_density;          //!< mixture density
    double m_pressure;         //!< mixture pressure
    Coord m_velocity;          //!< mixture velocity
    double m_frozenSoundSpeed; //!< frozen sound speed
    double m_woodSoundSpeed;   //!< wood sound speed
};

#endif // MIXUEQTOTENERGY_H
