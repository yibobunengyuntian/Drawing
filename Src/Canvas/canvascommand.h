#ifndef CANVASCOMMAND_H
#define CANVASCOMMAND_H

#include <QUndoCommand>
#include <QPixmap>

class Canvas;

class CanvasCommand : public QUndoCommand
{
public:
    explicit CanvasCommand(Canvas* canvas, const QPixmap& oldPixmap, const QPixmap& newPixmap);
    ~CanvasCommand() override;

    void undo() override;
    void redo() override;

private:
    Canvas* m_canvas;
    QPixmap m_oldPixmap;
    QPixmap m_newPixmap;
};

class DrawingCommand : public QUndoCommand
{
public:
    DrawingCommand(Canvas* canvas, const QPixmap& before, const QPixmap& after);
    void undo() override;
    void redo() override;

private:
    Canvas* m_canvas;
    QPixmap m_before;
    QPixmap m_after;
};

#endif // CANVASCOMMAND_H
