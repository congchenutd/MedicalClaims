#ifndef PAGEPROVIDERS_H
#define PAGEPROVIDERS_H

#include "PageDefault.h"

class PageProviders : public PageDefault
{
public:
    explicit PageProviders(QWidget* parent = 0);

    void onSwitch();

public:
    enum {COL_ID, COL_NAME};
};

#endif // PAGEPROVIDERS_H
