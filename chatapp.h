#ifndef CHATAPP_H
#define CHATAPP_H

#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>
#include <QTcpServer>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatApp; }
QT_END_NAMESPACE

class ChatApp : public QWidget
{
    Q_OBJECT

public:
    ChatApp(QWidget *parent = nullptr);
    ~ChatApp();

private:
    Ui::ChatApp *ui;

    QTcpSocket  _socket;
    QDataStream _stream;
    QTcpServer server;

    bool        _connected=false;
    void        switchState();
    std::vector<QTcpSocket*> _vec_sock;
    //void writeMess_onDisplay(QString &_leIn);
public slots:
    void printMess_onDisplay();
    void connectDisconnect();
    void isConnected();
    void isDisconnected();
    void socketError();
    void receive();
    void send();
    void ReceiveData();




/*public slots:
void send();*/
};
#endif // CHATAPP_H
