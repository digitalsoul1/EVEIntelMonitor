# EVEIntelMonitor
EVEIntelMonitor is an application designed for EVE Online players to monitor in-game intel channels
and provide notification via voice synthesis when a relevant message is detected.

The application also uses Eve Scout's API to notify users of nearest wormhole connections to Turnur and Thera.

Currently a binary bundle is provided only for MacOS but the application can be built for Windows.

It is written in C++ and uses the Qt framework for the GUI, networking, voice synthesis and multi-threading. The build system in CMake.
![EVEIntelMonitor](screenshot.png)

# License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.

The QT framework is licensed under the LGPL v3.0 - see the [LICENSE_QT.md](LICENSE_QT.md) file for details.

This program uses assets from the EVE Online game, which are used under the terms of the [CCP Games EVE Online EULA](https://www.eveonline.com/article/eve-online-end-user-license-agreement).

EVE Online is a registered trademark of CCP hf. All rights are reserved worldwide.

# Current issues
- [MacOS] Voice synthesis is using deprecated MacOS APIs. The QML TextToSpeech.enqueue(...) function does not work as expected with the latest TTS engine. The workaround used is to use the deprecated macos engine (darwin being the current one). If apple remove the macos engine in the future and the bug is not fixed I will write my own enqueue() function. I have posted a [bug report](https://bugreports.qt.io/browse/QTBUG-122884) to the Qt bug tracker.
- Not tested on Windows. The application should build and run on Windows but I have not tested it. I will test it when I have access to a Windows machine.