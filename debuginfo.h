#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <stdarg.h>
#include <vector>
#include <QString>
#include <qgl.h>

using std::vector;

class DebugInfo
{
public:
    DebugInfo();

    void addLine(const char *fmt, ...);

    void displayInfo(QGLWidget* qw);

private:

    vector<QString> vInfo_;

    QString curStr_;

    static const int offsetVertical  = 20;
    static const int offsetHorizonal = 10;
    static const int lineInterval    = 20;
};

#endif // DEBUGINFO_H
