/*
    SPDX-FileCopyrightText: 2006-2010 Aaron Seigo <aseigo@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLASMA_SVG_P_H
#define PLASMA_SVG_P_H

#include <QHash>
#include <QPointer>
#include <QSharedData>
#include <QSvgRenderer>
#include <QExplicitlySharedDataPointer>
#include <QObject>

namespace Plasma
{

class Svg;

class SharedSvgRenderer : public QSvgRenderer, public QSharedData
{
    Q_OBJECT
public:
    typedef QExplicitlySharedDataPointer<SharedSvgRenderer> Ptr;

    explicit SharedSvgRenderer(QObject *parent = nullptr);
    SharedSvgRenderer(
        const QString &filename,
        const QString &styleSheet,
        QHash<QString, QRectF> &interestingElements,
        QObject *parent = nullptr);

    SharedSvgRenderer(
        const QByteArray &contents,
        const QString &styleSheet,
        QHash<QString, QRectF> &interestingElements,
        QObject *parent = nullptr);

private:
    bool load(
        const QByteArray &contents,
        const QString &styleSheet,
        QHash<QString, QRectF> &interestingElements);
};

class SvgPrivate
{
public:
    struct CacheId {
        double width;
        double height;
        QString filePath;
        QString elementName;
        int status;
        double dpr;
        int colorGroup;
    };

    SvgPrivate(Svg *svg);
    ~SvgPrivate();

    //This function is meant for the rects cache
    CacheId cacheId(const QString &elementId) const;

    //This function is meant for the pixmap cache
    QString cachePath(const QString &path, const QSize &size) const;

    bool setImagePath(const QString &imagePath);

    Theme *actualTheme();
    Theme *cacheAndColorsTheme();

    QPixmap findInCache(const QString &elementId, qreal ratio, const QSizeF &s = QSizeF());

    void createRenderer();
    void eraseRenderer();

    QRectF elementRect(const QString &elementId);
    QRectF findAndCacheElementRect(const QString &elementId);

    void checkColorHints();

    //Following two are utility functions to snap rendered elements to the pixel grid
    //to and from are always 0 <= val <= 1
    qreal closestDistance(qreal to, qreal from);

    QRectF makeUniform(const QRectF &orig, const QRectF &dst);

    //Slots
    void themeChanged();
    void colorsChanged();

    static QHash<QString, SharedSvgRenderer::Ptr> s_renderers;
    static QPointer<Theme> s_systemColorsCache;
    static qreal s_lastScaleFactor;

    Svg *q;
    QPointer<Theme> theme;
 //   QHash<QString, QRectF> localRectCache;
//     QHash<CacheId, QRectF> localRectCache;
  //  QMultiHash<QString, QSize> elementsWithSizeHints;
    SharedSvgRenderer::Ptr renderer;
    QString themePath;
    QString path;
    QSizeF size;
    QSizeF naturalSize;
    QChar styleCrc;
    Theme::ColorGroup colorGroup;
    unsigned int lastModified;
    qreal devicePixelRatio;
    qreal scaleFactor;
    Svg::Status status;
    bool multipleImages : 1;
    bool themed : 1;
    bool useSystemColors : 1;
    bool fromCurrentTheme : 1;
    bool applyColors : 1;
    bool usesColors : 1;
    bool cacheRendering : 1;
    bool themeFailed : 1;
};


class SvgRectsCache : public QObject {
    Q_OBJECT
public:
    SvgRectsCache(QObject *parent = nullptr);

    static SvgRectsCache *instance();

    void insert(SvgPrivate::CacheId cacheId, const QRectF &rect, unsigned int &lastModified);
    bool findElementRect(SvgPrivate::CacheId cacheId, QRectF &rect);
    void loadImageFromCache(const QString &path);
    void expireCache(const QString &path);

    void setNaturalSize(const QString &path, qreal scaleFactor, const QSizeF &size);
    QSizeF naturalSize(const QString &path, qreal scaleFactor);

    QList<QSize> sizeHintsForId(const QString &path, const QString &id);
    void setSizeHintsForId(const QString &path, const QString &id, QList<QSize> sizes);

private:
    QTimer *m_configSyncTimer = nullptr;
    KSharedConfigPtr m_svgElementsCache;
    QHash<uint, QRectF> m_localRectCache;
    QHash<QString, QList<QSize>> m_sizeHintsForId;
};

}

uint qHash(const Plasma::SvgPrivate::CacheId &id);

#endif

