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

#ifndef OUTPUTPROBEGNU_H
#define OUTPUTPROBEGNU_H

#include "OutputGNU.h"
#include "../Maths/GOLine.h"
#include "../Maths/GOPlan.h"

//! \class     OutputProbeGNU
//! \brief     Printing results for a probe in flow

class OutputProbeGNU : public OutputGNU
{
  public:
    //! \brief     Probe output constructor from a XML format reading
    //! \details   Reading data from XML file under the following format:
    //!            ex: 	<probe name="sensor1">
    //!                   <vertex x = "0.3" y = "0.05" z = "0.05" / >
    //!                   <timeControl acqFreq = "1e-5." / >       <!-- if negative or nul, recording at each time step-->
    //!                 </probe>
    //! \param     casTest           Folder name of test case input files
    //! \param     run               Resutls folder name (defined in 'main.xml')
    //! \param     element           XML element to read for probe data
    //! \param     fileName          string name of readed XML file
    //! \param     entree            Pointer to corresponding run entry object
    OutputProbeGNU(std::string casTest, std::string run, tinyxml2::XMLElement* element, std::string fileName, Input* entree);
    ~OutputProbeGNU() override;

    void probeDisplacement(const double& dt) override;

    void locateProbeInMesh(const TypeMeshContainer<Cell*>& cells, const int& nbCells, bool localSeeking = false) override;
    Cell* locateProbeInAMRSubMesh(std::vector<Cell*>* cells, const int& nbCells) override;

    void initializeSpecificOutput() override;
    void writeResults(Mesh* /*mesh*/, std::vector<Cell*>* /*cellsLvl*/) override;

    void initializeOutputInfos() override {}; //nothing to print
    void writeInfos() override {};

    //Accessors
    double getNextTime() override { return m_nextAcq; };
    bool possesses() override { return m_possessesProbe[rankCpu]; };

  private:
    double m_acqFreq;         //!< Acquisition time frequency
    double m_nextAcq;         //!< Next acquisition time
    Cell* m_cell;             //!< Pointer to the level 0 cell containing the probe
    Cell* m_cellAMR;          //!< Pointer to the most refined cell containing the probe
    GeometricObject* m_objet; //!< To store position
    bool* m_possessesProbe;   //!< True if the CPU possesses probe
};

#endif //OUTPUTPROBEGNU_H
