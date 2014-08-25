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
#include "modelviewer.h"
#include "debuginfo.h"
#include "boxmodel.h"
#include "planemodel.h"
#include <QKeyEvent>
#include <QGLViewer/manipulatedCameraFrame.h>

using namespace std;
using namespace qglviewer;

ModelViewer::ModelViewer(QWidget *parent)
    :QGLViewer(parent),
      elementName_(1000),
      mpx_(worldFrame_),
      mpy_(worldFrame_),
      mpz_(worldFrame_),
      isLocalManipulate_(false)
{
}

ModelViewer::~ModelViewer()
{
}

int ModelViewer::addElement(BaseModel *pbm)
{
    mGraphElem_.insert(make_pair(elementName_++, GraphElement(pbm)));
    return elementName_ - 1;
}

int ModelViewer::addElement(BaseModel *pbm, const Frame& fr)
{
    mGraphElem_.insert(make_pair(elementName_++, GraphElement(pbm, fr)));
    return elementName_ - 1;
}

void ModelViewer::init()
{
    glPointSize(8.0);
    Frame fr(Vec(1,-1,1), Quaternion(Vec(1,0,0),Vec(1,0,0)));
    Frame frRef(Vec(-2,2,2), Quaternion(Vec(1,0,0),Vec(1,0,0)));
    ModelViewer::addElement(new BoxModel());
    ModelViewer::addElement(new BoxModel(), fr);
    ModelViewer::addElement(new BoxModel(), frRef);
    ModelViewer::addElement(new PlaneModel());

    setMouseBinding(Qt::NoModifier, Qt::RightButton, CAMERA, NO_MOUSE_ACTION);
    setMouseBinding(Qt::NoModifier, Qt::MidButton, CAMERA, NO_MOUSE_ACTION);
    setMouseBinding(Qt::NoModifier, Qt::LeftButton, SELECT);
    setMouseBinding(Qt::AltModifier, Qt::LeftButton, CAMERA, ROTATE);
    setMouseBinding(Qt::AltModifier, Qt::LeftButton, ALIGN_CAMERA, true);
    setMouseBinding(Qt::AltModifier, Qt::MidButton, CAMERA, TRANSLATE);
    setMouseBinding(Qt::AltModifier, Qt::RightButton, CAMERA, ZOOM);

    setShortcut(DRAW_GRID, 0);//disable the default qglviewer grid
    setShortcut(DRAW_AXIS, 0);//disable the default qglviewer axis, use manipulator instead
    setShortcut(CAMERA_MODE, 0);//disable camera fly
    setShortcut(EXIT_VIEWER, Qt::CTRL+Qt::Key_Q);
    setShortcut(DISPLAY_FPS, Qt::CTRL+Qt::Key_F);

    //set default camera position and orientation
    camera()->setPosition(Vec(20.0, 20.0, 14.0));
    camera()->setUpVector(Vec(0.0, 0.0, 1.0));
    camera()->lookAt(Vec(0.0, 0.0, 0.0));
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    setSceneRadius(30.0);
  // Restore previous viewer state.
  //restoreStateFromFile();

  // Opens help window
  //help();
}

void ModelViewer::drawWithNames()
{
    //draw selectable elements
    for(map<int, GraphElement>::iterator iter = mGraphElem_.begin(); iter != mGraphElem_.end(); ++iter)
    {
        glPushName(iter->first);
        iter->second.drawElement(false);
        glPopName();
    }
}

void ModelViewer::draw()
{
    drawGrid(1.0, 10);

    //draw Elements
    for(map<int, GraphElement>::iterator iter = mGraphElem_.begin(); iter != mGraphElem_.end(); ++iter)
    {
        iter->second.drawElement(selectedName() == iter->first);
    }

    //draw Manipulator
    glDisable(GL_DEPTH_TEST);
    if (selectedName() != -1) {
        enableManipulator();
    }
    else {
        disableManipulator();
    }
    glEnable(GL_DEPTH_TEST);

    //render debug information on screen
#ifdef QT_DEBUG
#endif
}

void ModelViewer::select(const QPoint &point)
{
    if (mpx_.grabsMouse())
        mpx_.checkIfGrabsMouse(point.x(), point.y(), camera());
    else if (mpy_.grabsMouse())
        mpy_.checkIfGrabsMouse(point.x(), point.y(), camera());
    else if (mpz_.grabsMouse())
        mpz_.checkIfGrabsMouse(point.x(), point.y(), camera());
    else
        QGLViewer::select(point);
}

void ModelViewer::mousePressEvent(QMouseEvent *event)
{
    QGLViewer::mousePressEvent(event);
    if(mpx_.grabsMouse())
        mpx_.mousePressEvent(event, camera());
    if(mpy_.grabsMouse())
        mpy_.mousePressEvent(event, camera());
    if(mpz_.grabsMouse())
        mpz_.mousePressEvent(event, camera());
}

