#include "debuginfo.h"

DebugInfo::DebugInfo()
{
}

void DebugInfo::addLine(const char * fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    curStr_.vsprintf(fmt, vargs);
    vInfo_.push_back(curStr_);

    va_end(vargs);
    return;
}

void DebugInfo::displayInfo(QGLWidget* qw)
{
    int v(offsetVertical);

    if (vInfo_.empty()) return;

    vector<QString>::const_iterator iter = vInfo_.begin();

    for(iter = vInfo_.begin(); iter != vInfo_.end(); ++iter)
    {
        qw->renderText(offsetHorizonal, v, *iter);
        v += lineInterval;
    }

    vInfo_.clear();

    return;
}
