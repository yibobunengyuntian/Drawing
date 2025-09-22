#include "canvascommand.h"
#include "canvas.h"

DrawingCommand::DrawingCommand(Canvas* canvas, const QPixmap& before, const QPixmap& after, const QPoint &pos)
    : m_canvas(canvas), m_before(before), m_after(after), m_pos(pos)
{
    setText("Drawing");
}

void DrawingCommand::undo()
{
    if (m_canvas) {
        m_canvas->drawingPixmap(m_before, m_pos);
    }
}

void DrawingCommand::redo()
{
    if (m_canvas) {
        m_canvas->drawingPixmap(m_after, m_pos);
    }
}
