#ifndef PLANEMODEL_H
#define PLANEMODEL_H

#include "basemodel.h"

class PlaneModel : public BaseModel
{
public:
    PlaneModel();
    virtual ~PlaneModel();

    virtual void drawModel();
};

#endif // PLANEMODEL_H
