#include <exception>

#include "propertyobserver.h"

PropertyObserver::PropertyObserver(QTableWidget *propertyWidget,
                                   QObject *parent)
    :QObject(parent), _propertyWidget(propertyWidget) {
    if (!propertyWidget)
        throw std::invalid_argument("nullptr QTableWidget");

    connect(_propertyWidget, SIGNAL(cellChanged(int,int)),
            this, SLOT(writeProperties()));
}

void PropertyObserver::loadProperties(Properties *item) {
    _propertyItem = item;

    if (item) {
        fillList();
    } else {
        _propertyWidget->clearContents();
    }
}

void PropertyObserver::writeProperties() {
    QMap<QString, double> lst;
    for (int i = 0; i < _propertyWidget->rowCount(); ++i) {
        lst.insert(_propertyWidget->item(i, 0)->text(),
                   _propertyWidget->item(i, 1)->text().toDouble());
    }

    if (_propertyItem) {
        _propertyItem->setProperties(lst);
    } else {
        while (_propertyWidget->rowCount()) _propertyWidget->removeRow(0);
    }
}

void PropertyObserver::fillList() {
    // чтобы не пытаться записать в пропертис только что выгруженные данные
    _propertyWidget->blockSignals(true);
    while (_propertyWidget->rowCount())
        _propertyWidget->removeRow(0);

    int counter = 0;
    auto lst = _propertyItem->getProperties();
    for (auto iter = lst.begin(); iter != lst.end(); ++iter) {
        _propertyWidget->insertRow(counter);
        _propertyWidget->setItem(counter, 0,
                                 new QTableWidgetItem(iter.key()));
        _propertyWidget->setItem(counter, 1,
                        new QTableWidgetItem(QString::number(iter.value())));

        ++counter;
    }
    _propertyWidget->blockSignals(false);
}
