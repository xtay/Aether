#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QGLViewer/qglviewer.h>
#include "basemodel.h"
#include "boxmodel.h"

class ModelViewer : public QGLViewer
{
public:
    explicit ModelViewer(QWidget* parent);
    ~ModelViewer();

protected:
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;

private:
    BaseModel *bm1;
    BaseModel *bm2;
};

#endif // MODELVIEWER_H
