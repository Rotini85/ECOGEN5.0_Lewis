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

#include "ElementSegment.h"

const int ElementSegment::TYPEGMSH    = 1;
const int ElementSegment::NUMBERNODES = 2;
const int ElementSegment::NUMBERFACES = 2;
const int ElementSegment::TYPEVTK     = 3;

//***********************************************************************

ElementSegment::ElementSegment() : ElementNS(TYPEGMSH, NUMBERNODES, NUMBERFACES, TYPEVTK) {}

//***********************************************************************

ElementSegment::~ElementSegment() {}

//***********************************************************************

void ElementSegment::computeVolume(const Coord* nodes)
{
  m_volume = (nodes[1] - nodes[0]).norm(); //segment length
}

//***********************************************************************

void ElementSegment::computeLCFL(const Coord* nodes)
{
  m_lCFL = (nodes[1] - nodes[0]).norm() / 2.0; //segment half-length
}

//***********************************************************************

void ElementSegment::construitFaces(const Coord* nodes, FaceNS** faces, int& iMax, int** facesBuff, int* sumNodesBuff)
{
  //2 vertex-kind faces to treat
  int indexFaceExiste(-1);
  int nodeAutre(0);
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      facesBuff[iMax][0] = m_numNodes[0];
      nodeAutre          = 1;
      break;
    case 1:
      facesBuff[iMax][0] = m_numNodes[1];
      nodeAutre          = 0;
      break;
    }
    sumNodesBuff[iMax] = facesBuff[iMax][0];
    // Checking face existence
    indexFaceExiste = FaceNS::searchFace(facesBuff[iMax], sumNodesBuff[iMax], facesBuff, sumNodesBuff, 1, iMax);
    // Create or attach face
    if (indexFaceExiste == -1) {
      faces[iMax] = new FacePoint(facesBuff[iMax][0]); //no need to sort here
      faces[iMax]->construitFace(nodes, m_numNodes[nodeAutre], this);
      iMax++;
    }
    else {
      faces[indexFaceExiste]->addElementNeighbor(this);
    }
  }
}

//***********************************************************************

void ElementSegment::construitFacesSimplifie(int& iMax, int** facesBuff, int* sumNodesBuff)
{
  //2 vertex-kind faces to treat
  int indexFaceExiste(-1);
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      facesBuff[iMax][0] = m_numNodes[0];
      break;
    case 1:
      facesBuff[iMax][0] = m_numNodes[1];
      break;
    }
    sumNodesBuff[iMax] = facesBuff[iMax][0];
    // Checking face existence
    indexFaceExiste = FaceNS::searchFace(facesBuff[iMax], sumNodesBuff[iMax], facesBuff, sumNodesBuff, 1, iMax);
    // Create face or attach it
    if (indexFaceExiste == -1) {
      iMax++;
    }
  }
}

//***********************************************************************

void ElementSegment::attributFaceLimite(FaceNS** faces, const int& indexMaxFaces)
{
  int indexFaceExiste(0);
  FaceSegment face(m_numNodes[0], m_numNodes[1]);
  if (face.faceExists(faces, indexMaxFaces, indexFaceExiste)) {
    faces[indexFaceExiste]->addElementNeighborLimite(this);
  }
  else {
    Errors::errorMessage("Issue when attributing boundary faces to segment element");
  }
}

//***********************************************************************

void ElementSegment::attributFaceCommunicante(FaceNS** faces, const int& indexMaxFaces, const int& numberNodesInternal)
{
  int indexFaceExiste(0);
  //Check first face
  if (m_numNodes[0] < numberNodesInternal) {
    FacePoint face(m_numNodes[0]);
    if (face.faceExists(faces, indexMaxFaces, indexFaceExiste)) {
      faces[indexFaceExiste]->addElementNeighborLimite(this);
      faces[indexFaceExiste]->setEstComm(true);
    }
  }
  //Check second face
  if (m_numNodes[1] < numberNodesInternal) {
    FacePoint face(m_numNodes[1]);
    if (face.faceExists(faces, indexMaxFaces, indexFaceExiste)) {
      faces[indexFaceExiste]->addElementNeighborLimite(this);
      faces[indexFaceExiste]->setEstComm(true);
    }
  }
}

//***********************************************************************

int ElementSegment::compteFaceCommunicante(std::vector<int*>& facesBuff, std::vector<int>& sumNodesBuff)
{
  //2 vertex-kind faces to treat
  int indexFaceExiste(-1), numberFacesCommunicante(0);
  int vertex;
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      vertex = m_numNodes[0];
      break;
    case 1:
      vertex = m_numNodes[1];
      break;
    }
    int iMax = sumNodesBuff.size();
    //Search if face exists
    indexFaceExiste = FaceNS::searchFace(&vertex, vertex, facesBuff, sumNodesBuff, 1, iMax);
    if (indexFaceExiste != -1) {
      numberFacesCommunicante++;
    }
  }
  return numberFacesCommunicante;
}

//***********************************************************************
//New version more efficient
int ElementSegment::compteFaceCommunicante(int& iMax, int** facesBuff, int* sumNodesBuff)
{
  //2 vertex-kind faces to treat
  int indexFaceExiste(-1), numberFacesCommunicante(0);
  int vertex;
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      vertex = m_numNodes[0];
      break;
    case 1:
      vertex = m_numNodes[1];
      break;
    }
    //Search if face exists
    indexFaceExiste = FaceNS::searchFace(&vertex, vertex, facesBuff, sumNodesBuff, 1, iMax);
    if (indexFaceExiste != -1) {
      numberFacesCommunicante++;
    }
  }
  return numberFacesCommunicante;
}

//***********************************************************************
