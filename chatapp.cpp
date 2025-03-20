#include "chatapp.h"
#include "ui_chatapp.h"

//#define LOCALHOST   2130706433

ChatApp::ChatApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatApp)
{
    ui->setupUi(this);
    if(!ui->_leIn->isEnabled())
    {ui->_leIn->setEnabled(true);}
    switchState();
    if(!server.listen(QHostAddress("129.187.229.21"), 9000)) {
           return;
        }
    connect(ui->_pbCon,
          SIGNAL(clicked()),
          this,
          SLOT(connectDisconnect()));

    connect(&server, &QTcpServer::newConnection, this, &ChatApp::ReceiveData);

    connect(&_socket,
          SIGNAL(error(QAbstractSocket::SocketError)),
          this,
          SLOT(socketError()));
    //connect(socket, SIGNAL(readyRead()), this, SLOT(printMess_onDisplay()));
    connect(ui->_leIn,
          SIGNAL(returnPressed()),
          this,
          SLOT(send()));

    connect(&_socket,
          SIGNAL(readyRead()),
          this,
          SLOT(receive()));

    connect(&_socket,
          SIGNAL(connected()),
          this,
          SLOT(isConnected()));

    connect(&_socket,
          SIGNAL(disconnected()),
          this,
          SLOT(isDisconnected()));

    connect(ui->_send, &QPushButton::pressed, this, &ChatApp::send);
}

void ChatApp::switchState() {
    //ui->_leIn->setEnabled(_connected);
    ui->_lePort->setDisabled(_connected);
    ui->_leAdr->setDisabled(_connected);
}


void ChatApp::connectDisconnect(){
    ui->_pbCon->setDisabled(true);
    if (_connected){
        _socket.disconnectFromHost();
    } else {
        ui->_tbOut->append("Verbindungsversuch");
        bool okPort=false;
        quint16 port=ui->_lePort->text().toUShort(&okPort);
        if (okPort && port>0){
            _socket.connectToHost(ui->_leAdr->text(), port);
        } else {
            ui->_tbOut->append("Fehler: Ungültige Portnummer.");
            ui->_pbCon->setEnabled(true);
        }
    }
}

void ChatApp::ReceiveData(){
    QTcpSocket *socket = server.nextPendingConnection();

    if (!socket)
        return;

    qDebug("Client connected");

    //socket->waitForReadyRead(5000);
    QByteArray data = socket->readAll();

    qDebug(data.constData());
    _vec_sock.push_back(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(printMess_onDisplay()));
}

void ChatApp::printMess_onDisplay(){
    QTcpSocket *socket = static_cast<QTcpSocket *>  (sender());
    auto bytes_array = socket->readAll();

    ui->_tbOut->append("Empfangen: ");
    ui->_tbOut->append(QString(bytes_array));
}

/*void ChatApp::writeMess_onDisplay(QString &_leIn){
           QByteArray data = _leIn.toUtf8();
           socket->write(data);
           socket->flush();
           return;
}*/

void ChatApp::isConnected(){
    ui->_tbOut->append("Verbunden mit: "+
                       ui->_leAdr->text()+
                       ", Port: "+
                       ui->_lePort->text());
    _connected=true;
    switchState();
    ui->_pbCon->setEnabled(true);
}

void ChatApp::isDisconnected(){
    ui->_tbOut->append("Verbindung getrennt.");
    _connected=false;
    switchState();
    ui->_pbCon->setEnabled(true);
}

void ChatApp::socketError(){
    ui->_tbOut->append("Verbindungsfehler oder Abbruch.");
    ui->_pbCon->setEnabled(true);
    _connected=false;
    switchState();
}

void ChatApp::receive(){
    ui->_tbOut->append("Erhalten: ");
    QByteArray received=_socket.readAll();
    ui->_tbOut->append(QString::fromUtf8(
                        received.constData(),
                        received.length()));
}


/*void ChatApp::send(){
        QString to_copy = ui->_message->text();
        ui->_chat->append(to_copy);
        ui->_message->clear();
    }*/

void ChatApp::send(){
    ui->_tbOut->append("Schreibe: "+ui->_leIn->text());
    //_stream<<ui->_leIn->text();

    QByteArray sendData=ui->_leIn->text().toUtf8();
    //_stream.writeRawData(sendData,sendData.size());
    //_stream<<static_cast<quint8>('\n');
    ui->_leIn->clear();
    for(auto socket_invector : _vec_sock){
        if(socket_invector->isValid()){
            socket_invector->write(sendData.constData());
        }
    }
}



ChatApp::~ChatApp()
{
    delete ui;
}

