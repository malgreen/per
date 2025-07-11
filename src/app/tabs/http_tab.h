#ifndef HTTP_TAB_H
#define HTTP_TAB_H

#include "../../core/http.h"

#include <QCheckBox>
#include <QWidget>
#include <QtWidgets/qtablewidget.h>

namespace Per
{
QT_BEGIN_NAMESPACE
namespace Ui
{
class HttpTab;
}
QT_END_NAMESPACE

class HttpTab : public QWidget
{
    Q_OBJECT

  public:
    explicit HttpTab(QWidget *parent, HttpRequestModel &httpRequestModel);
    ~HttpTab() override;

  private:
    Ui::HttpTab *ui;
    HttpRequestModel &m_httpRequestModel;
    void SetupEnabledKeyValueTable(QTableWidget &table, QList<EnabledKeyValuePair> &tableData) const;
    void OnTableCellChanged(int row, int column) const;
    void AddTableRow(QTableWidget &table, QList<EnabledKeyValuePair> &tableData, EnabledKeyValuePair &rowData) const;
    void AddTableRowIfLastRowNotEmpty(QTableWidget &table, QList<EnabledKeyValuePair> &tableData) const;
    static bool IsTableRowEmpty(const QTableWidget &table, int row);
};
} // namespace Per

#endif // HTTP_TAB_H
