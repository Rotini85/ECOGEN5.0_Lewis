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

#include "Output.h"
#include "../Run.h"
#include "../Config.h"

using namespace tinyxml2;

//***********************************************************************

Output::Output() {}

//***************************************************************

Output::Output(std::string casTest, std::string nameRun, XMLElement* element, std::string fileName, Input* entree) :
  m_input(entree), m_simulationName(casTest), m_folderOutput(nameRun), m_splitData(0), m_numFichier(0), m_nbCpusRestarted(0)
{
  //Affectation pointeur run
  m_run = m_input->getRun();

  //Names communs
  //------------
  m_infoCalcul                 = "infoCalcul.out";
  m_infoMesh                   = "infoMesh";
  m_treeStructure              = "treeStructure";
  m_domainDecomposition        = "domainDecomposition";
  m_fileNameResults            = "result";
  m_filenameCollectionParaview = "collectionParaview";
  m_filenameCollectionVisIt    = "collectionVisIt";

  m_folderOutput            = config.getWorkFolder() + "results/" + m_folderOutput + "/";
  m_folderSavesInput        = m_folderOutput + "savesInput/";
  m_folderDatasets          = m_folderOutput + "datasets/";
  m_folderInfoMesh          = m_folderOutput + "infoMesh/";
  m_folderCuts              = m_folderOutput + "cuts/";
  m_folderProbes            = m_folderOutput + "probes/";
  m_folderGlobalQuantities  = m_folderOutput + "globalQuantities/";
  m_folderBoundaries        = m_folderOutput + "boundaries/";
  m_folderErrorsAndWarnings = m_folderOutput + "errorsAndWarnings/";

  //XMLElement* elementCut;
  XMLError error;

  //Printing precision (digits number)
  if (element->QueryIntAttribute("precision", &m_precision) != XML_NO_ERROR) m_precision = 0; //default if not specified

  //Get if reduced output is on or off
  if (element->QueryBoolAttribute("reducedOutput", &m_reducedOutput) != XML_NO_ERROR) m_reducedOutput = false;

  //Get writing mode
  error = element->QueryBoolAttribute("binary", &m_writeBinary);
  if (error != XML_NO_ERROR) throw ErrorXMLAttribut("binary", fileName, __FILE__, __LINE__);

  //Creation du dossier de sortie ou vidange /Macro selon OS Windows ou Linux
  if (rankCpu == 0) {
    //Macro pour les interaction systeme (creation/destruction repertoires)
    std::string resultsFolder(config.getWorkFolder() + "results/");
#ifdef WIN32
    _mkdir(resultsFolder.c_str());
    _mkdir(m_folderOutput.c_str());
    _mkdir(m_folderSavesInput.c_str());
    _mkdir(m_folderDatasets.c_str());
    _mkdir(m_folderInfoMesh.c_str());
    _mkdir(m_folderCuts.c_str());
    _mkdir(m_folderProbes.c_str());
    _mkdir(m_folderGlobalQuantities.c_str());
    _mkdir(m_folderBoundaries.c_str());
    _mkdir(m_folderErrorsAndWarnings.c_str());
#else
    mkdir(resultsFolder.c_str(), S_IRWXU);
    mkdir(m_folderOutput.c_str(), S_IRWXU);
    mkdir(m_folderSavesInput.c_str(), S_IRWXU);
    mkdir(m_folderDatasets.c_str(), S_IRWXU);
    mkdir(m_folderInfoMesh.c_str(), S_IRWXU);
    mkdir(m_folderCuts.c_str(), S_IRWXU);
    mkdir(m_folderProbes.c_str(), S_IRWXU);
    mkdir(m_folderGlobalQuantities.c_str(), S_IRWXU);
    mkdir(m_folderBoundaries.c_str(), S_IRWXU);
    mkdir(m_folderErrorsAndWarnings.c_str(), S_IRWXU);
#endif
  }
  MPI_Barrier(MPI_COMM_WORLD);

  //Determination du mode Little / Big Endian
  //-----------------------------------------
  int entierTest   = 42; //En binary 0x2a
  char* chaineTest = reinterpret_cast<char*>(&entierTest);
  m_endianMode     = "LittleEndian";
  if (chaineTest[0] != 0x2a) {
    m_endianMode = "BigEndian";
  }
}

