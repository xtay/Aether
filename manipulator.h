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
