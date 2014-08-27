#include "meshmodel.h"
#include <QGLViewer/vec.h>
#include <fstream>
#include <string>
#include <assert.h>
using namespace std;
using namespace qglviewer;

MeshModel::MeshModel(char* filename)
{
    ifstream inFile;
    inFile.open(filename);
    readMesh(inFile);
}

MeshModel::~MeshModel()
{
}

void MeshModel::readMesh(ifstream &inFile)
{
    assert(inFile.is_open());
    string meshFormatStr;
    int meshFormatNum;
    string dimensionStr;
    int dimensionNum;
    string vertexStr;
    int vertexNum;
    string triangleStr;
    int triangleNum;
    string tetraStr;
    int tetraNum;
    inFile >> meshFormatStr >> meshFormatNum;
    inFile >> dimensionStr >> dimensionNum;
    inFile >> vertexStr >> vertexNum;
    for (int i=0; i < vertexNum; ++i) {
        double tmpX, tmpY, tmpZ;
        int ref;
        inFile >> tmpX >> tmpY >> tmpZ >> ref;
        vex_.push_back(Vec(tmpX, tmpY, tmpZ));
    }
    inFile >> tetraStr >> tetraNum;
    for (int i=0; i < tetraNum; ++i) {
        Tetra tmp;
        int ref;
        inFile >> tmp[0] >> tmp[1] >> tmp[2] >> tmp[3] >> ref;
        tmp[0] -= 1;
        tmp[1] -= 1;
        tmp[2] -= 1;
        tmp[3] -= 1;
        tet_.push_back(tmp);
    }
    inFile >> triangleStr >> triangleNum;
    for (int i=0; i < triangleNum; ++i) {
        Triangle tmp;
        int ref;
        inFile >> tmp[0] >> tmp[1] >> tmp[2] >> ref;
        tmp[0] -= 1;
        tmp[1] -= 1;
        tmp[2] -= 1;
        tri_.push_back(tmp);
    }
}

void MeshModel::drawModel()
{
    glBegin(GL_TRIANGLES);
    //boundary
    for (vector<Triangle>::iterator iter = tri_.begin(); iter != tri_.end(); ++iter) {
        Vec p0 = vex_[(*iter)[0]];
        Vec p1 = vex_[(*iter)[1]];
        Vec p2 = vex_[(*iter)[2]];
        Vec normal = cross(p1-p0, p2-p1).unit();
        glNormal3dv(normal);
        glVertex3dv(p0);
        glVertex3dv(p1);
        glVertex3dv(p2);
    }
    //innerside
    for (vector<Tetra>::iterator iter = tet_.begin(); iter != tet_.end(); ++iter) {
        Vec p0 = vex_[(*iter)[0]];
        Vec p1 = vex_[(*iter)[1]];
        Vec p2 = vex_[(*iter)[2]];
        Vec p3 = vex_[(*iter)[3]];
        Vec normal;
        //face 0, 1, 2
        normal = cross(p1-p0, p2-p1).unit();
        glNormal3dv(normal);
        glVertex3dv(p0);
        glVertex3dv(p1);
        glVertex3dv(p2);
        //face 0, 1, 3
        normal = cross(p1-p0, p3-p1).unit();
        glNormal3dv(normal);
        glVertex3dv(p0);
        glVertex3dv(p1);
        glVertex3dv(p3);
        //face 0, 2, 3
        normal = cross(p2-p0, p3-p2).unit();
        glNormal3dv(normal);
        glVertex3dv(p0);
        glVertex3dv(p2);
        glVertex3dv(p3);
        //face 1, 2, 3
        normal = cross(p2-p1, p3-p2).unit();
        glNormal3dv(normal);
        glVertex3dv(p1);
        glVertex3dv(p2);
        glVertex3dv(p3);
    }
    glEnd();
}
