#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QGLViewer/qglviewer.h>

class ModelViewer : public QGLViewer
{
public:
    explicit ModelViewer(QWidget* parent);

protected:
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;
};

#endif // MODELVIEWER_H
