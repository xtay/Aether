#include "planemodel.h"

PlaneModel::PlaneModel()
{
}

PlaneModel::~PlaneModel()
{
}

void PlaneModel::drawModel()
{
    glBegin(GL_QUADS);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(-1.0, 1.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
    glEnd();
}
