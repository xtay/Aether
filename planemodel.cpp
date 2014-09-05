/******************************************************************************
    Aether, a 3D model viewer based on Qt and libQGLViewer
    Copyright (C) 2014  Xiangxin Zhang

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/
#include "planemodel.h"
#include "GL/gl.h"
#include <fstream>
#include <ios>
using namespace std;

PlaneModel::PlaneModel()
{
}

PlaneModel::~PlaneModel()
{
}

    float origin[200][200][200];
    GLbyte color[200][200][200][1];
void PlaneModel::initModel()
{
    ifstream inFile;
    inFile.open("../Aether/shrink.dat", ios_base::binary);
    inFile.read(static_cast<char*>(static_cast<void*>(&origin[0][0][0])), sizeof(origin));

    float min, max, tmp;
    min = max = tmp = origin[0][0][0];
    for (int i = 0; i < 200; ++i) {
        for (int j = 0; j < 200; ++j) {
            for (int k = 0; k < 200; ++k) {
                tmp = origin[i][j][k];
                min = min < tmp ? min : tmp;
                max = max > tmp ? max : tmp;
            }
        }
    }
    cout << min << endl;
    cout << max << endl;

    for (int i = 0; i < 200; ++i) {
        for (int j = 0; j < 200; ++j) {
            for (int k = 0; k < 200; ++k) {
                tmp = (origin[i][j][k]-min)/(max-min)*256;
                color[i][j][k][0] = static_cast<GLbyte>(tmp);
                //color[i][j][k][1] = static_cast<GLbyte>(j);
                //color[i][j][k][2] = static_cast<GLbyte>(k);
                //color[i][j][k][3] = static_cast<GLbyte>(0);
            }
        }
    }
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //no repeat
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, 200, 200, 200, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, color);
}

void PlaneModel::drawModel()
{
    glEnable(GL_TEXTURE_3D);
    glBegin(GL_QUADS);
        glNormal3f(0.0, 0.0, -1.0);
        glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
        glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(0.0, 1.0, 0.0);
        glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
        glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_3D);
}
