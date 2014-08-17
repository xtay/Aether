#include "manipulator.h"
#include <QGLViewer/camera.h>
#include <GL/gl.h>
#include <cmath>
#include <QMouseEvent>

Manipulator::Manipulator(Frame &fr):
    pfr_(&fr), direction_(Vec(0,0,1)), action_(MOVE), length_(1)
{
}

Manipulator::Manipulator(Frame &fr, const Vec& dir, ManipulateMode mode):
    pfr_(&fr), direction_(dir.unit()), action_(mode)
{
}

void Manipulator::setManipulateDirection(const Vec &dir, bool isLocal)
{
    //note that pfr_ must not be NULL...
    Vec worldDirection(dir);
    isLocalDirection_ = isLocal;
    if (isLocal) {
        worldDirection =  pfr_->inverseTransformOf(dir);
    }
    direction_ = worldDirection.unit();
}

void Manipulator::setManipulatedFrame(Frame &fr)
{
    pfr_ = &fr;
}

void Manipulator::setManipulateMode(ManipulateMode mode)
{
    action_ = mode;
}

void Manipulator::setManipulateLength(float length)
{
    length_ = length;
}

//figure out if (x,y) is close enough to line segment (x1,y1)---(x2,y2) bool check1 = false;
static bool closeToSegment(int x, int y, int x1, int y1, int x2, int y2, int offset)
{
    bool check1 = false;
    bool check2 = false;
    //line ax + by + c = 0
    int a = y2 - y1;
    int b = x1 - x2;
    int c = - b * y1 - a * x1;
    //bounding box check
    int maxX = (x1 > x2 ? x1 : x2) + offset;
    int minX = (x1 < x2 ? x1 : x2) - offset;
    int maxY = (y1 > y2 ? y1 : y2) + offset;
    int minY = (y1 < y2 ? y1 : y2) - offset;

    check1 = (minX <= x)&&(x <= maxX)&&(minY <= y)&&(y <= maxY);

    //distance check
    if(check1)
    {
        /*
        check2 = (a*(x + offset)+b*y + c)*(a*(x-offset) + b*y + c)<0 ||
                 (a*x + b*(y+offset) + c)*(a*x + b*(y-offset) + c)<0 ;
                 //(a==0 && b==0);
        */
        check2 = ((a*x + b*y + c) * (a*x + b*y +c) / (a*a + b*b)) < offset*offset;
    }
    return check2;
}

void Manipulator::checkIfGrabsMouse(int x, int y, const Camera * const camera)
{
    const static int grabOffset = 10;
    Vec direction = direction_;

    switch (action_) {
    case MOVE: {
        const Vec point1 = camera->projectedCoordinatesOf(pfr_->position());
        const Vec point2 = camera->projectedCoordinatesOf(pfr_->position()+direction*length_);
        bool close = closeToSegment(x, y, point1.x, point1.y, point2.x, point2.y, grabOffset);
        setGrabsMouse(keepsGrabbingMouse_ || close);
        break;
    }//case MOVE
    case ROTATE: {
        Vec objPos = pfr_->position();
        Vec camPos = camera->position();
        Vec sight = objPos - camPos;
        Vec curPoint(static_cast<float>(x),static_cast<float>(y),0.0);
        Vec curPos = camera->unprojectedCoordinatesOf(curPoint);
        Vec curDir = curPos - camPos;
        Vec curPlane = cross(sight, curDir);
        Vec line = cross(direction, curPlane);
        Vec nearPos = objPos + length_ * line.unit();
        double ratio = sight.unit() * direction;
        if (ratio > 0)
            nearPos = objPos - length_ * line.unit();

        //modification
        Vec toNearPos = nearPos - objPos;
        Vec orth = cross(toNearPos, direction);//orth has a length of length_(a variable in class)
        Vec toNearPos1  = (toNearPos + orth * 0.57).unit() * 0.87 *length_;//magic, 0.57 = arctan(30) to make a 30 degree rotate
        Vec toNearPos2  = (toNearPos - orth * 0.57).unit() * 0.87 *length_;//magic, 0.87 close to cos(30)

        Vec p0 = camera->projectedCoordinatesOf(nearPos);
        Vec p1 = camera->projectedCoordinatesOf(objPos + toNearPos1);
        Vec p2 = camera->projectedCoordinatesOf(objPos + toNearPos2);

        bool close = closeToSegment(x, y, p0.x, p0.y, p1.x, p1.y, grabOffset) ||
                     closeToSegment(x, y, p0.x, p0.y, p2.x, p2.y, grabOffset);

        //if the ellipse is too mush narrow, consider it as a line segement...
        if (fabs(ratio) < 0.1) {
            //directin of the majorAxis of the ellipse
            Vec majorAxis = cross(direction, sight.unit());
            //two endpoint the majorAxis
            Vec far0 = camera->projectedCoordinatesOf(objPos + majorAxis.unit()*length_);
            Vec far1 = camera->projectedCoordinatesOf(objPos - majorAxis.unit()*length_);
            close = closeToSegment(x, y, far0.x, far0.y, far1.x, far1.y, grabOffset);
        }
        setGrabsMouse(keepsGrabbingMouse_ || close);

        break;
    }//case ROTATE
    }//switch (action_)
}