//***********************************************************************

Output::Output(std::string nameRun, int fileNumberRestartMeshMapping, Input* input) :
  m_folderOutput(nameRun), m_writeBinary(false), m_splitData(0), m_numFichier(fileNumberRestartMeshMapping)
{
  m_input = input;
  m_run   = m_input->getRun();

  //Names communs
  //------------
  m_infoCalcul                 = "infoCalcul.out";
  m_infoMesh                   = "infoMesh";
  m_treeStructure              = "treeStructure";
  m_domainDecomposition        = "domainDecomposition";
  m_fileNameResults            = "result";
  m_filenameCollectionParaview = "collectionParaview";
  m_filenameCollectionVisIt    = "collectionVisIt";

  m_folderOutput            = config.getWorkFolder() + "results/" + m_folderOutput + "/";
  m_folderSavesInput        = m_folderOutput + "savesInput/";
  m_folderDatasets          = m_folderOutput + "datasets/";
  m_folderInfoMesh          = m_folderOutput + "infoMesh/";
  m_folderCuts              = m_folderOutput + "cuts/";
  m_folderProbes            = m_folderOutput + "probes/";
  m_folderGlobalQuantities  = m_folderOutput + "globalQuantities/";
  m_folderBoundaries        = m_folderOutput + "boundaries/";
  m_folderErrorsAndWarnings = m_folderOutput + "errorsAndWarnings/";
}

//***********************************************************************

Output::Output(XMLElement* element)
{
  //Printing precision (digits number)
  if (element->QueryIntAttribute("precision", &m_precision) != XML_NO_ERROR) m_precision = 0; //default if not specified
}

//***********************************************************************

Output::~Output() {}

//***********************************************************************

void Output::copyInputFiles() const
{
  try {
    IO::copyFile(m_input->getMain(), m_simulationName, m_folderSavesInput);
    IO::copyFile(m_input->getMesh(), m_simulationName, m_folderSavesInput);
    IO::copyFile(m_input->getCI(), m_simulationName, m_folderSavesInput);
    IO::copyFile(m_input->getModel(), m_simulationName, m_folderSavesInput);
  }
  catch (ErrorInput&) {
    throw;
  }
}

//***********************************************************************

