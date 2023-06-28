/*
    SPDX-FileCopyrightText: 2023 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLASMAACTION_H
#define PLASMAACTION_H

#include <QAction>
#include <QActionGroup>
#include <QObject>
#include <qqml.h>
#include <qtclasshelpermacros.h>
#include <qtmetamacros.h>

class QQuickItem;

namespace PlasmaQuick
{
class SharedQmlEngine;
}

class ActionExtension;

class IconGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit IconGroup(ActionExtension *parent = nullptr);
    ~IconGroup();

    void setName(const QString &name);
    QString name() const;

Q_SIGNALS:
    void nameChanged();

private:
    QAction *m_action;
    QString m_name;
};

class ActionExtension : public QObject
{
    Q_OBJECT
    Q_PROPERTY(IconGroup *icon MEMBER m_iconGroup CONSTANT)
    Q_PROPERTY(bool isSeparator READ isSeparator WRITE setSeparator NOTIFY isSeparatorChanged)
    Q_PROPERTY(QActionGroup *actionGroup READ actionGroup WRITE setActionGroup NOTIFY actionGroupChanged)
    Q_PROPERTY(QVariant shortcut READ shortcut WRITE setShortcut NOTIFY shortcutChanged)

public:
    explicit ActionExtension(QObject *parent = nullptr);
    ~ActionExtension() override;

    bool isSeparator() const;
    void setSeparator(bool setSeparator);

    void setActionGroup(QActionGroup *group);
    QActionGroup *actionGroup() const;

    void setShortcut(const QVariant &var);
    QVariant shortcut() const;

    QAction *action() const;

Q_SIGNALS:
    void isSeparatorChanged();
    void actionGroupChanged();
    void shortcutChanged();

private:
    QAction *m_action;
    IconGroup *m_iconGroup;
    QString m_icon;
};

struct QActionForeign {
    Q_GADGET
    QML_FOREIGN(QAction)
    QML_ELEMENT
    QML_EXTENDED(ActionExtension)
};

class ActionGroup : public QActionGroup
{
    Q_OBJECT
public:
    ActionGroup(QObject *parent = nullptr)
        : QActionGroup(parent)
    {
    }
};

#endif