void Manipulator::mousePressEvent(QMouseEvent * const event, Camera * const camera)
{
    if (grabsMouse())
        keepsGrabbingMouse_ = true;
    moved_ = true;

    prevCurPoint_ = event->pos();
    //the position cursor is at, for MOVE
    prevObjPos_ = pfr_->position();

    /****************all the below are for ROTATE*******************/
    Vec camPos = camera->position();
    Vec curPos = camera->unprojectedCoordinatesOf(Vec(static_cast<float>(prevCurPoint_.x()), static_cast<float>(prevCurPoint_.y()), 0.0));
    Vec curDir = curPos - camPos;
    Vec objPos = pfr_->position();//prevObjPos_ will remain unchanged when rotating
    Vec sight = objPos - camPos;
    float offset = length_*length_/sight.norm();
    Vec pinPos = objPos - sight.unit()*offset;
    //line along <curDir> through <camPos>, plane orth to <sight> through <pinPos>
    float coefT = (sight*(pinPos - camPos)) / (sight*curDir);
    Vec curProj = Vec(camPos.x + coefT*curDir.x,
                      camPos.y + coefT*curDir.y,
                      camPos.z + coefT*curDir.z);
    Vec toCurProj = curProj - objPos;

    //if the curProj is ocassionally outside the manipulat sphere(due to the grabOffset)
    if (toCurProj.norm() > length_) {
        prevObjLookAt_ = toCurProj.unit() * length_;
    }
    else {
        Vec temp1 = toCurProj * sight / sight.norm() * sight.unit();
        Vec temp2 = toCurProj - temp1;
        float la = temp2.norm();
        float lb = sqrt(length_*length_ - la*la);
        Vec temp3 = -lb * sight.unit();
        prevObjLookAt_ = temp2 + temp3;
    }
    prevObjOrien_ = pfr_->orientation();
}

inline static float det(float a0, float b0, float c0,
                        float a1, float b1, float c1,
                        float a2, float b2, float c2)
{
    return a0 * (b1*c2 - c1*b2) +
           b0 * (c1*a2 - a1*c2) +
           c0 * (a1*b2 - b1*a2);
}

void Manipulator::mouseMoveEvent(QMouseEvent * const event, Camera * const camera)
{
    if (moved_) {
        Vec direction = direction_;
        Vec prevCurPos = camera->unprojectedCoordinatesOf(Vec(static_cast<float>(prevCurPoint_.x()), static_cast<float>(prevCurPoint_.y()), 0.0));
        QPoint curPoint = event->pos();
        Vec curPos = camera->unprojectedCoordinatesOf(Vec(static_cast<float>(curPoint.x()), static_cast<float>(curPoint.y()), 0.0));
        Vec camPos = camera->position();
        //start and current direction which mouse pointed to
        Vec prevCurDir = prevCurPos - camPos;
        Vec curDir = curPos - camPos;

        switch (action_) {
        case MOVE: {
            //plane which the MOVE is going to happen
            Vec plane0 = cross(prevCurDir, direction);
            //get two planes orthnxxx to plane0;
            Vec plane1 = cross(curDir, plane0);
            Vec plane2 = cross(direction, plane0);
            //coefficients, ready to solve the linear equation!
            float A0 = plane0.x, B0 = plane0.y, C0 = plane0.z;
            float A1 = plane1.x, B1 = plane1.y, C1 = plane1.z;
            float A2 = plane2.x, B2 = plane2.y, C2 = plane2.z;
            float D0 = (A0*camPos.x + B0*camPos.y + C0*camPos.z);
            float D1 = (A1*camPos.x + B1*camPos.y + C1*camPos.z);
            float D2 = (A2*prevCurPos.x + B2*prevCurPos.y + C2*prevCurPos.z);
            //solve the equation Ax+By+Cz=D, to get the targeted point from curPos in direction;
            float detD = det(A0, B0, C0,
                             A1, B1, C1,
                             A2, B2, C2);
            float detX = det(D0, B0, C0,
                             D1, B1, C1,
                             D2, B2, C2);
            float detY = det(A0, D0, C0,
                             A1, D1, C1,
                             A2, D2, C2);
            float detZ = det(A0, B0, D0,
                             A1, B1, D1,
                             A2, B2, D2);
            Vec great(detX/detD, detY/detD, detZ/detD);
            //figure out how much to translate and which direction to translate...
            Vec trans(great - prevCurPos);
            float distance = trans.norm()*(prevObjPos_ - camPos).norm()/prevCurDir.norm();
            distance = (trans*direction)>0 ? distance : -distance;
            pfr_->setPosition(prevObjPos_ + direction*distance);
            break;
        } //case MOVE
        case ROTATE: {
            Vec objPos = prevObjPos_;//prevObjPos_ will remain unchanged when rotating
            Vec sight = objPos - camPos;
            float offset = length_*length_/sight.norm();
            Vec pinPos = objPos - sight.unit()*offset;

            //line along <curDir> through <camPos>, plane orth to <sight> through <pinPos>
            float coefT = (sight*(pinPos - camPos)) / (sight*curDir);
            Vec curProj = Vec(camPos.x + coefT*curDir.x,
                              camPos.y + coefT*curDir.y,
                              camPos.z + coefT*curDir.z);
            Vec toCurProj = curProj - objPos;

            //if the curProj is ocassionally outside the manipulat sphere(due to the grabOffset)
            Vec curObjLookAt;
            if (toCurProj.norm() > length_) {
                curObjLookAt = toCurProj.unit() * (length_);
            }
            else {
                Vec temp1 = toCurProj * sight / sight.norm() * sight.unit();
                Vec temp2 = toCurProj - temp1;
                float la = temp2.norm();
                float lb = sqrt(length_*length_ - la*la);
                Vec temp3 = -lb * sight.unit();
                curObjLookAt = temp2 + temp3;
            }

            //project both lookAt vec on specific plane defined by direction_
            Vec tempPrev = prevObjLookAt_*direction/direction.norm() * direction;
            Vec tempCur  = curObjLookAt*direction/direction.norm() * direction;
            Vec tempFrom = prevObjLookAt_ - tempPrev;
            Vec tempTo   = curObjLookAt - tempCur;
            if (tempTo.norm() < 0.0001) tempTo = tempFrom;

            Vec projFrom, projTo;
            projFrom = pfr_->transformOf(tempFrom);
            projTo   = pfr_->transformOf(tempTo);
            Quaternion distance(projFrom.unit(), projTo.unit());
            pfr_->setOrientation((prevObjOrien_*distance).normalized());

            /*
            dbi.addLine("prevObjLookAt_: %f, %f, %f", prevObjLookAt_.x, prevObjLookAt_.y, prevObjLookAt_.z);
            dbi.addLine("norm: %f", prevObjLookAt_.norm());
            dbi.addLine("curObjLookAt_: %f, %f, %f", curObjLookAt.x, curObjLookAt.y, curObjLookAt.z);
            dbi.addLine("norm: %f", curObjLookAt.norm());

            dbi.addVertex3fv(objPos + prevObjLookAt_);
            dbi.addVertex3fv(objPos + curObjLookAt);

            dbi.addVertex3fv(objPos);
            dbi.addVertex3fv(objPos + tempFrom);
            dbi.addVertex3fv(objPos + tempTo);
            */

            break;
        } //case ROTATE

        } //switch (action_)
    } //if (moved_)
}

