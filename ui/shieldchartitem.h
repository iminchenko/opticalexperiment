#pragma once

#include "deviceconfigs/display.h"
#include "deviceconfigs/wave.h"
#include "ui/basechartitem.h"

using namespace QtDataVisualization;

class ShieldChartItem : public BaseChartItem {
public:
    ShieldChartItem(int id, Display* source);

private:
    virtual QSurfaceDataArray* fill3DSeries() override;
    virtual QLineSeries* fill2DSeries() override;
    virtual bool isSourceChanged() override;

    double randomDouble(double max) const;

    // source of data for generations
    Display* _source;
};
