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

#include "OutputGNU.h"
#include "../Run.h"

using namespace tinyxml2;

//***********************************************************************

OutputGNU::OutputGNU() {}

//***********************************************************************

OutputGNU::OutputGNU(std::string casTest, std::string run, XMLElement* element, std::string fileName, Input* entree) :
  Output(casTest, run, element, fileName, entree)
{
  m_fileNameVisu        = "visualization.gnu";
  m_fileNamePDF         = "generatePDF.gnu";
  m_folderScriptGnuplot = "./datasets/";
  m_type                = TypeOutput::GNU;
}

//***********************************************************************

OutputGNU::OutputGNU(tinyxml2::XMLElement* element) : Output(element) {}

//***********************************************************************

OutputGNU::~OutputGNU() {}

//***********************************************************************

void OutputGNU::writeResults(Mesh* mesh, std::vector<Cell*>* cellsLvl)
{
  try {
    std::ofstream fileStream;
    std::string file = m_folderDatasets + createFilenameGNU(m_fileNameResults.c_str(), -1, rankCpu, m_numFichier);
    fileStream.open(file.c_str());
    if (!fileStream) {
      throw ErrorECOGEN(file + " cannot be opened", __FILE__, __LINE__);
    }
    if (m_precision != 0) fileStream.precision(m_precision);
    mesh->writeResultsGnuplot(cellsLvl, fileStream, nullptr, m_run->m_recordPsat);
    fileStream << std::endl;
    fileStream.close();

    //Generation of Gnuplot scripts for: 1) direct results vizualisation and 2) pdf generation
    if (rankCpu == 0) {
      writeScriptGnuplot(mesh->getProblemDimension());
      writeScriptGnuplotPDF(mesh->getProblemDimension());
    }
  }
  catch (ErrorECOGEN&) {
    throw;
  }
  m_numFichier++;
}

//***********************************************************************

