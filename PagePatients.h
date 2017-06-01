#ifndef PAGEPATIENTS_H
#define PAGEPATIENTS_H

#include "PageDefault.h"

class PagePatients : public PageDefault
{
public:
    explicit PagePatients(QWidget* parent = 0);

private:
    enum {COL_ID, COL_NAME};
};

#endif // PAGEPATIENTS_H
