This repository contains a collection of free, open-source audio plugins made or maintained by sainsay.

Most plugins are made to work without GUI and only exposing a simple set of controls. However, if a plugin has a GUI, the non-GUI variant is also available.

Navigate to the [download page](https://github.com/sainsay/AudioPlugins/releases) and download the .zip file for your system from the "All Plugins" release.

A full list of all the availible plugins and their features can be found [here](PLUGINS.md).

## Building from source
The project depends on [Juce](https://github.com/juce-framework/JUCE) as a submodule to properly initialize after cloning this repository please run `git submodule init` and `git submodule update` before building the project.

This project uses [CMake](https://cmake.org/) and requires a minimum version of `CMake 3.15 or later`

## Contributing
For bug reports and features requests or the addition new plugins, please visit the [Issues](https://github.com/sainsay/AudioPlugins/issues) page. Pull request without any issue or discussion attached will likely be declined. Any pull request that changes the submodules will be declined unless explicitly given permission by the owner of this repository.

## License
All plugins are covered by a
[GNU General Public License v.3](https://www.gnu.org/licenses/gpl-3.0.en.html).

### Juce
The core JUCE modules (juce_audio_basics, juce_audio_devices, juce_blocks_basics, juce_core
and juce_events) are permissively licensed under the terms of the
[ISC license](http://www.isc.org/downloads/software-support-policy/isc-license/).
Other modules are covered by a
[GPL/Commercial license](https://www.gnu.org/licenses/gpl-3.0.en.html).

There are multiple commercial licensing tiers for JUCE, with different terms for each:
- JUCE Personal (developers or startup businesses with revenue under 50K USD) - free
- JUCE Indie (small businesses with revenue under 500K USD) - $40/month
- JUCE Pro (no revenue limit) - $130/month
- JUCE Educational (no revenue limit) - free for bona fide educational institutes

For full terms see [LICENSE.md](https://github.com/juce-framework/JUCE/blob/master/LICENSE.md).
