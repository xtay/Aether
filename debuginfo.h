#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <stdarg.h>
#include <vector>
#include <QString>
#include <qgl.h>
#include <QGLViewer/vec.h>

using qglviewer::Vec;

using std::vector;

class DebugInfo
{
public:
    DebugInfo();

    void addLine(const char *fmt, ...);
    void addVertex3f(float x, float y, float z);
    void addVertex3fv(Vec v);

    void displayInfo(QGLWidget* qw);

private:

    vector<QString> vInfo_;
    vector<Vec> vVertex_;

    QString curStr_;

    static const int offsetVertical  = 20;
    static const int offsetHorizonal = 10;
    static const int lineInterval    = 20;
};

#endif // DEBUGINFO_H
