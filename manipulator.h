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
#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QGLViewer/mouseGrabber.h>
#include <QGLViewer/frame.h>
#include <QGLViewer/vec.h>
#include <QGLViewer/quaternion.h>
#include <debuginfo.h>

using namespace qglviewer;

class Manipulator : public MouseGrabber
{
public:
    enum ManipulateMode {MOVE, ROTATE};

    Manipulator(Frame& fr);
    Manipulator(Frame& fr, const Vec& dir, ManipulateMode mode);
    virtual void checkIfGrabsMouse(int x, int y, const Camera * const camera);
    virtual void mousePressEvent(QMouseEvent * const event, Camera * const camera);
    virtual void mouseMoveEvent(QMouseEvent * const event, Camera * const camera);
    virtual void mouseReleaseEvent(QMouseEvent * const event, Camera * const camera);

    void setManipulateDirection(const Vec& dir, bool isLocal);
    void setManipulatedFrame(Frame& fr);
    void setManipulateMode(ManipulateMode mode);
    void setManipulateLength(float length);
    void draw(float* color);
    void drawAxis(float length);

    const Vec position() {return pfr_->position();}
    const Quaternion orientation() {return pfr_->orientation();}

    DebugInfo dbi;

private:
    void drawArrow(float* color);
    void drawArrow();
    void drawCircle(float* color);

    Frame* pfr_;
    Vec direction_;
    ManipulateMode action_;
    float length_;
    bool isLocalDirection_;
    bool keepsGrabbingMouse_;
    bool moved_;
    QPoint prevCurPoint_;
    Vec prevObjPos_;
    Vec prevObjLookAt_;
    Quaternion prevObjOrien_;
};

#endif // MANIPULATOR_H
