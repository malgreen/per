#ifndef HTTP_TAB_H
#define HTTP_TAB_H

#include "../../core/http.h"

#include <QtWidgets/QTableWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWidgets/QGraphicsEffect>


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
    explicit HttpTab(QWidget *parent, HttpRequestModel_t &httpRequestModel);
    ~HttpTab() override;

  private:
    Ui::HttpTab *ui;
    QNetworkAccessManager &m_networkManager;
    HttpRequestModel_t &m_httpRequestModel;
    HttpResponseModel_t *m_httpResponseModel; // can be null

    void SetupUrlGroup();
    void SetupRequestGroup();
    void SetupResponseGroup();

    void SetupEnabledKeyValueTable(QTableWidget &table, QList<EnabledKeyValuePair_t> &tableData) const;

    void OnNetworkManagerFinished(QNetworkReply *reply);
    void OnSendButtonClicked();

    void GetParametersFromUrl(const QUrl &url) const;
    void GetUrlFromParameters() const;

    void AddTableRow(QTableWidget &table, QList<EnabledKeyValuePair_t> &tableData, EnabledKeyValuePair_t &rowData) const;
    void AddTableRowIfLastRowNotEmpty(QTableWidget &table, QList<EnabledKeyValuePair_t> &tableData) const;
    static bool IsTableRowEmpty(const QTableWidget &table, int row);
};
} // namespace Per

#endif // HTTP_TAB_H
