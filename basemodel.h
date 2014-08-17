#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QString>
#include "debuginfo.h"

class BaseModel
{
public:
    BaseModel();
    virtual ~BaseModel(){}

    virtual void drawModel();
};

#endif // BASEMODEL_H
