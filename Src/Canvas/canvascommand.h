#ifndef CANVASCOMMAND_H
#define CANVASCOMMAND_H

#include <QUndoCommand>
#include <QPixmap>

class Canvas;

class DrawingCommand : public QUndoCommand
{
public:
    DrawingCommand(Canvas* canvas, const QPixmap& before, const QPixmap& after, const QPoint &pos = QPoint(0, 0));
    void undo() override;
    void redo() override;

private:
    Canvas* m_canvas;
    QPixmap m_before;
    QPixmap m_after;
    QPoint m_pos;
};

#endif // CANVASCOMMAND_H