void ModelViewer::mouseReleaseEvent(QMouseEvent *event)
{
    QGLViewer::mouseReleaseEvent(event);
    if(mpx_.grabsMouse())
        mpx_.mouseReleaseEvent(event, camera());
    if(mpy_.grabsMouse())
        mpy_.mouseReleaseEvent(event, camera());
    if(mpz_.grabsMouse())
        mpz_.mouseReleaseEvent(event, camera());
}

QString ModelViewer::helpString() const
{
  QString text("<h2>S i m p l e V i e w e r</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
  text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
  text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
  text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
  text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
  text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}

void ModelViewer::keyPressEvent(QKeyEvent *event)
{
    bool handeled = false;
    //get event modifiers key
    const Qt::KeyboardModifiers modifires = event->modifiers();

    //W key to set the manipulators in MOVE mode
    if ((event->key() == Qt::Key_W) && modifires == Qt::NoButton) {
        mpx_.setManipulateMode(Manipulator::MOVE);
        mpy_.setManipulateMode(Manipulator::MOVE);
        mpz_.setManipulateMode(Manipulator::MOVE);
        updateGL();
        handeled = true;
    }

    //E key to set the manipulators in ROTATE mode
    if ((event->key() == Qt::Key_E) && modifires == Qt::NoButton) {
        mpx_.setManipulateMode(Manipulator::ROTATE);
        mpy_.setManipulateMode(Manipulator::ROTATE);
        mpz_.setManipulateMode(Manipulator::ROTATE);
        updateGL();
        handeled = true;
    }

    if ((event->key() == Qt::Key_G) && modifires == Qt::NoButton) {
        camera()->setPosition(Vec(20.0, 20.0, 14.0));
        camera()->setUpVector(Vec(0.0, 0.0, 1.0));
        camera()->lookAt(Vec(0.0, 0.0, 0.0));
        camera()->frame()->stopSpinning();
        updateGL();
        handeled = true;
    }

    if ((event->key() == Qt::Key_Delete) && modifires == Qt::NoButton) {
        //completely delete the selected element
        disableManipulator();
        mGraphElem_.erase(selectedName());
        setSelectedName(-1);
        updateGL();
        handeled = true;
    }

    if(!handeled)
        QGLViewer::keyPressEvent(event);

}

void ModelViewer::drawGrid(float interval, int nbSubdivision)
{
    float size = interval * nbSubdivision; //in one quad
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_WIDTH);
    glBegin(GL_LINES);
        glColor3f(0,0,0);
        glLineWidth(2.0);
        glVertex3f(-size, 0, 0);
        glVertex3f( size, 0, 0);
        glVertex3f(0, -size, 0);
        glVertex3f(0,  size, 0);
        glColor3f(0.8, 0.8, 0.8);
        glLineWidth(1.0);
        for (int i = -nbSubdivision; i <= nbSubdivision; ++i) {
            glVertex3f(-size, i * interval, 0);
            glVertex3f( size, i * interval, 0);
            glVertex3f(i * interval, -size, 0);
            glVertex3f(i * interval,  size, 0);
        }
    glEnd();
    glDisable(GL_LINE_WIDTH);
    glEnable(GL_LIGHTING);
}

void ModelViewer::enableManipulator()
{
    setMouseTracking(true);
    float color[4];
    Frame& fr = mGraphElem_[selectedName()].getFrame();
    //X
    color[0] = 1.0; color[1] = 0.0; color[2] = 0.0; color[3] = 1.0;
    mpx_.setManipulatedFrame(fr);
    mpx_.setManipulateDirection(Vec(1,0,0), isLocalManipulate_);
    mpx_.setManipulateLength((camera()->position()-fr.position()).norm()/6.0);
    mpx_.draw(color);
    //Y
    color[0] = 0.0; color[1] = 1.0; color[2] = 0.0; color[3] = 1.0;
    mpy_.setManipulatedFrame(fr);
    mpy_.setManipulateDirection(Vec(0,1,0), isLocalManipulate_);
    mpy_.setManipulateLength((camera()->position()-fr.position()).norm()/6.0);
    mpy_.draw(color);
    //Z
    color[0] = 0.0; color[1] = 0.0; color[2] = 1.0; color[3] = 1.0;
    mpz_.setManipulatedFrame(fr);
    mpz_.setManipulateDirection(Vec(0,0,1), isLocalManipulate_);
    mpz_.setManipulateLength((camera()->position()-fr.position()).norm()/6.0);
    mpz_.draw(color);
}

void ModelViewer::disableManipulator()
{
    mpx_.setManipulatedFrame(worldFrame_);
    mpy_.setManipulatedFrame(worldFrame_);
    mpz_.setManipulatedFrame(worldFrame_);
    setMouseTracking(false);
}
