# libplasma

Foundational libraries, components, and tools of the Plasma workspaces

## Introduction
libplasma provides the following:
- QML components that can be used by any Plasma shell
- A C++ library: `libplasma` itself
- Script engines

## QML components
### org.kde.plasma.core

Bindings for libplasma functionality, such FrameSvg, see @ref core.

### org.kde.plasma.components
Graphical components for common items such as buttons, lineedits, tabbars and so on. Compatible subset of the MeeGo components used on the N9, see @ref plasmacomponents.

### org.kde.plasma.extras
Extra graphical components that extend org.kde.plasma.components but are not in the standard api, see @ref plasmaextracomponents.

### org.kde.plasma.plasmoid
Attached properties for manipulating the current applet or containment, see @ref libplasmaquick

## libplasma
This C++ library provides:
- rendering of SVG themes
- loading of files from a certain filesystem structure: packages
- data access through data engines
- loading of the plugin structure of the workspace: containments and applets

See @ref libplasma.

## Script engines
Provides support to create applets or containments in various scripting languages.

## Contributing to libplasma

We appreciate your interest in contributing! We use the [GitHub flow - GitHub Docs](https://docs.github.com/en/get-started/using-github/github-flow) for code contributions. To report a bug, please use the libplasma-sonic bug tracker at [Issues · Sonic-DE/libplasma-sonic](https://github.com/Sonic-DE/libplasma-sonic/issues).

## Getting in contact

We'd love to hear from you on one of our channels. To get end-user support, please also check your distribution's chat or forum.

<img src="./.github/icons/bluesky.svg">&nbsp;[Bluesky](https://bsky.app/profile/sonicdesktop.bsky.social)&nbsp; <img src="./.github/icons/discord.svg">&nbsp;[Discord](https://discord.gg/cNZMQ62u5S) &nbsp; <img src="./.github/icons/mastodon.svg">&nbsp;[Mastodon](https://mastodon.social/@sonicdesktop) &nbsp; <img src="./.github/icons/matrix.svg">&nbsp;[Matrix](https://matrix.to/#/#sonicdesktop:matrix.org) &nbsp; <img src="./.github/icons/oftc.svg">&nbsp;[OFTC IRC](https://webchat.oftc.net/?channels=sonicde%2Csonicde-devel%2Csonicde-dist&uio=MT11bmRlZmluZWQb1) &nbsp; <img src="./.github/icons/telegram.svg">&nbsp;[Telegram](https://t.me/sonic_de) &nbsp; <img src="./.github/icons/x.svg">&nbsp;[X (Twitter)](https://x.com/SonicDesktop)

