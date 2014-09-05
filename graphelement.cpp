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
#include "graphelement.h"
#include "planemodel.h"

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
    glMatrixMode(GL_TEXTURE);
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
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0.5,0.5,0.5);
        pbm_->drawModel();
    }
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
