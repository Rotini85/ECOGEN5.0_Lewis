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

#ifndef ELEMENTQUADRANGLE_H
#define ELEMENTQUADRANGLE_H

#include "ElementNS.h"
#include "FaceSegment.h"
#include "FaceQuadrangle.h"

class ElementQuadrangle : public ElementNS
{
  public:
    ElementQuadrangle();
    ~ElementQuadrangle() override;

    void construitFaces(const Coord* nodes, FaceNS** faces, int& indexMaxFaces, int** facesBuff, int* sumNodesBuff) override;
    void construitFacesSimplifie(int& iMax, int** facesBuff, int* sumNodesBuff) override;
    void attributFaceLimite(FaceNS** faces, const int& indexMaxFaces) override;
    void attributFaceCommunicante(FaceNS** faces, const int& indexMaxFaces, const int& numberNodesInternal) override;
    int compteFaceCommunicante(std::vector<int*>& faces, std::vector<int>& sumNodesBuff) override;
    int compteFaceCommunicante(int& iMax, int** faces, int* sumNodesBuff) override;

  private:
    void computeVolume(const Coord* nodes) override;
    void computeLCFL(const Coord* nodes) override;

    static const int TYPEGMSH;
    static const int NUMBERNODES;
    static const int NUMBERFACES; /* Here it is the number of segments */
    static const int TYPEVTK;
};

#endif // ELEMENTQUADRANGLE_H
