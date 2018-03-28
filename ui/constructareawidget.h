#pragma once

#include <QGraphicsView>

class Properties;

class ConstructAreaWidget : public QGraphicsView {
    Q_OBJECT
public:
    explicit ConstructAreaWidget(QWidget *parent = 0);

signals:
    void propertiesItemClicked(Properties *);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void createConnectionLine(QPointF pos);
    void adjustConnectionLine(QPointF pos);
    void dropConnectionLine();

    QGraphicsLineItem *_connectionLine;
};
