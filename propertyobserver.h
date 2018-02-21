#pragma once

#include <QObject>
#include <QTableWidget>

#include "properties.h"

class PropertyObserver: public QObject {
    Q_OBJECT
public:
    PropertyObserver(QTableWidget *propertyWidget,
                     QObject *parent = nullptr);

public slots:
    void loadProperties(Properties *item);
    void writeProperties();

private:
    void fillList();

    QTableWidget *_propertyWidget;
    Properties *_propertyItem;
};
