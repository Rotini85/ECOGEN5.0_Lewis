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

#ifndef PHASEEULER_H
#define PHASEEULER_H

#include "../Phase.h"
#include "../../Eos/Eos.h"
#include <fstream>

//! \class     PhaseEuler
//! \brief     Phase variables for Euler equations (single phase)
class PhaseEuler : public Phase
{
  public:
    PhaseEuler();
    //! \brief     Phase constructor from a XML format reading
    //! \details   Reading data from XML file under the following format:
    //!            ex: <dataFluid density = "10.0" pressure = "1.e5">
    //!                  <velocity x = "1000." y = "1000." z = "0." / >
    //!                </dataFluid>
    //! \param     material           XML element to read for phase data
    //! \param     eos                EOS pointer to compute thermodynamic variables
    //! \param     fileName           string name of readed XML file
    PhaseEuler(tinyxml2::XMLElement* material, Eos* eos, std::string fileName);
    ~PhaseEuler() override;

    void allocateAndCopyPhase(Phase** vecPhase) override;
    void copyPhase(Phase& vecPhase) override;
    void extendedCalculusPhase(const Coord& /*velocity*/) override;

    void localProjection(const Coord& normal, const Coord& tangent, const Coord& binormal) override;
    void reverseProjection(const Coord& normal, const Coord& tangent, const Coord& binormal) override;

    //Specific methods for data printing
    //----------------------------------
    int getNumberScalars() const override { return numberScalarsPhase; };
    int getNumberVectors() const override { return 1; };
    double returnScalar(const int& numVar) const override;
    Coord returnVector(const int& numVar) const override;
    std::string returnNameScalar(const int& numVar) const override;
    std::string returnNameVector(const int& numVar) const override;

    //Specific method for reading from file
    //-------------------------------------
    void setScalar(const int& numVar, const double& value) override;
    void setVector(const int& numVar, const Coord& value) override;

    //Specific methods for parallel computing
    //---------------------------------------
    int numberOfTransmittedVariables() const override;
    void fillBuffer(double* buffer, int& counter) const override;
    void fillBuffer(std::vector<double>& dataToSend) const override;
    void getBuffer(double* buffer, int& counter, Eos** eos) override;
    void getBuffer(std::vector<double>& dataToReceive, int& counter, Eos** eos) override;

    //Specific methods for second order
    //---------------------------------
    void computeSlopesPhase(const Phase& sLeft, const Phase& sRight, const double& distance) override;
    void setToZero() override;
    void setToMax() override;
    void extrapolate(const Phase& slope, const double& distance) override;
    void limitSlopes(const Phase& slopeGauche, const Phase& slopeDroite, Limiter& globalLimiter, Limiter& /*volumeFractionLimiter*/) override;
    void setMin(const Phase& phase1, const Phase& phase2) override;
    void setMax(const Phase& phase1, const Phase& phase2) override;
    void computeGradientLimiter(
      const Limiter& globalLimiter, const Phase& phase, const Phase& phaseMin, const Phase& phaseMax, const Phase& slope) override;

    //Specific methods for parallele computing at second order
    //--------------------------------------------------------
    int numberOfTransmittedSlopes() const override;
    void fillBufferSlopes(double* buffer, int& counter) const override;
    void getBufferSlopes(double* buffer, int& counter) override;

    //Verifications
    //-------------
    void verifyPhase(const std::string& message = "") const override;
    void verifyAndCorrectPhase() override;
    void verifyAndCorrectDensityMax(const double& /*mass*/) override;
    void verifyAndCorrectDensityMax() override;

    //Accessors
    //---------
    const double& getAlpha() const override { return Errors::defaultDouble; };
    const double& getMassFraction() const override { return Errors::defaultDouble; };
    const double& getDensity() const override { return m_density; };
    const double& getPressure() const override { return m_pressure; };
    const double& getU() const override { return m_velocity.getX(); };
    const double& getV() const override { return m_velocity.getY(); };
    const double& getW() const override { return m_velocity.getZ(); };
    Coord& getVelocity() override { return m_velocity; };
    const Coord& getVelocity() const override { return m_velocity; };
    Eos* getEos() const override { return m_eos; };
    const double& getEnergy() const override { return m_energy; };
    const double& getSoundSpeed() const override { return m_soundSpeed; };
    const double& getTotalEnergy() const override { return m_totalEnergy; };
    double getTemperature() const override { return m_eos->computeTemperature(m_density, m_pressure); };

    void setAlpha(double /*alpha*/) override {};
    void setDensity(double density) override;
    void setPressure(double pressure) override;
    void setVelocity(const double& u, const double& v, const double& w) override;
    void setVelocity(const Coord& vit) override;
    void setU(const double& u) override;
    void setV(const double& v) override;
    void setW(const double& w) override;
    void setEos(Eos* eos) override;
    void setEnergy(double energy) override;
    void setSoundSpeed(double soundSpeed) override;
    void setTotalEnergy(double totalEnergy) override;
    void setTemperature(double temperature) override;

    //Operators
    //---------
    void changeSign() override;
    void multiplyAndAdd(const Phase& slopesPhasesTemp, const double& coeff) override;
    void divide(const double& coeff) override;

  protected:
    double m_density;     //!< specific mass
    double m_pressure;    //!< pressure
    double m_temperature; //!< temperature
    Coord m_velocity;     //!< velocity
    Eos* m_eos;           //!< pointer to equation of state
    double m_energy;      //!< internal energy
    double m_totalEnergy; //!< total energy
    double m_soundSpeed;  //!< speed of sound
  private:
};

#endif // PHASEEULER_H
