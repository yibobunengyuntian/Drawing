#include "canvascommand.h"
#include "canvas.h"

CanvasCommand::CanvasCommand(Canvas* canvas, const QPixmap& oldPixmap, const QPixmap& newPixmap)
    : m_canvas(canvas), m_oldPixmap(oldPixmap), m_newPixmap(newPixmap)
{
}

CanvasCommand::~CanvasCommand()
{
}

void CanvasCommand::undo()
{
    if (m_canvas) {
        m_canvas->restorePixmap(m_oldPixmap);
    }
}

void CanvasCommand::redo()
{
    if (m_canvas) {
        m_canvas->restorePixmap(m_newPixmap);
    }
}

DrawingCommand::DrawingCommand(Canvas* canvas, const QPixmap& before, const QPixmap& after)
    : m_canvas(canvas), m_before(before), m_after(after)
{
    setText("Drawing");
}

void DrawingCommand::undo()
{
    if (m_canvas) {
        m_canvas->restorePixmap(m_before);
    }
}

void DrawingCommand::redo()
{
    if (m_canvas) {
        m_canvas->restorePixmap(m_after);
    }
}
