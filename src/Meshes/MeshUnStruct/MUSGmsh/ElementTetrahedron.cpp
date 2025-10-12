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

#include "ElementTetrahedron.h"

const int ElementTetrahedron::TYPEGMSH    = 4;
const int ElementTetrahedron::NUMBERNODES = 4;
const int ElementTetrahedron::NUMBERFACES = 4; /* ici il s'agit de triangles*/
const int ElementTetrahedron::TYPEVTK     = 10;

//***********************************************************************

ElementTetrahedron::ElementTetrahedron() : ElementNS(TYPEGMSH, NUMBERNODES, NUMBERFACES, TYPEVTK) {}

//***********************************************************************

ElementTetrahedron::~ElementTetrahedron() {}

//***********************************************************************

void ElementTetrahedron::computeVolume(const Coord* nodes)
{
  //AF//TODO// *6 ??
  Coord v1(nodes[1] - nodes[0]), v2(nodes[2] - nodes[0]), v3(nodes[3] - nodes[0]);
  m_volume = std::fabs(Coord::determinant(v1, v2, v3)) / 6.; //volume du tetradre
}

//***********************************************************************

void ElementTetrahedron::computeLCFL(const Coord* nodes)
{
  Coord vec;
  //AF//TODO// remove division by 3 or hard impl ??
  m_lCFL = 1e10;
  vec    = ((nodes[0] + nodes[1] + nodes[2]) / 3.) - m_position;
  m_lCFL = std::min(m_lCFL, vec.norm());
  vec    = ((nodes[1] + nodes[2] + nodes[3]) / 3.) - m_position;
  m_lCFL = std::min(m_lCFL, vec.norm());
  vec    = ((nodes[2] + nodes[3] + nodes[0]) / 3.) - m_position;
  m_lCFL = std::min(m_lCFL, vec.norm());
  vec    = ((nodes[3] + nodes[0] + nodes[1]) / 3.) - m_position;
  m_lCFL = std::min(m_lCFL, vec.norm());
}

//***********************************************************************

void ElementTetrahedron::construitFaces(const Coord* nodes, FaceNS** faces, int& iMax, int** facesBuff, int* sumNodesBuff)
{
  //4 faces a traiter de type triangle
  int indexFaceExiste(-1);
  int nodeAutre(0);
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      facesBuff[iMax][0] = m_numNodes[0];
      facesBuff[iMax][1] = m_numNodes[1];
      facesBuff[iMax][2] = m_numNodes[2];
      nodeAutre          = 3;
      break;
    case 1:
      facesBuff[iMax][0] = m_numNodes[1];
      facesBuff[iMax][1] = m_numNodes[2];
      facesBuff[iMax][2] = m_numNodes[3];
      nodeAutre          = 0;
      break;
    case 2:
      facesBuff[iMax][0] = m_numNodes[2];
      facesBuff[iMax][1] = m_numNodes[3];
      facesBuff[iMax][2] = m_numNodes[0];
      nodeAutre          = 1;
      break;
    case 3:
      facesBuff[iMax][0] = m_numNodes[3];
      facesBuff[iMax][1] = m_numNodes[0];
      facesBuff[iMax][2] = m_numNodes[1];
      nodeAutre          = 2;
      break;
    }
    sumNodesBuff[iMax] = facesBuff[iMax][0] + facesBuff[iMax][1] + facesBuff[iMax][2];
    std::sort(facesBuff[iMax], facesBuff[iMax] + 3); //Sort nodes
    // Checking face existence
    indexFaceExiste = FaceNS::searchFace(facesBuff[iMax], sumNodesBuff[iMax], facesBuff, sumNodesBuff, 3, iMax);
    //Creation face ou rattachement
    if (indexFaceExiste == -1) {
      faces[iMax] = new FaceTriangle(facesBuff[iMax][0], facesBuff[iMax][1], facesBuff[iMax][2], 0); //pas besoin du tri ici
      faces[iMax]->construitFace(nodes, m_numNodes[nodeAutre], this);
      iMax++;
    }
    else {
      faces[indexFaceExiste]->addElementNeighbor(this);
    }
  }
}

//***********************************************************************

void ElementTetrahedron::construitFacesSimplifie(int& iMax, int** facesBuff, int* sumNodesBuff)
{
  //4 faces a traiter de type triangle
  int indexFaceExiste(-1);
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      facesBuff[iMax][0] = m_numNodes[0];
      facesBuff[iMax][1] = m_numNodes[1];
      facesBuff[iMax][2] = m_numNodes[2];
      break;
    case 1:
      facesBuff[iMax][0] = m_numNodes[1];
      facesBuff[iMax][1] = m_numNodes[2];
      facesBuff[iMax][2] = m_numNodes[3];
      break;
    case 2:
      facesBuff[iMax][0] = m_numNodes[2];
      facesBuff[iMax][1] = m_numNodes[3];
      facesBuff[iMax][2] = m_numNodes[0];
      break;
    case 3:
      facesBuff[iMax][0] = m_numNodes[3];
      facesBuff[iMax][1] = m_numNodes[0];
      facesBuff[iMax][2] = m_numNodes[1];
      break;
    }
    sumNodesBuff[iMax] = facesBuff[iMax][0] + facesBuff[iMax][1] + facesBuff[iMax][2];
    std::sort(facesBuff[iMax], facesBuff[iMax] + 3); //Tri des nodes
    // Checking face existence
    indexFaceExiste = FaceNS::searchFace(facesBuff[iMax], sumNodesBuff[iMax], facesBuff, sumNodesBuff, 3, iMax);
    //Creation face ou rattachement
    if (indexFaceExiste == -1) {
      iMax++;
    }
  }
}

