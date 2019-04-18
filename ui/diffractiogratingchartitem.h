#ifndef DIFFRACTIOGRATINGCHARTITEM_H
#define DIFFRACTIOGRATINGCHARTITEM_H

#include "ui/basechartitem.h"
#include "deviceconfigs/diffractiongrating.h"

class DiffractioGratingChartItem : public BaseChartItem
{
public:
    DiffractioGratingChartItem(int id, DiffractionGrating * diffractionGrating);

private:
    virtual QSurfaceDataArray* fill3DSeries() override;
    virtual QLineSeries* fill2DSeries( ) override;

    DiffractionGrating *_source;

    double _gradeStepY = _stepY;
    double _gradeStepX = _stepX;
};

#endif // DIFFRACTIOGRATINGCHARTITEM_H
