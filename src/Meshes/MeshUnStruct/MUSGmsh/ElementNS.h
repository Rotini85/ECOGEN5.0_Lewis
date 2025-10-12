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

#ifndef ELEMENTNS_H
#define ELEMENTNS_H

#include "../../Element.h"

class FaceNS; //Predeclaration de la classe Face pour pouvoir inclure Face.h
#include "FaceNS.h"

class ElementNS : public Element
{
  public:
    ElementNS();
    ElementNS(const int& typeGmsh, const int& numberNodes, const int& numberFaces, const int& typeVTK);
    ~ElementNS() override;

    void construitElement(const int* numNodes,
                          const Coord* nodes,
                          const int numberEntitePhysique,
                          const int numberEntiteGeometrique,
                          int& indexElement); /*Compute element properties*/

    void construitElementParallele(const Coord* nodes); /*Compute element properties*/
    virtual void attributFaceLimite(FaceNS** /*faces*/, const int& /*indexMaxFaces*/)
    {
      Errors::errorMessage("attributFaceLimite non prevu pour le type d element demande");
    };
    virtual void attributFaceCommunicante(FaceNS** /*faces*/, const int& /*indexMaxFaces*/, const int& /*numberNodesInternal*/)
    {
      Errors::errorMessage("attributFaceCommunicante non prevu pour le type d element demande");
    };
    virtual void construitFaces(const Coord* /*nodes*/, FaceNS** /*faces*/, int& /*indexMaxFaces*/, int** /*facesBuff*/, int* /*sumNodesBuff*/)
    {
      Errors::errorMessage("construitFaces non prevu pour le type d element demande");
    }; //Pour tests
    virtual void construitFacesSimplifie(int& /*iMax*/, int** /*facesBuff*/, int* /*sumNodesBuff*/)
    {
      Errors::errorMessage("construitFacesSimplifie non prevu pour le type d element demande");
    };
    virtual int compteFaceCommunicante(std::vector<int*>& /*faces*/, std::vector<int>& /*sumNodesBuff*/)
    {
      Errors::errorMessage("compteFaceCommunicante non prevu pour le type d element demande");
      return 0;
    };
    virtual int compteFaceCommunicante(int& /*iMax*/, int** /*faces*/, int* /*sumNodesBuff*/)
    {
      Errors::errorMessage("compteFaceCommunicante non prevu pour le type d element demande");
      return 0;
    };
    /*  void removeCPUOthers(const int& numCPU);*/
    void removeCPUOthers(std::vector<int>& numCPU);

    //Accesseurs
    void setIndex(int& index);
    void setAppartenancePhysique(int& appartenancePhysique);
    void setNumNode(int* numNodes);
    void setNumNode(int& node, int& numNode);
    void setIsFantome(bool isFantome);
    void setIsCommunicant(bool isCommunicant);
    void setAppartenanceCPU(const int* numCPU, const int& numberCPU);

    const int& getIndex() const override { return m_index; };
    const int& getNumberNodes() const { return m_numberNodes; };
    const int& getNumberFaces() const { return m_numberFaces; };
    const int& getTypeGmsh() const { return m_typeGmsh; };
    const int& getTypeVTK() const { return m_typeVTK; };
    const int& getNumNode(int& node) const { return m_numNodes[node]; };
    const int& getAppartenancePhysique() const override { return m_appartenancePhysique; };
    const int& getAppartenanceGeometrique() const { return m_appartenanceGeometrique; };
    const int& getCPU() const { return m_CPU; };
    const int& getNumberOthersCPU() const { return m_numberOtherCPU; };
    const int& getAutreCPU(const int& autreCPU) const;
    void printInfo() const override;

    const bool& isFantome() const { return m_isFantome; };
    const bool& isCommunicant() const { return m_isCommunicant; };

  protected:
    virtual void computeVolume(const Coord* /*nodes*/) {};
    virtual void computeLCFL(const Coord* /*nodes*/) {};

    int m_index;

    int m_typeGmsh;
    int m_typeVTK;
    int m_numberNodes;
    int m_numberFaces;
    int m_appartenancePhysique;
    int m_appartenanceGeometrique;
    bool m_isFantome;
    bool m_isCommunicant;
    int m_CPU; /* Index of the CPU owning the element (i.e. where the element is physically located) */
    int m_numberOtherCPU;
    int* m_otherCPU; /* Indices of the CPUs on which the element is phantom (overlap) */
    int* m_numNodes; /* Mapping with mesh nodes arrays*/
};

#endif // ELEMENTNS_H