//***********************************************************************

void ElementTetrahedron::attributFaceCommunicante(FaceNS** faces, const int& indexMaxFaces, const int& numberNodesInternal)
{
  int indexFaceExiste(0);
  //Verification face 1 :
  if (m_numNodes[0] < numberNodesInternal && m_numNodes[1] < numberNodesInternal && m_numNodes[2] < numberNodesInternal) {
    FaceTriangle face(m_numNodes[0], m_numNodes[1], m_numNodes[2]);
    if (face.faceExists(faces, indexMaxFaces, indexFaceExiste)) {
      faces[indexFaceExiste]->addElementNeighborLimite(this);
      faces[indexFaceExiste]->setEstComm(true);
    }
  }
  //Verification face 2 :
  if (m_numNodes[1] < numberNodesInternal && m_numNodes[2] < numberNodesInternal && m_numNodes[3] < numberNodesInternal) {
    FaceTriangle face(m_numNodes[1], m_numNodes[2], m_numNodes[3]);
    if (face.faceExists(faces, indexMaxFaces, indexFaceExiste)) {
      faces[indexFaceExiste]->addElementNeighborLimite(this);
      faces[indexFaceExiste]->setEstComm(true);
    }
  }
  //Verification face 3 :
  if (m_numNodes[2] < numberNodesInternal && m_numNodes[3] < numberNodesInternal && m_numNodes[0] < numberNodesInternal) {
    FaceTriangle face(m_numNodes[2], m_numNodes[3], m_numNodes[0]);
    if (face.faceExists(faces, indexMaxFaces, indexFaceExiste)) {
      faces[indexFaceExiste]->addElementNeighborLimite(this);
      faces[indexFaceExiste]->setEstComm(true);
    }
  }
  //Verification face 4 :
  if (m_numNodes[3] < numberNodesInternal && m_numNodes[0] < numberNodesInternal && m_numNodes[1] < numberNodesInternal) {
    FaceTriangle face(m_numNodes[3], m_numNodes[0], m_numNodes[1]);
    if (face.faceExists(faces, indexMaxFaces, indexFaceExiste)) {
      faces[indexFaceExiste]->addElementNeighborLimite(this);
      faces[indexFaceExiste]->setEstComm(true);
    }
  }
}

//***********************************************************************

int ElementTetrahedron::compteFaceCommunicante(std::vector<int*>& facesBuff, std::vector<int>& sumNodesBuff)
{
  //4 faces a traiter de type triangle
  int indexFaceExiste(-1), numberFacesCommunicante(0);
  int face[3], sumNodes;
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      face[0] = m_numNodes[0];
      face[1] = m_numNodes[1];
      face[2] = m_numNodes[2];
      break;
    case 1:
      face[0] = m_numNodes[1];
      face[1] = m_numNodes[2];
      face[2] = m_numNodes[3];
      break;
    case 2:
      face[0] = m_numNodes[2];
      face[1] = m_numNodes[3];
      face[2] = m_numNodes[0];
      break;
    case 3:
      face[0] = m_numNodes[3];
      face[1] = m_numNodes[0];
      face[2] = m_numNodes[1];
      break;
    }
    int iMax = sumNodesBuff.size();
    sumNodes = face[0] + face[1] + face[2];
    std::sort(face, face + 3);
    //Recherche existance faces
    indexFaceExiste = FaceNS::searchFace(face, sumNodes, facesBuff, sumNodesBuff, 3, iMax);
    if (indexFaceExiste != -1) {
      numberFacesCommunicante++;
    }
  }
  return numberFacesCommunicante;
}

//***********************************************************************
//Nouvelle version plus efficace
int ElementTetrahedron::compteFaceCommunicante(int& iMax, int** facesBuff, int* sumNodesBuff)
{
  //4 faces a traiter de type triangle
  int indexFaceExiste(-1), numberFacesCommunicante(0);
  int face[3], sumNodes;
  for (int i = 0; i < NUMBERFACES; i++) {
    switch (i) {
    case 0:
      face[0] = m_numNodes[0];
      face[1] = m_numNodes[1];
      face[2] = m_numNodes[2];
      break;
    case 1:
      face[0] = m_numNodes[1];
      face[1] = m_numNodes[2];
      face[2] = m_numNodes[3];
      break;
    case 2:
      face[0] = m_numNodes[2];
      face[1] = m_numNodes[3];
      face[2] = m_numNodes[0];
      break;
    case 3:
      face[0] = m_numNodes[3];
      face[1] = m_numNodes[0];
      face[2] = m_numNodes[1];
      break;
    }
    sumNodes = face[0] + face[1] + face[2];
    std::sort(face, face + 3);
    //Recherche existance faces
    indexFaceExiste = FaceNS::searchFace(face, sumNodes, facesBuff, sumNodesBuff, 3, iMax);
    if (indexFaceExiste != -1) {
      numberFacesCommunicante++;
    }
  }
  return numberFacesCommunicante;
}

//***********************************************************************
