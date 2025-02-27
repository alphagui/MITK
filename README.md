![DE_SEG Logo][logo]

The [Medical Imaging Interaction Toolkit][DE_SEG] (DE_SEG) is a free open-source software
system for development of interactive medical image processing software. DE_SEG
combines the [Insight Toolkit][itk] (ITK) and the [Visualization Toolkit][vtk] (VTK) with an application framework.

The links below provide high-level and reference documentation targeting different
usage scenarios:

 - Get a [high-level overview][DE_SEG-overview] about DE_SEG with pointers to further
   documentation
 - End-users looking for help with DE_SEG applications should read the
   [DE_SEG User Manual][DE_SEG-usermanual]
 - Developers contributing to or using DE_SEG, please see the [DE_SEG Developer Manual][DE_SEG-devmanual]
   as well as the [DE_SEG API Reference][DE_SEG-apiref]

See the [DE_SEG homepage][DE_SEG] for details.

Supported platforms
-------------------

DE_SEG is a cross-platform C++ toolkit and officially supports:

 - Windows
 - Linux
 - macOS

For details, please read the [Supported Platforms][platforms] page.

License
-------

Copyright (c) [German Cancer Research Center (DKFZ)][dkfz]. All rights reserved.

DE_SEG is available as free open-source software under a [3-clause BSD license][license].

Download
--------

The *DE_SEGWorkbench* application and a bunch of command-line apps are released twice per year on our [Download][download] page and the [GitHub Releases][releases] page.

The official DE_SEG source code is available in the [DE_SEG Git repository][git_repo]. The Git clone command is

    git clone https://github.com/DE_SEG/DE_SEG.git

Active development takes place in the DE_SEG develop branch and its usage is advised for advanced users only.

How to contribute
-----------------

Contributions are encouraged. To make the contribution process as smooth as possible, please read [Contributing to DE_SEG][contribute] before.

Build instructions
------------------

DE_SEG uses [CMake][cmake] to configure a build tree. The following is a crash course about cloning, configuring, and building DE_SEG with Ninja on Linux or macOS when all [prerequisites][prerequisites] are met:

    git clone https://github.com/DE_SEG/DE_SEG.git
    mkdir DE_SEG-superbuild
    cmake -S DE_SEG -B DE_SEG-superbuild -G "Ninja" -D CMAKE_BUILD_TYPE=Release
    cmake --build DE_SEG-superbuild

On Windows, configuring and building with Visual Studio/MSBuild would look something like this:

    cmake -S DE_SEG -B DE_SEG-superbuild -G "Visual Studio 17 2022"
    cmake --build DE_SEG-superbuild --config Release -- -m

Read the comprehensive [build instructions][build] page for details.

Useful links
------------

 - [Homepage][DE_SEG]
 - [Download][download]
 - [Create an issue/ask for help][issues]

[logo]: https://github.com/DE_SEG/DE_SEG/raw/master/DE_SEG.png
[DE_SEG]: https://www.DE_SEG.org
[itk]: https://itk.org
[vtk]: https://vtk.org
[DE_SEG-overview]: https://docs.DE_SEG.org/nightly/
[DE_SEG-usermanual]: https://docs.DE_SEG.org/nightly/UserManualPortal.html
[DE_SEG-devmanual]: https://docs.DE_SEG.org/nightly/DeveloperManualPortal.html
[DE_SEG-apiref]: https://docs.DE_SEG.org/nightly/usergroup0.html
[platforms]: https://docs.DE_SEG.org/nightly/SupportedPlatformsPage.html
[prerequisites]: https://docs.DE_SEG.org/nightly/BuildInstructionsPage.html#BuildInstructions_Prerequisites
[build]: https://docs.DE_SEG.org/nightly/BuildInstructionsPage.html
[dkfz]: https://www.dkfz.de
[license]: https://github.com/DE_SEG/DE_SEG/blob/master/LICENSE
[download]: https://www.DE_SEG.org/Download
[releases]: https://github.com/DE_SEG/DE_SEG/releases
[git_repo]: https://github.com/DE_SEG/DE_SEG
[contribute]: https://github.com/DE_SEG/DE_SEG/blob/master/CONTRIBUTING.md
[cmake]: https://www.cmake.org
[issues]: https://github.com/DE_SEG/DE_SEG/issues
