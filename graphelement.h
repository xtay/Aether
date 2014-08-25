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
#ifndef GRAPHELEMENT_H
#define GRAPHELEMENT_H

#include "basemodel.h"
#include <QGLViewer/frame.h>
#include <auto_ptr.h>
#include <tr1/memory>

using qglviewer::Frame;
using qglviewer::Vec;
using qglviewer::Quaternion;

class GraphElement
{
public:
    GraphElement();
    GraphElement(BaseModel* pbm);
    GraphElement(BaseModel* pbm, const qglviewer::Frame& f);
    GraphElement(BaseModel* pbm, const qglviewer::Vec &v, const qglviewer::Quaternion &q);
    ~GraphElement();

    void setFrame(const qglviewer::Frame& f);
    void setFrame(const qglviewer::Vec &v, const qglviewer::Quaternion &q);

    //this Frame is going to be modified by the manipulator
    Frame& getFrame() {return fr_;}
    const Frame* getFramePtr() const {return &fr_;}

    void drawElement(bool isSelected = false);

private:
    std::tr1::shared_ptr<BaseModel> pbm_;
    Frame fr_;
};

#endif // GRAPHELEMENT_H
