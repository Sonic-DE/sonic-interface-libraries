/*
    SPDX-FileCopyrightText: 2023 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "action.h"

IconGroup::IconGroup(ActionExtension *parent)
    : QObject(parent)
    , m_action(parent->action())
{
}

IconGroup::~IconGroup()
{
}

void IconGroup::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }

    m_name = name;
    m_action->setIcon(QIcon::fromTheme(name));
    Q_EMIT nameChanged();
}

QString IconGroup::name() const
{
    return m_name;
}

ActionExtension::ActionExtension(QObject *parent)
    : QObject(parent)
    , m_action(qobject_cast<QAction *>(parent))
    , m_iconGroup(new IconGroup(this))
{
}

ActionExtension::~ActionExtension()
{
}

bool ActionExtension::isSeparator() const
{
    return m_action->isSeparator();
}

void ActionExtension::setSeparator(bool separator)
{
    if (separator == m_action->isSeparator()) {
        return;
    }

    m_action->setSeparator(separator);

    Q_EMIT isSeparatorChanged();
}

void ActionExtension::setActionGroup(QActionGroup *group)
{
    if (group == m_action->actionGroup()) {
        return;
    }

    m_action->setActionGroup(group);
    Q_EMIT actionGroupChanged();
}

QActionGroup *ActionExtension::actionGroup() const
{
    return m_action->actionGroup();
}

QAction *ActionExtension::action() const
{
    return m_action;
}

#include "moc_action.cpp"
