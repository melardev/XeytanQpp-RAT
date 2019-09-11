# WARNING
Project that is still a work in progress, it is not finished nor stable. The progress is coming very slowly,
because I do a lot of other things at the same time, do not expect this application to be production ready any time soon.


# Introduction
RAT tool created to waste meaningfully some of my free time. It is written in C++ using Qt Widgets.

# Features
- Chat
- Desktop Streaming
- Camera Streaming
- Process List
- Reverse Shell
- Get Client info

# Tips
- While refactoring be aware of QT methods you use that work with template args, at the time of writing it is
easy to make it right, but if you then refactor your code and change data types for a variable, if you used
that variable inside a function taking template args, you will see no errors, but the compilation will fail.
So always keep track of methods you use that work through templates and make sure you refactor them as well.

https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5/16795664
- Qt is a C++ Framework, and you know, C++ is C++ ... a simple include present/missing may cause your app to not build with success,
this is one of the really nasty problems I had for 15 mins: not including QDataStream in media_defs.h caused this project not to compile
because of a link error due to not finding << operator for qint32; Interestingly enough, the XeytaCuteClient app which also uses that header
compiled fine, the reason was that XeytanCuteClient included QDataStream somewhere else, and the compiler found the << qint32 definition,
but in this project, the place where includes media_defs it does not include QDataStream so the operators defined in that QDataStream.h
are never found, this is why I had a link error ... crazy!!!!!

# TODO
- Encrypting network traffic.
- Handle windows closing, i.e Remote Desktop window closed should lead to closing RemoteDesktop Session
- Melt
- Persistence
- Builder([For Windows](https://docs.microsoft.com/en-us/windows/win32/menurc/resources-functions); [For Linux](https://www.linuxjournal.com/content/embedding-file-executable-aka-hello-world-version-5967); [For Mac](https://developer.apple.com/documentation/foundation/nsbundle))
- The networking stuff is run in a separate thread from the app, so calling netServerService methods from XeytaQpipi application
class may lead to issues when trying to queue events into objets owned by another thread, this is indeed what happened to me
in Close connection functionality. To keep the code simple without much indirections I leave the code as is, but in the future
I have to refactor the code and anything that goes from XeytaQpipi to NetServerService has to begin with an emit signal and not
a direct call, not this: `netServerService->something()` but this `emit something()`.
- Go through constructors, remove those unusued
- Go through destructors, add logic to release resources.
- Move all the code from slots that receive objects to slots receiving QSharedPointer
- There is a bug triggered when we stream desktop, then close window, the packets get a with delay,
if we request then, process list, and then info, we will get process list instead.
- To keep it simple I used QSharedPointer everywhere instead of using QScopedPointer, this should be refactored
- To keep it simple and avoid some errors I created default constructors, it mat not be a good idea in some cases,
review that.
- Chat
- Resize Desktop Capture
- Put The CameraService and DesktopHandler into a single file
- Remove camera picture file auto saved by Qt, in Windows 10 (my system) for some reason <br>
it gets saved into OneDrive folder ...., delete that image, we don't need it.
- Audio recording
- There is a problem requesting connection to be closed when the client is streaming Camera
- A lot of refactoring, I have to move a lot of things from NetService to the Controller class (XeytanQpp.cpp)
