#ifndef SIGNAL_DISABLER_H
#define SIGNAL_DISABLER_H
#include <QWidget>

namespace Per {

class SignalBlocker {

public:
    explicit SignalBlocker(QWidget &widget);
  ~SignalBlocker();

private:
    QWidget &m_widget; // should maybe be unique pointer

};

} // Per

#endif //SIGNAL_DISABLER_H
