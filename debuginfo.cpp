#include "debuginfo.h"
#include "GL/gl.h"
using qglviewer::Vec;

DebugInfo::DebugInfo()
{
}

void DebugInfo::addLine(const char * fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    curStr_.vsprintf(fmt, vargs);
    vInfo_.push_back(curStr_);

    va_end(vargs);
    return;
}

void DebugInfo::addVertex3f(float x, float y, float z)
{
    vVertex_.push_back(Vec(x, y, z));
}

void DebugInfo::addVertex3fv(qglviewer::Vec v)
{
    vVertex_.push_back(v);
}

void DebugInfo::displayInfo(QGLWidget* qw)
{
    //display text info
    if (!vInfo_.empty()) {
        int v(offsetVertical);
        for (vector<QString>::const_iterator iter = vInfo_.begin(); iter != vInfo_.end(); ++iter) {
            qw->renderText(offsetHorizonal, v, *iter);
            v += lineInterval;
        }
    }

    //draw some vertex
    if (!vVertex_.empty()) {
        glBegin(GL_POINTS);
        glColor3f(0.1, 0.2, 0.3);
        for (vector<Vec>::const_iterator iter = vVertex_.begin(); iter != vVertex_.end(); ++iter) {
            glVertex3f(iter->x, iter->y, iter->z);
        }
        glEnd();
    }

    vInfo_.clear();
    vVertex_.clear();

    return;
}
