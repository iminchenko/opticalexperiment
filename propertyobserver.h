#pragma once

#include <QObject>
#include <QTableWidget>
#include <memory>

#include "properties.h"

class Command;

class PropertyObserver: public QObject {
    Q_OBJECT
public:
    PropertyObserver(QTableWidget *propertyWidget, QObject *parent = nullptr);

signals:
    void invoke(std::shared_ptr<Command>);
    void changed();

public slots:
    void loadProperties(Properties *item);
    void writeProperties();

private:
    void fillList();

    QTableWidget *_propertyWidget;
    Properties *_propertyItem;
};
