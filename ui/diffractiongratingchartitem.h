#pragma once

#include "deviceconfigs/diffractiongrating.h"
#include "ui/basechartitem.h"

class DiffractionGratingChartItem : public BaseChartItem {
public:
    DiffractionGratingChartItem(int id, DiffractionGrating* diffractionGrating);

private:
    virtual QSurfaceDataArray* fill3DSeries() override;
    virtual QLineSeries* fill2DSeries() override;
    virtual bool isSourceChanged() override;

    DiffractionGrating* _source;

    double _gradeStepY = _stepY;
    double _gradeStepX = _stepX;
};
