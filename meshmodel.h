#ifndef MESHMODEL_H
#define MESHMODEL_H

#include "basemodel.h"
using std::vector;
using std::ifstream;
using qglviewer::Vec;

struct Triangle {
    int t_[3];
    int& operator[](int i) { return t_[i]; }
    operator int* () { return t_; }
};

struct Tetra{
    int t_[4];
    int& operator[](int i) { return t_[i]; }
    operator int* () { return t_; }
};

class MeshModel : public BaseModel
{
public:
    MeshModel(char *filename);
    virtual ~MeshModel();

    virtual void drawModel();

private:
    void readMesh(ifstream&);

    vector<Vec> vex_;
    vector<Triangle> tri_;
    vector<Tetra> tet_;
};

#endif // MESHMODEL_H
