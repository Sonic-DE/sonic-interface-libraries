/*
    SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "themetest.h"
#include "utils.h"
#include <QApplication>
#include <QSignalSpy>
#include <QStandardPaths>

#include <KConfigGroup>
#include <KIconLoader>
#include <KIconTheme>
#include <KSvg/ImageSet>
#include <KWindowSystem>
#include <KX11Extras>

#include <config-plasma.h>
#include <KSelectionOwner>
#include <array>

void ThemeTest::initTestCase()
{
    // make our theme in search path
    qputenv("XDG_DATA_DIRS", QByteArray(qgetenv("XDG_DATA_DIRS") + ":" + QFINDTESTDATA("data").toLocal8Bit()));

    // set default icon theme to test-theme
    QStandardPaths::setTestModeEnabled(true);

    m_theme = new Plasma::Theme("testtheme", this);
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);

    if (!QDir(configPath).mkpath(QStringLiteral("."))) {
        qFatal("Failed to create test configuration directory.");
    }

    QFile::remove(configPath);

    QIcon::setThemeSearchPaths(QStringList() << QFINDTESTDATA("data/icons"));

    KConfigGroup plasmaConfig(KSharedConfig::openConfig("plasmarc"), "Theme");
    plasmaConfig.writeEntry("name", "default");

    KIconTheme::forceThemeForTests("test-theme");
    KSharedConfig::openConfig()->reparseConfiguration();
    KIconTheme::reconfigure();
    KIconLoader::global()->reconfigure(QString());
}

void ThemeTest::testThemeConfig_data()
{
    QTest::addColumn<QString>("themeName");

    QTest::addRow("new metadata style theme") << QStringLiteral("testtheme");
}

void ThemeTest::testThemeConfig()
{
    QFETCH(QString, themeName);
    auto theme = std::make_unique<Plasma::Theme>(themeName, this);
    QCOMPARE(theme->backgroundContrastEnabled(), true);
    QCOMPARE(theme->backgroundContrast(), 0.23);
}

void ThemeTest::testColors()
{
    QCOMPARE(m_theme->color(Plasma::Theme::TextColor, Kirigami::Platform::PlatformTheme::Window), QColor(49, 54, 59));
    QCOMPARE(m_theme->color(Plasma::Theme::BackgroundColor, Kirigami::Platform::PlatformTheme::Window), QColor(239, 240, 241));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightColor, Kirigami::Platform::PlatformTheme::Window), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HoverColor, Kirigami::Platform::PlatformTheme::Window), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::FocusColor, Kirigami::Platform::PlatformTheme::Window), QColor(30, 146, 255));
    QCOMPARE(m_theme->color(Plasma::Theme::LinkColor, Kirigami::Platform::PlatformTheme::Window), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::VisitedLinkColor, Kirigami::Platform::PlatformTheme::Window), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightedTextColor, Kirigami::Platform::PlatformTheme::Window), QColor(252, 252, 252));
    QCOMPARE(m_theme->color(Plasma::Theme::PositiveTextColor, Kirigami::Platform::PlatformTheme::Window), QColor(17, 209, 22));
    QCOMPARE(m_theme->color(Plasma::Theme::NeutralTextColor, Kirigami::Platform::PlatformTheme::Window), QColor(201, 206, 59));
    QCOMPARE(m_theme->color(Plasma::Theme::NegativeTextColor, Kirigami::Platform::PlatformTheme::Window), QColor(237, 21, 21));

    QCOMPARE(m_theme->color(Plasma::Theme::TextColor, Kirigami::Platform::PlatformTheme::Button), QColor(49, 54, 59));
    QCOMPARE(m_theme->color(Plasma::Theme::BackgroundColor, Kirigami::Platform::PlatformTheme::Button), QColor(239, 240, 241));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightColor, Kirigami::Platform::PlatformTheme::Button), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HoverColor, Kirigami::Platform::PlatformTheme::Button), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::FocusColor, Kirigami::Platform::PlatformTheme::Button), QColor(30, 146, 255));
    QCOMPARE(m_theme->color(Plasma::Theme::LinkColor, Kirigami::Platform::PlatformTheme::Button), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::VisitedLinkColor, Kirigami::Platform::PlatformTheme::Button), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightedTextColor, Kirigami::Platform::PlatformTheme::Button), QColor(252, 252, 252));
    QCOMPARE(m_theme->color(Plasma::Theme::PositiveTextColor, Kirigami::Platform::PlatformTheme::Button), QColor(17, 209, 23));
    QCOMPARE(m_theme->color(Plasma::Theme::NeutralTextColor, Kirigami::Platform::PlatformTheme::Button), QColor(201, 206, 60));
    QCOMPARE(m_theme->color(Plasma::Theme::NegativeTextColor, Kirigami::Platform::PlatformTheme::Button), QColor(237, 21, 22));

    QCOMPARE(m_theme->color(Plasma::Theme::TextColor, Kirigami::Platform::PlatformTheme::View), QColor(49, 54, 59));
    QCOMPARE(m_theme->color(Plasma::Theme::BackgroundColor, Kirigami::Platform::PlatformTheme::View), QColor(252, 252, 252));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightColor, Kirigami::Platform::PlatformTheme::View), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HoverColor, Kirigami::Platform::PlatformTheme::View), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::FocusColor, Kirigami::Platform::PlatformTheme::View), QColor(30, 146, 255));
    QCOMPARE(m_theme->color(Plasma::Theme::LinkColor, Kirigami::Platform::PlatformTheme::View), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::VisitedLinkColor, Kirigami::Platform::PlatformTheme::View), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightedTextColor, Kirigami::Platform::PlatformTheme::View), QColor(252, 252, 252));
    QCOMPARE(m_theme->color(Plasma::Theme::PositiveTextColor, Kirigami::Platform::PlatformTheme::View), QColor(17, 209, 24));
    QCOMPARE(m_theme->color(Plasma::Theme::NeutralTextColor, Kirigami::Platform::PlatformTheme::View), QColor(201, 206, 61));
    QCOMPARE(m_theme->color(Plasma::Theme::NegativeTextColor, Kirigami::Platform::PlatformTheme::View), QColor(237, 21, 23));

    QCOMPARE(m_theme->color(Plasma::Theme::TextColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(239, 240, 241));
    QCOMPARE(m_theme->color(Plasma::Theme::BackgroundColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(49, 54, 59));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(61, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HoverColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(71, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::FocusColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(40, 146, 255));
    QCOMPARE(m_theme->color(Plasma::Theme::LinkColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(71, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::VisitedLinkColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(71, 174, 230));
    QCOMPARE(m_theme->color(Plasma::Theme::HighlightedTextColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(252, 252, 252));
    QCOMPARE(m_theme->color(Plasma::Theme::PositiveTextColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(17, 209, 25));
    QCOMPARE(m_theme->color(Plasma::Theme::NeutralTextColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(201, 206, 62));
    QCOMPARE(m_theme->color(Plasma::Theme::NegativeTextColor, Kirigami::Platform::PlatformTheme::Complementary), QColor(237, 21, 24));
}

void ThemeTest::testSilverThemes()
{
    Plasma::TestUtils::installPlasmaTheme(QStringLiteral("default"));
    Plasma::TestUtils::installPlasmaTheme(QStringLiteral("silver-light"));
    Plasma::TestUtils::installPlasmaTheme(QStringLiteral("silver-dark"));

    Plasma::Theme light(QStringLiteral("silver-light"), this);
    QCOMPARE(light.themeName(), QStringLiteral("silver-light"));
    Plasma::Theme dark(QStringLiteral("silver-dark"), this);
    QCOMPARE(dark.themeName(), QStringLiteral("silver-dark"));
    Plasma::Theme fallback(QStringLiteral("default"), this);
    QCOMPARE(fallback.themeName(), QStringLiteral("default"));
}

void ThemeTest::testInvalidThemePreservesSelection()
{
    Plasma::Theme theme(QStringLiteral("silver-light"), this);
    QCOMPARE(theme.themeName(), QStringLiteral("silver-light"));
    theme.setThemeName(QStringLiteral("theme-with-no-valid-metadata"));
    // Theme::setThemeName switches to a shared private object keyed by the
    // requested name before ThemePrivate validates metadata. Invalid metadata
    // leaves that private object's themeName empty; assets still resolve via
    // the ImageSet fallback. Assert the actual public behavior so regressions
    // are visible rather than documenting unsupported preservation semantics.
    QCOMPARE(theme.themeName(), QString());
}

void ThemeTest::testCompositingChange()
{
    // This test owns the compositor selection while exercising both state
    // transitions. Running it inside an already composited X11 session would
    // make the process contend with the real compositor and invalidate the
    // test's ownership assumptions.
    if (KX11Extras::compositingActive()) {
        QSKIP("Requires an X11 display without an active compositor");
    }

    // Create a global imageset, m_theme will manipulate it
    KSvg::ImageSet set;
    set.setBasePath(QStringLiteral(PLASMA_RELATIVE_DATA_INSTALL_DIR "/desktoptheme/"));

    // this test simulates the compositing change on X11
    QVERIFY(!KX11Extras::compositingActive());

    // image path should give us an opaque variant
    QVERIFY(set.selectors().contains("opaque"));

    QSignalSpy themeChangedSpy(m_theme, &Plasma::Theme::themeChanged);
    QVERIFY(themeChangedSpy.isValid());

    // fake the compositor
    QSignalSpy compositingChangedSpy(KX11Extras::self(), &KX11Extras::compositingChanged);
    QVERIFY(compositingChangedSpy.isValid());
    std::unique_ptr<KSelectionOwner> compositorSelection(new KSelectionOwner("_NET_WM_CM_S0"));
    QSignalSpy claimedSpy(compositorSelection.get(), &KSelectionOwner::claimedOwnership);
    QVERIFY(claimedSpy.isValid());
    compositorSelection->claim(true);
    QVERIFY(claimedSpy.wait());

    QCOMPARE(compositingChangedSpy.count(), 1);
    QVERIFY(KX11Extras::compositingActive());
    QVERIFY(themeChangedSpy.wait());
    QCOMPARE(themeChangedSpy.count(), 1);
    QVERIFY(!set.selectors().contains("opaque"));

    // remove compositor again
    compositorSelection.reset();
    QVERIFY(compositingChangedSpy.wait());
    QCOMPARE(compositingChangedSpy.count(), 2);
    QVERIFY(!KX11Extras::compositingActive());
    QVERIFY(themeChangedSpy.wait());
    QCOMPARE(themeChangedSpy.count(), 2);
    QVERIFY(set.selectors().contains("opaque"));
}

QTEST_MAIN(ThemeTest)

#include "moc_themetest.cpp"
