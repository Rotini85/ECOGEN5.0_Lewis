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

#ifndef PHASEEULERKORTEWEG_H
#define PHASEEULERKORTEWEG_H

#include "../Phase.h"
#include "../../Eos/Eos.h"
#include <fstream>

//! \class     PhaseEulerKorteweg
//! \brief     Phase variables for Augmented Euler--Korteweg equations (single phase)
class PhaseEulerKorteweg : public Phase
{
  public:
    PhaseEulerKorteweg();
    //! \brief     Phase constructor from a XML format reading
    //! \details   Reading data from XML file under the following format:
    //!            ex: <dataFluid density = "10.0">
    //!                  <velocity x = "1000." y = "1000." z = "0." / >
    //!                </dataFluid>
    //! \param     material           XML element to read for phase data
    //! \param     eos                EOS pointer to compute thermodynamic variables
    //! \param     fileName           string name of readed XML file
    PhaseEulerKorteweg(tinyxml2::XMLElement* material, Eos* eos, std::string fileName);
    ~PhaseEulerKorteweg() override;

    void allocateAndCopyPhase(Phase** vecPhase) override;
    void copyPhase(Phase& vecPhase) override;
    void extendedCalculusPhase(const Coord& /*velocity*/) override {};

    void localProjection(const Coord& normal, const Coord& tangent, const Coord& binormal) override;
    void reverseProjection(const Coord& normal, const Coord& tangent, const Coord& binormal) override;

    //Specific methods for data printing
    //----------------------------------
    int getNumberScalars() const override { return 4; };
    int getNumberVectors() const override { return 2; };
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
    void extrapolate(const Phase& slope, const double& distance) override;
    void limitSlopes(const Phase& slopeGauche, const Phase& slopeDroite, Limiter& globalLimiter, Limiter& /*volumeFractionLimiter*/) override;

    //Specific methods for parallele computing at second order
    //--------------------------------------------------------
    int numberOfTransmittedSlopes() const override;
    void fillBufferSlopes(double* buffer, int& counter) const override;
    void getBufferSlopes(double* buffer, int& counter) override;

    //Verifications
    //-------------
    void verifyPhase(const std::string& message = "") const override;
    void verifyAndCorrectPhase() override;
    void verifyAndCorrectDensityMax() override;

    //Accessors
    //---------
    const double& getAlpha() const override { return Errors::defaultDouble; };
    const double& getDensity() const override { return m_density; };
    const double& getOmega() const override { return m_omega; };
    const double& getEta() const override { return m_eta; };
    const double& getPressure() const override { return m_pressure; };
    const double& getU() const override { return m_velocity.getX(); };
    const double& getV() const override { return m_velocity.getY(); };
    const double& getW() const override { return m_velocity.getZ(); };
    Coord& getVelocity() override { return m_velocity; };
    const Coord& getVelocity() const override { return m_velocity; };
    const double& getVectorPX() const override { return m_vectorP.getX(); };
    const double& getVectorPY() const override { return m_vectorP.getY(); };
    const double& getVectorPZ() const override { return m_vectorP.getZ(); };
    Coord& getVectorP() override { return m_vectorP; };
    const Coord& getVectorP() const override { return m_vectorP; };
    Eos* getEos() const override { return m_eos; };

    void setDensity(double density) override;
    void setOmega(const double& omega) override;
    void setEta(const double& eta) override;
    void setPressure(double pressure) override;
    void setVelocity(const double& u, const double& v, const double& w) override;
    void setVelocity(const Coord& vit) override;
    void setU(const double& u) override;
    void setV(const double& v) override;
    void setW(const double& w) override;
    void setVectorP(const double& Px, const double& Py, const double& Pz) override;
    void setVectorP(const Coord& vecP) override;
    virtual void setVectorPX(const double& Px);
    virtual void setVectorPY(const double& Py);
    virtual void setVectorPZ(const double& Pz);
    void setEos(Eos* eos) override;

    //Operators
    //---------
    void changeSign() override;
    void multiplyAndAdd(const Phase& slopesPhasesTemp, const double& coeff) override;
    void divide(const double& coeff) override;

  protected:
    double m_density;  //!< Specific mass
    double m_omega;    //!< Time derivative of eta
    double m_eta;      //!< Analogue of density
    double m_pressure; //!< Pressure, so far it is only for information
    Coord m_velocity;  //!< Velocity
    Coord m_vectorP;   //!< Gradient of eta
    Eos* m_eos;        //!< Pointer to equation of state
};

#endif // PHASEEULERKORTEWEG_H
