#ifndef HTTP_TAB_H
#define HTTP_TAB_H

#include "../../core/http.h"

#include <QtWidgets/QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>


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
    QNetworkAccessManager &m_networkManager;
    HttpRequestModel &m_httpRequestModel;
    HttpResponseModel *m_httpResponseModel; // can be null

    void SetupEnabledKeyValueTable(QTableWidget &table, QList<EnabledKeyValuePair> &tableData) const;
    void AddTableRow(QTableWidget &table, QList<EnabledKeyValuePair> &tableData, EnabledKeyValuePair &rowData) const;
    void AddTableRowIfLastRowNotEmpty(QTableWidget &table, QList<EnabledKeyValuePair> &tableData) const;
    static bool IsTableRowEmpty(const QTableWidget &table, int row);
};
} // namespace Per

#endif // HTTP_TAB_H
