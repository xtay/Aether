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