void Output::initializeOutput(const Cell& cell)
{
  //Preparation de la cell de reference
  //-----------------------------------
  m_cellRef.allocate(m_run->m_addPhys);
  for (int k = 0; k < m_run->m_numberPhases; k++) {
    m_cellRef.copyPhase(k, cell.getPhase(k));
  }
  m_cellRef.copyMixture(cell.getMixture());
  for (int k = 0; k < m_run->m_numberTransports; k++) {
    m_cellRef.setTransport(cell.getTransport(k).getValue(), k);
  }

  //Preparation propres au type de sortie
  //-------------------------------------
  try {
    this->initializeSpecificOutput();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void Output::initializeOutput(std::vector<CellInterface*>* cellInterfacesLvl)
{
  //Preparation de la cell de reference
  //-----------------------------------
  m_cellRef.allocate(m_run->m_addPhys); //Even if not used here

  //Preparation propres au type de sortie
  //-------------------------------------
  try {
    this->initializeSpecificOutput(cellInterfacesLvl);
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void Output::initializeOutputMeshMapping(const Cell& cell)
{
  //Initialize reference cell
  m_cellRef.allocate(m_run->m_addPhys);
  for (int k = 0; k < m_run->m_numberPhases; k++) {
    m_cellRef.copyPhase(k, cell.getPhase(k));
  }
  m_cellRef.copyMixture(cell.getMixture());
  for (int k = 0; k < m_run->m_numberTransports; k++) {
    m_cellRef.setTransport(cell.getTransport(k).getValue(), k);
  }

  //Note that initializeSpecificOutput is not called to avoid to delete the content
  //of the collection file of the mapped mesh.
}

//***********************************************************************

void Output::initializeOutputInfos()
{
  try {
    std::ofstream fileStream;
    //Fichier infosCalcul
    if (rankCpu == 0) {
      fileStream.open((m_folderOutput + m_infoCalcul).c_str(), std::ios::trunc);
      fileStream.close();
    }
    //Fichiers infosMeshes
    std::string file = m_folderInfoMesh + createFilename(m_infoMesh.c_str(), -1, rankCpu);
    fileStream.open(file.c_str(), std::ios::trunc);
    fileStream.close();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void Output::printTree(Mesh* mesh, std::vector<Cell*>* cellsLvl, int resumeAMRsaveFreq)
{
  if (resumeAMRsaveFreq != 0) {
    if ((m_numFichier % resumeAMRsaveFreq) == 0) {
      try {
        std::ofstream fileStream;
        std::string file;
        //Print domain decomposition
        if (rankCpu == 0) {
          file = m_folderInfoMesh + createFilename(m_domainDecomposition.c_str(), -1, -1, m_numFichier);
          fileStream.open(file.c_str());
          mesh->printDomainDecomposition(fileStream);
          fileStream.close();
        }
        //Print cell tree
        file = m_folderInfoMesh + createFilename(m_treeStructure.c_str(), -1, rankCpu, m_numFichier);
        fileStream.open(file.c_str());
        for (int lvl = 0; lvl <= mesh->getLvlMax(); lvl++) {
          for (unsigned int c = 0; c < cellsLvl[lvl].size(); c++) {
            fileStream << cellsLvl[lvl][c]->getSplit() << " ";
          }
        }
        fileStream.close();
      }
      catch (ErrorECOGEN&) {
        throw;
      }
    }
  }
}

//***********************************************************************

void Output::readDomainDecompostion(Mesh* mesh)
{
  try {
    std::ifstream fileStream;
    std::string file;
    file = m_folderInfoMesh + createFilename(m_domainDecomposition.c_str(), -1, -1, m_numFichier);
    fileStream.open(file.c_str());
    mesh->readDomainDecomposition(fileStream);
    fileStream.close();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void Output::readTree(Mesh* mesh,
                      TypeMeshContainer<Cell*>* cellsLvl,
                      TypeMeshContainer<Cell*>* cellsLvlGhost,
                      TypeMeshContainer<CellInterface*>* cellInterfacesLvl,
                      const std::vector<AddPhys*>& addPhys,
                      int& nbCellsTotalAMR)
{
  try {
    std::ifstream fileStream;
    int splitCell(0);
    std::string chaine;
    std::string file = m_folderInfoMesh + createFilename(m_treeStructure.c_str(), -1, rankCpu, m_numFichier);
    fileStream.open(file.c_str(), std::ios::in);
    if (!fileStream.is_open()) {
      // Avoid segfault if file doesn't exist
      throw ErrorInput("failed to open file: " + file);
    }

    for (int lvl = 0; lvl <= mesh->getLvlMax(); lvl++) {
      //Refine cells and cell interfaces
      for (unsigned int c = 0; c < cellsLvl[lvl].size(); c++) {
        fileStream >> splitCell;
        if (splitCell) mesh->refineCellAndCellInterfaces(cellsLvl[lvl][c], addPhys, nbCellsTotalAMR);
      }

      if (lvl < mesh->getLvlMax()) {
        if (Ncpu > 1) {
          //Refine ghost cells
          parallel.communicationsSplit(lvl);
          cellsLvlGhost[lvl + 1].clear();
          for (unsigned int i = 0; i < cellsLvlGhost[lvl].size(); i++) {
            cellsLvlGhost[lvl][i]->chooseRefineDeraffineGhost(mesh->getNumberCellsY(), mesh->getNumberCellsZ(), addPhys, cellsLvlGhost);
          }

          //Update of persistent communications of cells lvl + 1
          parallel.communicationsNumberGhostCells(lvl + 1);
          parallel.updatePersistentCommunicationsLvlAMR(lvl + 1, mesh->getProblemDimension());
        }

        //Reconstruction of the arrays of cells and cell interfaces of lvl + 1
        cellsLvl[lvl + 1].clear();
        cellInterfacesLvl[lvl + 1].clear();
        for (unsigned int i = 0; i < cellsLvl[lvl].size(); i++) {
          cellsLvl[lvl][i]->buildLvlCellsAndLvlInternalCellInterfacesArrays(cellsLvl, cellInterfacesLvl);
        }
        for (unsigned int i = 0; i < cellInterfacesLvl[lvl].size(); i++) {
          cellInterfacesLvl[lvl][i]->constructionArrayExternalCellInterfacesLvl(cellInterfacesLvl);
        }
      }
    }
    nbCellsTotalAMR = 0;
    for (unsigned int i = 0; i < cellsLvl[0].size(); i++) {
      cellsLvl[0][i]->updateNbCellsTotalAMR(nbCellsTotalAMR);
    }
    fileStream.close();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void Output::writeInfos()
{
  if (m_run->m_iteration > 0) {
    printWritingInfo();
  }
  saveInfos();
  std::cout << "T" << m_run->m_numTest << " | Printing file number: " << m_numFichier << "... ";
}

//***********************************************************************

void Output::writeProgress()
{
  double progress;
  if (!m_run->m_timeControlIterations) progress = m_run->m_physicalTime / m_run->m_finalPhysicalTime * 100.;
  else {
    progress = double(m_run->m_iteration) / double(m_run->m_nbIte) * 100.;
  }
  std::cout << "T" << m_run->m_numTest << " | Iteration " << m_run->m_iteration << " / Timestep " << m_run->m_dt << " / Progress " << progress << "%"
            << std::endl;
}

//***********************************************************************

void Output::saveInfoCells() const
{
  try {
    std::ofstream fileStream;
    std::string file = m_folderInfoMesh + createFilename(m_infoMesh.c_str(), -1, rankCpu);
    fileStream.open(file.c_str(), std::ios::app);
    fileStream << m_run->m_nbCellsTotalAMR << std::endl;
    fileStream.close();
  }
  catch (ErrorECOGEN&) {
    throw;
  }
}

//***********************************************************************

void Output::writeDataset(std::vector<double> dataset, std::ofstream& fileStream, TypeData typeData)
{
  if (m_precision != 0) fileStream.precision(m_precision);
  if (!m_writeBinary) {
    for (unsigned int k = 0; k < dataset.size(); k++) {
      fileStream << dataset[k] << " ";
    }
  }
  else {
    int donneeInt;
    float donneeFloat;
    double donneeDouble;
    char donneeChar;
    int taille;
    switch (typeData) {
    case DOUBLE:
      taille = dataset.size() * sizeof(double);
      break;
    case FLOAT:
      taille = dataset.size() * sizeof(float);
      break;
    case INT:
      taille = dataset.size() * sizeof(int);
      break;
    case CHAR:
      taille = dataset.size() * sizeof(char);
      break;
    }
    IO::writeb64(fileStream, taille);
    char* chaineTampon = new char[taille];
    int index          = 0;
    switch (typeData) {
    case DOUBLE:
      for (unsigned int k = 0; k < dataset.size(); k++) {
        donneeDouble = static_cast<double>(dataset[k]);
        IO::addToTheString(chaineTampon, index, donneeDouble);
      }
      break;
    case FLOAT:
      for (unsigned int k = 0; k < dataset.size(); k++) {
        donneeFloat = static_cast<float>(dataset[k]);
        IO::addToTheString(chaineTampon, index, donneeFloat);
      }
      break;
    case INT:
      for (unsigned int k = 0; k < dataset.size(); k++) {
        donneeInt = static_cast<int>(std::round(dataset[k]));
        IO::addToTheString(chaineTampon, index, donneeInt);
      }
      break;
    case CHAR:
      for (unsigned int k = 0; k < dataset.size(); k++) {
        donneeChar = static_cast<char>(dataset[k]);
        IO::addToTheString(chaineTampon, index, donneeChar);
      }
      break;
    }
    IO::writeb64Chaine(fileStream, chaineTampon, taille);
    delete[] chaineTampon;
  }
}

//***********************************************************************

void Output::getDataset(std::istringstream& data, std::vector<double>& dataset)
{
  if (!m_writeBinary) {
    for (unsigned int k = 0; k < dataset.size(); k++) {
      data >> dataset[k];
    }
  }
  else {
    Errors::errorMessage("resuming on binary results file not available");
    //int donneeInt; float donneeFloat; double donneeDouble; char donneeChar;
    //int taille;
    //switch (typeData) {
    //case DOUBLE:
    //  taille = dataset.size() * sizeof(double); break;
    //case FLOAT:
    //  taille = dataset.size() * sizeof(float); break;
    //case INT:
    //  taille = dataset.size() * sizeof(int); break;
    //case CHAR:
    //  taille = dataset.size() * sizeof(char); break;
    //}
    //IO::writeb64(fileStream, taille);
    //char* chaineTampon = new char[taille]; int index = 0;
    //switch (typeData) {
    //case DOUBLE:
    //  for (unsigned int k = 0; k < dataset.size(); k++) {
    //    donneeDouble = static_cast<double>(dataset[k]);
    //    IO::addToTheString(chaineTampon, index, donneeDouble);
    //  }
    //  break;
    //case FLOAT:
    //  for (unsigned int k = 0; k < dataset.size(); k++) {
    //    donneeFloat = static_cast<float>(dataset[k]);
    //    IO::addToTheString(chaineTampon, index, donneeFloat);
    //  }
    //  break;
    //case INT:
    //  for (unsigned int k = 0; k < dataset.size(); k++) {
    //    donneeInt = static_cast<int>(dataset[k]);
    //    IO::addToTheString(chaineTampon, index, donneeInt);
    //  }
    //  break;
    //case CHAR:
    //  for (unsigned int k = 0; k < dataset.size(); k++) {
    //    donneeChar = static_cast<char>(dataset[k]);
    //    IO::addToTheString(chaineTampon, index, donneeChar);
    //  }
    //  break;
    //}
    //IO::writeb64Chaine(fileStream, chaineTampon, taille);
    //delete[]chaineTampon;
  }
}

//***********************************************************************

void Output::printWritingInfo() const
{
  std::cout << "T" << m_run->m_numTest << " | -------------------------------------------" << std::endl;
  std::cout << "T" << m_run->m_numTest << " | RESULTS FILE NUMBER: " << m_numFichier << ", ITERATION " << m_run->m_iteration << std::endl;
  std::cout << "T" << m_run->m_numTest << " |     Physical time       = " << m_run->m_physicalTime << " s " << std::endl;
  std::cout << "T" << m_run->m_numTest << " |     Last time step      = " << m_run->m_dt << " s " << std::endl;
  m_run->m_stat.printScreenStats(m_run->m_numTest);
}

//***********************************************************************

void Output::saveInfos() const
{
  std::ofstream fileStream;
  if (m_precision != 0) fileStream.precision(m_precision);
  if (rankCpu == 0) {
    fileStream.open((m_folderOutput + m_infoCalcul).c_str(), std::ios::app);

    if (m_numFichier == 0) fileStream << Ncpu << std::endl;

    double secondCompTime = static_cast<double>(m_run->m_stat.getComputationTime()) / CLOCKS_PER_SEC;
    double secondAMRTime  = static_cast<double>(m_run->m_stat.getAMRTime()) / CLOCKS_PER_SEC;
    double secondComTime  = static_cast<double>(m_run->m_stat.getCommunicationTime()) / CLOCKS_PER_SEC;
    fileStream << m_numFichier << " " << m_run->m_iteration << " " << m_run->m_physicalTime << " " << m_run->m_dtNext << " " << secondCompTime << " "
               << secondAMRTime << " " << secondComTime;

    //Additional output with purpose to track the radius of a bubble over time and the maximum pressures.
    //To comment if not needed. Be carefull when using it, integration for bubble radius and maximum pressure at the wall are not generalized.
    //-----
    // fileStream << " " << m_run->m_volumePhaseK;
    // fileStream << " " << m_run->m_pMax[0] << " " << m_run->m_pMax[1] << " " << m_run->m_pMax[2] << " " << m_run->m_pMax[3];
    // fileStream << " " << m_run->m_pMaxWall[0] << " " << m_run->m_pMaxWall[1] << " " << m_run->m_pMaxWall[2] << " " << m_run->m_pMaxWall[3];
    // fileStream << " " << m_run->m_pMax[0];
    // fileStream << " " << m_run->m_pMaxWall[0];
    // fileStream << " " << m_run->m_alphaWanted;
    //-----

    fileStream << std::endl;
    fileStream.close();
  }
}

//***********************************************************************

void Output::readInfos()
{
  std::fstream fileStream;
  std::vector<std::stringstream*> chaine(m_run->m_resumeSimulation + 2); //1 for CPU number, and 1 for initial conditions
  for (unsigned int i = 0; i < chaine.size(); i++) {
    chaine[i] = new std::stringstream;
  }
  std::string chaineTemp;
  double secondCompTime;
  double secondAMRTime;
  double secondComTime;
  int numberCPURead;
  int iter(0);
  try {
    fileStream.open((m_folderOutput + m_infoCalcul).c_str(), std::ios::in); //Opening in reading mode
    if (!fileStream.is_open()) {
      // Avoid segfault if file doesn't exist
      throw ErrorInput("failed to open file: " + m_folderOutput + m_infoCalcul);
    }
    //Verifying CPU number
    std::getline(fileStream, chaineTemp);
    *(chaine[iter]) << chaineTemp;
    *(chaine[iter]) >> numberCPURead;
    if (numberCPURead != Ncpu) {
      throw ErrorInput("resume simulation not possible - number of CPU differs from read files");
    }
    iter++;
    //Finding corresponding results files
    do {
      std::getline(fileStream, chaineTemp);
      *(chaine[iter]) << chaineTemp;
      *(chaine[iter]) >> m_numFichier >> m_run->m_iteration >> m_run->m_physicalTime >> m_run->m_dt >> secondCompTime >> secondAMRTime >>
        secondComTime;
      iter++;
    } while (m_numFichier != m_run->m_resumeSimulation && !fileStream.eof());
    if (fileStream.eof()) {
      throw ErrorInput("resume simulation not possible - check file 'infosCalcul.out'");
    }
  }
  catch (ErrorECOGEN&) {
    for (unsigned int i = 0; i < chaine.size(); i++) {
      delete chaine[i];
    }
    fileStream.close();
    throw;
  }

  //Erasing end of file
  MPI_Barrier(MPI_COMM_WORLD);
  if (rankCpu == 0) {
    fileStream.close();
    fileStream.open((m_folderOutput + m_infoCalcul).c_str(), std::ios::out | std::ios::trunc); //Opening in printing mode with erasing
    for (unsigned int i = 0; i < chaine.size(); i++) {
      fileStream << chaine[i]->str() << std::endl;
    }
    fileStream.close();
  }

  for (unsigned int i = 0; i < chaine.size(); i++) {
    delete chaine[i];
  }

  clock_t compTime(secondCompTime * CLOCKS_PER_SEC);
  clock_t AMRTime(secondAMRTime * CLOCKS_PER_SEC);
  clock_t comTime(secondComTime * CLOCKS_PER_SEC);
  m_run->m_stat.setCompTime(compTime, AMRTime, comTime);
}

//***********************************************************************

int Output::readNbCpu()
{
  std::fstream fileStream;
  std::string nbCpusRestarted("0");
  try {
    fileStream.open((m_folderOutput + m_infoCalcul).c_str(), std::ios::in); //Opening in reading mode
    if (!fileStream.is_open()) {
      // Avoid segfault or erroneous initial number CPUs if file doesn't exist
      throw ErrorInput("failed to open file: " + m_folderOutput + m_infoCalcul);
    }
    //Get number of CPU
    std::getline(fileStream, nbCpusRestarted);
  }
  catch (ErrorECOGEN&) {
    // ErrorInput derives from ErrorECOGEN so it is catched too
    fileStream.close();
    throw;
  }
  return std::stoi(nbCpusRestarted);
}

//***********************************************************************

std::string Output::createFilename(const char* name, int lvl, int proc, int numFichier) const
{
  std::stringstream num;
  num << name;
  //Gestion cpu
  if (proc > -1) num << "_CPU" << proc;
  //Gestion niveau AMR
  if (lvl != -1) num << "_AMR" << lvl;
  //Gestion number de file resultat
  if (numFichier != -1) num << "_TIME" << numFichier;
  //Gestion extension
  num << ".out";
  return num.str();
}

//***********************************************************************
