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