void Manipulator::mouseReleaseEvent(QMouseEvent * const event, Camera * const camera)
{
    keepsGrabbingMouse_ = false;
    moved_ = false;
}

void Manipulator::draw(float *color)
{
    Frame tmpFr(pfr_->position(),Quaternion(Vec(0,0,1), direction_));
    glPushMatrix();
    glMultMatrixd(tmpFr.worldMatrix());
    glDisable(GL_LIGHTING);
    switch(action_){
    case MOVE:
        drawArrow(color);
        break;
    case ROTATE:
        drawCircle(color);
        break;
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Manipulator::drawArrow(float* color)
{
    GLboolean lighting, colorMaterial;
    glGetBooleanv(GL_LIGHTING, &lighting);
    glGetBooleanv(GL_COLOR_MATERIAL, &colorMaterial);

    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    float radius = 0.01*length_;
    const static int nbSubdivisions = 16;
    static GLUquadric* quadric = gluNewQuadric();

    if (radius < 0.0)
        radius = 0.0625 * length_;

    const float head = 2.5*(radius / length_) + 0.25;
    const float coneRadiusCoef = 6.0 - 5.0 * head;

    float color_select_[4] = {1.0, 1.0, 0.0, 1.0};
    if (grabsMouse()) {
        glColor3fv(color_select_);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_select_);
    }
    else {
        glColor3fv(color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
    }

    gluCylinder(quadric, radius, radius, length_ * 0.9, nbSubdivisions, 1);
    glTranslatef(0.0, 0.0, length_ * (1.0 - head));
    gluCylinder(quadric, coneRadiusCoef * radius, 0.0, head * length_, nbSubdivisions, 1);
    glTranslatef(0.0, 0.0, -length_ * (1.0 - head));

    if (!lighting)
        glDisable(GL_LIGHTING);
    if (colorMaterial)
        glEnable(GL_COLOR_MATERIAL);
}

void Manipulator::drawCircle(float* color)
{
    const static int nbSubdivisions = 64;
    float s = 0, c = 0;
    float radian = 2*M_PI / nbSubdivisions;

    float color_select_[4] = {1.0, 1.0, 0.0, 1.0};
    if (grabsMouse())
        glColor3fv(color_select_);
    else
        glColor3fv(color);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < nbSubdivisions; ++i) {
        s = sin(i * radian);
        c = cos(i * radian);
        glVertex3f(c * length_, s * length_, 0.0);
    }
    glEnd();
}
