#ifndef LIBRARYCONTROLLER_H
#define LIBRARYCONTROLLER_H

#include "basecontroller.h"
#include "models.h"

#include <QUuid>

namespace mopidy {

class LibraryController : public BaseController
{
    Q_OBJECT

public:
    explicit LibraryController();
    ~LibraryController();

    void browse(const QString &uri = "");
    void search(const QHash<QString, QStringList> &query, const QStringList &uris, bool exact, const QUuid &identifier);
    void lookup(const QStringList &uris = QStringList());
    void refresh(const QString &uri = "");
    void getImages(const QStringList &uris);

signals:
    void browsed(const mopidy::Refs &refs);
    void lookedup(const mopidy::Tracks &tracks);
    void searchResult(const QUuid &identifier, const mopidy::SearchResult &sr);
    void imagesReceived(const QMap<QString, mopidy::Images> &uri_images);
};

} // mopidy

#endif //LIBRARYCONTROLLER_H
