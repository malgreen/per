#include "signal_blocker.h"

namespace Per {

SignalBlocker::SignalBlocker(QWidget &widget) : m_widget(widget)
{
    m_widget.blockSignals(true);
}
SignalBlocker::~SignalBlocker()
{
    m_widget.blockSignals(false);
}

} // Per