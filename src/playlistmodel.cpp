#include "playlistmodel.h"

PlaylistModel::PlaylistModel():QAbstractListModel()
{
m_manager = new QNetworkAccessManager();
m_handler = new Handler();
connect(m_handler,&Handler::itemFound,this,&PlaylistModel::addXmlItem);
connect(m_handler,&Handler::mEndDocument,[this] {
    QModelIndex m = QModelIndex();
    qDebug() << dataItems.count();
    insertRows(0,dataItems.count(),m);
});
connect(m_manager,&QNetworkAccessManager::finished,this,&PlaylistModel::requestReceived);
connect(m_manager,&QNetworkAccessManager::authenticationRequired,this,&PlaylistModel::authenticate);
connect(this,&PlaylistModel::searchChanged,[this]() {
m_manager->clearAccessCache();

    m_proxyModel->setFilterRegExp(QRegExp(search(), Qt::CaseInsensitive));

;

});
        m_proxyModel = new QSortFilterProxyModel(this);
        m_proxyModel->setFilterRole(NameRole);
        m_proxyModel->setSourceModel(this);
        connect(this,&PlaylistModel::remoteUrlChanged,this,&PlaylistModel::fetchFromUrl);


}
void PlaylistModel::update() {
    dataItems.clear();
    fetchFromUrl();
}
void PlaylistModel::fetchFromUrl()
{
    m_manager->get(QNetworkRequest(QUrl("http://"+m_remoteUrl+"/requests/playlist.xml").toString()));
}
bool PlaylistModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}


bool PlaylistModel::addXmlItem(DataItem *item)
{
dataItems.append(*item);
return true;
}
QHash<int,QByteArray> PlaylistModel::roleNames() const {
    QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[IdRole] = "id";
        roles[CurrentRole] = "current";
    return roles;
}
void PlaylistModel::authenticate(QNetworkReply* reply,QAuthenticator* authenticator) {
    authenticator->setUser(username());
    authenticator->setPassword(password());
    //Only try one time since settings does not change
    disconnect(m_manager,&QNetworkAccessManager::authenticationRequired,this,&PlaylistModel::authenticate);
}
void PlaylistModel::requestReceived(QNetworkReply*reply) {
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()==401) {
        DataItem *item = new DataItem;
        item->name="Authentication error, check settings";
        item->id="-1";
        dataItems.append(*item);
        insertRows(0,1,QModelIndex());
        return;
    }
    QXmlInputSource *xInSrc = new QXmlInputSource();
    xInSrc->setData(reply->readAll());
    QXmlSimpleReader *reader = new QXmlSimpleReader();

    reader->setContentHandler(m_handler);
    reader->setErrorHandler(m_handler);
    qDebug() << reader->parse(xInSrc);
}
QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    QString ret = "";
    switch(role) {
        case NameRole:{
            ret = dataItems.at(index.row()).name;
            break;
        }
    case IdRole:{
        ret = dataItems.at(index.row()).id;
        break;
    }
    case CurrentRole:{
        ret = dataItems.at(index.row()).current;
        break;
    }
    }
    return ret;
}
int PlaylistModel::rowCount(const QModelIndex &parent) const
{

    return dataItems.size();

}
