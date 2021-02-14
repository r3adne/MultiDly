# MultiDly
An expressive and musical multitap delay.

## Building

The only prerequisite is [JUCE](https://github.com/juce-framework/JUCE). 

This project has migrated to CMake, and requires at least CMake 3.15. You can build this repo with the CMakeLists.txt file located in the root directory.     


~~Because of the ease of development, we're using the Projucer for now. Soon we will probably move to a cmake based build system, but for the time being, as you'll need to install JUCE anyway, you may as well build the Projucer found in `JUCE/extras/Projucer` by either using your supported IDE of choice (XCode, VS, or makefile) or using the included CMakeLists file.~~ 

~~In order to build MultiDly, open MultiDly.jucer in the Projucer you've just built, and save it for your IDE. Open the project in your IDE and build.~~ 

~~In order to keep clutter out of this repo, anything under the Builds tab is ignored. Please don't make a PR with any strange IDE files in a commit (like `Builds/MacOSX/multidly.xcodeproj/.../UserInterfaceState.xcuserstate`).~~ 

~~In order to target VST3 hosts, you'll need to download the VST3 library from Steinberg and add its path to the Projucer.~~ 


## Using this plugin

MultiDly is written with JUCE and is an audio plugin. As such various audio plugin types are supported. For the time being, I am only ensuring that the project builds for AudioUnit and VST3, but I hope to eventually support AAX and possibly RTAS and VST. 

### Operating Systems
Until further notice this project is only tested on MacOS. Currently I am testing on MacOS 10.14 and 10.15. In the future, I hope to support Windows and, if people want it, Linux support shouldn't be too difficult.

### Plugin Hosts
Regular testing takes place using the AudioPluginHost from the JUCE library, which is a simple host meant for testing. DAWs supported include at least Ableton Live 10 and Logic Pro X 10.4. Eventually I will try to support Pro Tools as well.


## Versioning

The project is not quite at the first stage of completeness and currently consists only of an engine and the framing of a UI. Some work needs to be done to prepare the project to be built for releasing. The GitHub releases pane will be used to categorize each release, and builds along with an installer script will be hosted on another repository.



## License

MultiDly is licensed under AGPLv3. See info about it [here](https://www.gnu.org/licenses/agpl-3.0.en.html).



 

