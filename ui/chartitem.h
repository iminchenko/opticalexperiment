#ifndef CHARTITEM_H
#define CHARTITEM_H

#include "ui/basechartitem.h"
#include "deviceconfigs/wave.h"
#include "deviceconfigs/display.h"

using namespace QtDataVisualization;



class ChartItem : public BaseChartItem
{
public:
    ChartItem(int id, Display* source);

private:
    virtual QSurfaceDataArray* fill3DSeries() override;
    virtual QLineSeries* fill2DSeries( ) override;

    double randomDouble(double max) const;


    //source of data for generations
    Display *_source;
};;

#endif // CHARTITEM_H
