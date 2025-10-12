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

#include "MUSGmsh.h"

//***********************************************************************

MUSGmsh::MUSGmsh(const std::string& meshFile, const std::string& meshExtension) : MeshUnStruct(meshFile, meshExtension) {}

//***********************************************************************

MUSGmsh::~MUSGmsh() {}

//***********************************************************************

std::string MUSGmsh::readVersion(const std::string& meshFile)
{
  try {
    std::string pathMeshFile(meshFile);
    std::string currentLine;
    std::ifstream mesh(pathMeshFile.c_str(), std::ios::in);
    if (!mesh) {
      throw ErrorMeshNS("mesh file not found : " + pathMeshFile, __FILE__, __LINE__);
    }

    // Exclusion of unwanted sections
    std::string fileVersion;
    while (currentLine != "$MeshFormat") {
      mesh >> currentLine; //To exclude the newline caracter
      if (mesh.eof()) {
        throw ErrorMeshNS("mesh file format is not compatible, see mesh file: " + meshFile, __FILE__, __LINE__);
      }
    }

    // Get file version
    mesh >> fileVersion; //To exclude the newline caracter

    mesh.close();
    return fileVersion;
  }
  catch (ErrorMeshNS&) {
    throw;
  }
}

//***********************************************************************