void OutputGNU::writeScriptGnuplot(const int& dim)
{
  try {
    std::ofstream fileStream;

    fileStream.open((m_folderOutput + m_fileNameVisu).c_str());
    if (!fileStream) {
      throw ErrorECOGEN("File " + m_folderOutput + m_fileNameVisu + " cannot be opened", __FILE__, __LINE__);
    }
    fileStream << "reset" << std::endl << "set style data lines" << std::endl;
    fileStream << "set nokey" << std::endl;
    fileStream << std::endl;

    int index;

    if (dim == 0) { //Special case probe: first columns : t, etc.
      index = 2;
      fileStream << "set xlabel 't (s)'" << std::endl;
    }
    else {
      index = 1 + dim; //premiere colonne file resultat == premiere donnee apres X,(Y,Z)
      fileStream << "set xlabel 'x (m)'" << std::endl;
    }
    //Gestion 2D/3D
    if (dim == 2) {
      fileStream << "set surface" << std::endl;
      fileStream << "set dgrid3d 50,50" << std::endl;
      fileStream << "set contour base" << std::endl;
      fileStream << "set cntrparam levels 25" << std::endl;
      fileStream << "show contour" << std::endl;
      //fileStream << "set view 180,180,1,1" << endl;
    }
    else if (dim == 3) {
      throw ErrorECOGEN("OutputGNU::writeScriptGnuplot : print script gnuplot non prevu en 3D", __FILE__, __LINE__);
    }

    //1) Variables des phases
    //-----------------------
    for (int phase = 0; phase < m_run->getNumberPhases(); phase++) {
      //Variables scalars
      for (int var = 1; var <= m_cellRef.getPhase(phase)->getNumberScalars(); var++) {
        fileStream << "set title '" << formatVarNameStyle(m_cellRef.getPhase(phase)->returnNameScalar(var));
        if (m_cellRef.getPhase(phase)->getEos() != nullptr) {
          fileStream << "\\_" << formatVarNameStyle(m_cellRef.getPhase(phase)->getEos()->getName());
        }
        fileStream << "'" << std::endl;
        printBlocGnuplot(fileStream, index, dim);
      } //End var scalar
      //Variables vectorielles (u)
      for (int var = 1; var <= m_cellRef.getPhase(phase)->getNumberVectors(); var++) {
        fileStream << "set title '" << formatVarNameStyle(m_cellRef.getPhase(phase)->returnNameVector(var));
        if (m_cellRef.getPhase(phase)->getEos() != nullptr) {
          fileStream << "\\_" << formatVarNameStyle(m_cellRef.getPhase(phase)->getEos()->getName());
        }
        fileStream << "'" << std::endl;
        printBlocGnuplot(fileStream, index, dim);
      } //End var vectorielle
    } //End phase

    //2) Variables mixture
    //--------------------
    //Variables scalars
    for (int var = 1; var <= m_cellRef.getMixture()->getNumberScalars(); var++) {
      fileStream << "set title '" << formatVarNameStyle(m_cellRef.getMixture()->returnNameScalar(var)) << "'" << std::endl;
      printBlocGnuplot(fileStream, index, dim);
    } //End var scalar
    //Variables vectorielle
    for (int var = 1; var <= m_cellRef.getMixture()->getNumberVectors(); var++) {
      fileStream << "set title '" << formatVarNameStyle(m_cellRef.getMixture()->returnNameVector(var)) << "'" << std::endl;
      printBlocGnuplot(fileStream, index, dim);
    } //End var vectorielle

    //3) Variables transports
    //-----------------------
    for (int var = 1; var <= m_cellRef.getNumberTransports(); var++) {
      fileStream << "set title 'Transport\\_" << var << "'" << std::endl;
      printBlocGnuplot(fileStream, index, dim);
    } //End var scalar

    //4) Write AMR levels
    //-------------------
    fileStream << "set title 'AMR level'" << std::endl;
    printBlocGnuplot(fileStream, index, dim);

    //5) Write variable detection gradients
    //-------------------------------------
    fileStream << "set title 'Xi'" << std::endl;
    printBlocGnuplot(fileStream, index, dim);

    //6) Writing saturation pressure (specific recording)
    //---------------------------------------------------
    if (m_run->m_recordPsat) {
      fileStream << "set title 'Psat'" << std::endl;
      printBlocGnuplot(fileStream, index, dim);
    }

    fileStream.close();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void OutputGNU::writeScriptGnuplotPDF(const int& dim)
{
  try {
    std::ofstream fileStream;

    fileStream.open((m_folderOutput + m_fileNamePDF).c_str());
    if (!fileStream) {
      throw ErrorECOGEN("File " + m_folderOutput + m_fileNamePDF + " cannot be opened", __FILE__, __LINE__);
    }
    fileStream << "reset" << std::endl;
    fileStream << "set terminal pdfcairo enhanced font \"Arial,12\" size 8,12" << std::endl;
    fileStream << "set output \"results.pdf\"" << std::endl << std::endl;
    fileStream << "set style data lines" << std::endl;
    fileStream << "set nokey" << std::endl << std::endl;

    int index;

    if (dim == 0) { //Special case probe: first columns : t, etc.
      index = 2;
      fileStream << "set xlabel 't (s)'" << std::endl;
    }
    else {
      index = 1 + dim; //premiere colonne file resultat == premiere donnee apres X,(Y,Z)
      fileStream << "set xlabel 'x (m)'" << std::endl;
    }
    //Gestion 2D/3D
    if (dim == 2) {
      fileStream << "set surface" << std::endl;
      fileStream << "set dgrid3d 50,50" << std::endl;
      fileStream << "set contour base" << std::endl;
      fileStream << "set cntrparam levels 25" << std::endl;
      fileStream << "show contour" << std::endl;
      //fileStream << "set view 180,180,1,1" << endl;
    }
    else if (dim == 3) {
      throw ErrorECOGEN("OutputGNU::writeScriptGnuplot : print script gnuplot non prevu en 3D", __FILE__, __LINE__);
    }

    //1) Variables des phases
    //-----------------------
    for (int phase = 0; phase < m_run->getNumberPhases(); phase++) {
      fileStream << "set multiplot layout 4,2 title \"Phases variables\"" << std::endl;
      //Variables scalars
      for (int var = 1; var <= m_cellRef.getPhase(phase)->getNumberScalars(); var++) {
        fileStream << "set title '" << formatVarNameStyle(m_cellRef.getPhase(phase)->returnNameScalar(var));
        if (m_cellRef.getPhase(phase)->getEos() != nullptr) {
          fileStream << "\\_" << formatVarNameStyle(m_cellRef.getPhase(phase)->getEos()->getName());
        }
        fileStream << "'" << std::endl;
        printBlocGnuplot(fileStream, index, dim, false);
      } //End var scalar
      //Variables vectorielles (u)
      for (int var = 1; var <= m_cellRef.getPhase(phase)->getNumberVectors(); var++) {
        fileStream << "set title '" << formatVarNameStyle(m_cellRef.getPhase(phase)->returnNameVector(var));
        if (m_cellRef.getPhase(phase)->getEos() != nullptr) {
          fileStream << "\\_" << formatVarNameStyle(m_cellRef.getPhase(phase)->getEos()->getName());
        }
        fileStream << "'" << std::endl;
        printBlocGnuplot(fileStream, index, dim, false);
      } //End var vectorielle
    } //End phase

    //2) Variables mixture
    //--------------------
    if (m_cellRef.getMixture()->getNumberScalars() != 0) fileStream << "set multiplot layout 4,2 title \"Mixture variables\"" << std::endl;
    //Variables scalars
    for (int var = 1; var <= m_cellRef.getMixture()->getNumberScalars(); var++) {
      fileStream << "set title '" << formatVarNameStyle(m_cellRef.getMixture()->returnNameScalar(var)) << "'" << std::endl;
      printBlocGnuplot(fileStream, index, dim, false);
    } //End var scalar
    //Variables vectorielle
    for (int var = 1; var <= m_cellRef.getMixture()->getNumberVectors(); var++) {
      fileStream << "set title '" << formatVarNameStyle(m_cellRef.getMixture()->returnNameVector(var)) << "'" << std::endl;
      printBlocGnuplot(fileStream, index, dim, false);
    } //End var vectorielle

    //3) Variables transports
    //-----------------------
    if (m_cellRef.getNumberTransports() != 0) fileStream << "set multiplot layout 4,2 title \"Transport variables\"" << std::endl;
    for (int var = 1; var <= m_cellRef.getNumberTransports(); var++) {
      fileStream << "set title 'Transport\\_" << var << "'" << std::endl;
      printBlocGnuplot(fileStream, index, dim, false);
    } //End var scalar

    //Other graphs
    fileStream << "set multiplot layout 4,2 title \"Other variables\"" << std::endl;
    //4) Write AMR levels
    //-------------------
    fileStream << "set title 'AMR level'" << std::endl;
    printBlocGnuplot(fileStream, index, dim, false);

    //5) Write variable detection gradients
    //-------------------------------------
    fileStream << "set title 'Xi'" << std::endl;
    printBlocGnuplot(fileStream, index, dim, false);

    //6) Writing saturation pressure (specific recording)
    //---------------------------------------------------
    if (m_run->m_recordPsat) {
      fileStream << "set title 'Psat'" << std::endl;
      printBlocGnuplot(fileStream, index, dim, false);
    }

    fileStream << "unset multiplot" << std::endl;
    fileStream << "set output" << std::endl;
    fileStream.close();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void OutputGNU::writeScriptGnuplot(const std::string& varName)
{
  try {
    std::ofstream fileStream;

    fileStream.open((m_folderOutput + m_fileNameVisu).c_str());
    if (!fileStream) {
      throw ErrorECOGEN("Cannot open the file " + m_folderOutput + m_fileNameVisu, __FILE__, __LINE__);
    }
    fileStream << "reset" << std::endl << "set style data lines" << std::endl;
    fileStream << "set nokey" << std::endl;
    fileStream << std::endl;

    fileStream << "set xlabel 't (s)'" << std::endl;
    fileStream << "set title '" << formatVarNameStyle(varName) << "'" << std::endl;

    int index(2);
    int dim(0);
    printBlocGnuplot(fileStream, index, dim);
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void OutputGNU::printBlocGnuplot(std::ofstream& fileStream, int& index, const int& dim, bool pause)
{
  try {
    if (dim <= 1) {
      fileStream << "plot";
    }
    else {
      fileStream << "splot";
    }
    for (int t = 0; t <= m_numFichier; t++) {
      for (int p = 0; p < Ncpu; p++) {
        fileStream << " \"";
        if (dim == 0) {
          fileStream << m_folderScriptGnuplot + createFilenameGNU(m_fileNameResults.c_str(), -1, -1, -1);
        }
        else {
          fileStream << m_folderScriptGnuplot + createFilenameGNU(m_fileNameResults.c_str(), -1, p, t);
        }
        fileStream << "\"";
        if (dim <= 1) {
          fileStream << " u 1:" << index;
        }
        else {
          fileStream << " u 1:2:" << index;
        }
        if (dim == 0) {
          fileStream << std::endl;
          break;
        }
        if (p < Ncpu - 1 || t != m_numFichier) fileStream << ",\\";
        fileStream << std::endl;
      }
    }
    if (pause) fileStream << "pause(-1)" << std::endl;
    fileStream << std::endl;
    index++;
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

std::string OutputGNU::createFilenameGNU(const char* name, int lvl, int proc, int numFichier, std::string nameVariable) const
{
  try {
    std::stringstream num;

    if (m_splitData) {
      throw ErrorECOGEN("OutputGNU::createFilenameGNU : separated data not planned", __FILE__, __LINE__);
      return 0;
    }
    num << name;
    //Gestion nameVariable
    if (nameVariable != "defaut") num << "_" << nameVariable << "_";
    //Gestion binary
    if (m_writeBinary) {
      //num << "B64";
      throw ErrorECOGEN("OutputGNU::createFilenameGNU : binary data not planned", __FILE__, __LINE__);
      return 0;
    }
    //Gestion cpu
    if (proc > -1) num << "_CPU" << proc;
    //Gestion niveau AMR
    if (lvl != -1) {
      //num << "_AMR" << lvl;
      throw ErrorECOGEN("OutputGNU::createFilenameGNU : AMR data by level not planned", __FILE__, __LINE__);
      return 0;
    }
    //Gestion number de file resultat
    if (numFichier != -1) num << "_TIME" << numFichier;
    //Gestion extension
    num << ".out";
    return num.str();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

std::string OutputGNU::formatVarNameStyle(std::string const& varNameToFormat) const
{
  // Replace all occurrences of an underscore character in a given string with a backslash followed by an underscore
  std::string str = varNameToFormat;
  size_t pos      = 0;
  while ((pos = str.find("_", pos)) != std::string::npos) {
    str.replace(pos, 1, "\\_");
    pos += 2; // Move past the replaced "\\_"
  }
  // Remove extension ".xml" for EOS file
  if (str.length() >= 4 && str.substr(str.length() - 4) == ".xml") {
    str = str.substr(0, str.length() - 4); // Remove the extension ".xml"
  }
  return str;
}

//***********************************************************************
