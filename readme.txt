Bit Fiddle is an application running on Windows and macOS, capable of converting
decimal, hexadecimal and binary numbers on the fly.

This application serves as a demo application to learn how to use NALib to create
GUIs (graphical user interfaces) in plain C.


Cloning with submodules:
------------------------
Bit Fiddle requires the two submodules "NALib" and "ManderApp".

1. Either clone Bit Fiddle with the following git command:
   git clone --recursive https://XXXXXX/BitFiddle

2. Or write the following commands:
   git clone https://XXXXXX/BitFiddle
   cd BitFiddle
   git submodule init
   git submodule update

3. Open the provided project/solution file in the BitFiddle/proj folder.
   It should compile and run without errors in XCode and VisualStudio.


Documentation:
--------------
- Can be found directly within the source files.


Contribute:
-----------
You may contribute but be aware that this is a published selling application and
shall at the same time be available as a demo for the GUI capabilities for NALib.


History:
--------
Bit Fiddle originated on the authors website as a PHP implementation of numeral
system conversion back in 2005 or maybe even earlier. Go have a look in the
internet archive:

https://web.archive.org/web/20050220024908/http://www.manderby.com/programme/umrechner.php

Later on it was re-programmed in JavaScript (before it became cool) and is still
available today here:

https://manderc.com/concepts/umrechner/index.php

A few years later then, the application Bit Fiddle for Mac OS X was created and
sold on the AppStore where it can still be found today.

https://apps.apple.com/us/app/bit-fiddle/id669462988?mt=12

In January 2020, the application had been rewritten with the latest NALib and
was then available for download for Windows as well:

https://manderc.com/apps/bitfiddle/index.php

And a few months after that, the source code had been published on GitHub for
everyone to see on how to use NALib to create GUIs in the C programming language.