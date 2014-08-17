#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedFrame.h>
#include <vector>
#include <map>
#include <auto_ptr.h>
#include "graphelement.h"
#include "manipulator.h"

using qglviewer::Frame;

using std::map;

class ModelViewer : public QGLViewer
{
public:
    explicit ModelViewer(QWidget* parent);
    ~ModelViewer();
    int addElement(BaseModel* pbm);
    int addElement(BaseModel* pbm, const qglviewer::Frame &pfr);

protected:
    virtual void init();
    virtual void drawWithNames();
    virtual void draw();

    //overload these three functions to make sure
    //that select action won't surpress the grab action
    virtual void select(const QPoint &point);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //some customized shortcut
    virtual void keyPressEvent(QKeyEvent *event);

    //this might be help full...
    virtual QString helpString() const;

    //this function surpress the QGLViewer::drawGrid()
    void drawGrid(float interval, int nbSubdivision);

private:
    void enableManipulator();
    void disableManipulator();

    int elementName_;
    map<int, GraphElement> mGraphElem_;
    Frame worldFrame_;

    Manipulator mpx_;
    Manipulator mpy_;
    Manipulator mpz_;
    bool isLocalManipulate_;

    DebugInfo dbi_;
};

#endif // MODELVIEWER_H
