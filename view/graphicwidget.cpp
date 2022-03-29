#include "graphicwidget.h"
#include <QGraphicsView>

GraphicWidget::GraphicWidget() {

}

void GraphicWidget::mousePressEvent(QMouseEvent *event) {
    emit mouseClicked(event->x(), event->y());
}

GraphicScene::GraphicScene() {

}
void GraphicScene::mousePressEvent(QMouseEvent *event) {
    emit mouseClicked(event->x(), event->y());
}

