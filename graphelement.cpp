#include "graphelement.h"

using qglviewer::Frame;
using qglviewer::Vec;
using qglviewer::Quaternion;

GraphElement::GraphElement():
    pbm_(new BaseModel())
{
}

GraphElement::GraphElement(BaseModel *pbm):
    pbm_(pbm)
{
}

GraphElement::GraphElement(BaseModel *pbm, const qglviewer::Frame& f):
    pbm_(pbm), fr_(f)
{
}

GraphElement::GraphElement(BaseModel *pbm, const Vec& v, const Quaternion& p):
    pbm_(pbm), fr_(Frame(v,p))
{
}

GraphElement::~GraphElement()
{
}

void GraphElement::setFrame(const Frame& f)
{
    fr_ = f;
}

void GraphElement::setFrame(const Vec& v, const Quaternion& q)
{
    fr_ = Frame(v, q);
}

void GraphElement::drawElement(bool isSelected)
{
    glPushMatrix();
    glMultMatrixd(fr_.worldMatrix());
    if (isSelected) { //with wireframes
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        glColor3f(0.5,0.5,0.5);
        pbm_->drawModel();
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        glColor3f(0.5, 1.0, 0.5);
        pbm_->drawModel();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glColor3f(0.5,0.5,0.5);
        pbm_->drawModel();
    }
    glPopMatrix();
}
