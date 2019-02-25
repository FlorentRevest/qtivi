#ifndef MODELTRANSLATOR_H
#define MODELTRANSLATOR_H

#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>

#include "models.h"

namespace mopidy {

class ModelTranslator
{
public:
    /*
     * Encoders
     */
    static QJsonObject toJson(const Artist &artist);
    static QJsonObject toJson(const Album &album);
    static QJsonObject toJson(const Track &track);
    static QJsonObject toJson(const Playlist &playlist);
    static QJsonObject toJson(const TlTrack &tltrack);

    template <class T>
    static QJsonArray toJsonArray(const QList<T> &lt)
    {
        QJsonArray ja;
        for (T t : lt)
            ja.append(toJson(t));
        return ja;
    }

    /*
     * Decoders
     */
    static bool fromJson(const QJsonObject &jo, Artist &artist);
    static bool fromJson(const QJsonObject &jo, Album &album);
    static bool fromJson(const QJsonObject &jo, Track &track);
    static bool fromJson(const QJsonObject &jo, Playlist &playlist);
    static bool fromJson(const QJsonObject &jo, SearchResult &searchResult);
    static bool fromJson(const QJsonObject &jo, TlTrack &tlTrack);
    static bool fromJson(const QJsonObject &jo, Ref &ref);
    static bool fromJson(const QJsonObject &jo, PlaybackState &state);
    static bool fromJson(const QJsonObject &jo, Image &image);

    template <class T>
    static QList<T> fromJsonArray(const QJsonArray &ja)
    {
        QList<T> lst;
        for (QJsonValue jv : ja)
        {
            QJsonObject jso = jv.toObject();
            T t;
            if (fromJson(jso, t))
                lst << t;
        }
        return lst;
    }

    // date
    static QDate getMopidyDate(const QString &);
    static QString toMopidyDate(const QDate &date);

    // state
    static PlaybackState getState(const QString &);
    static QString getStateStr(const PlaybackState &state);

    // Ref type
    static RefType getRefType(const QString &);

    // dict
    static QJsonObject toJsonDict(const QHash<QString, QString> &);
};
}

#endif // MODELTRANSLATOR_H
