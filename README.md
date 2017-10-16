# ESPEasy (mega branch)

Build status: [![Build Status](https://travis-ci.org/letscontrolit/ESPEasy.svg?branch=mega)](https://travis-ci.org/letscontrolit/ESPEasy)

<<<<<<< HEAD
Introduction and wiki: https://www.letscontrolit.com/wiki/index.php/ESPEasy#Introduction

This is the development branch for the next upcoming release (2.0.0). This is also known as ESPEasyMega.

Check here to learn how to use this branch and help us improving ESPEasy: http://www.letscontrolit.com/wiki/index.php/ESPEasy#Source_code_development

## Versions

* v2.0.0-devX: these are the current development releases. use these if you want bleeding edge features, or if you want to help us testing and developing. new release every month.
* v2.0.0-betaX: as soon as we think its stable/complete enough for real testing, we will start numbering beta's.
* v2.0.0-rcX: release candidates (probably stable)
* v2.0.0: This will be the first stable release. After this we will use more standard numbering. Next bugfix release will probably be 2.0.1.

## Firmware binary releases

Binary development releases are here: https://github.com/letscontrolit/ESPEasy/releases

Depending on your needs, we release different types of files.

For version 2.0.0-dev9 and higer:

Firmware name                        | Hardware                | Included plugins            |
-------------------------------------|-------------------------|-----------------------------|
ESPEasy_v2.0.0-dev9_dev_1024.bin     | ESP8266 with 1Mb flash  | Stable + Test + Development |
ESPEasy_v2.0.0-dev9_test_1024.bin    | ESP8266 with 1Mb flash  | Stable + Test               |
ESPEasy_v2.0.0-dev9_normal_1024.bin  | ESP8266 with 1Mb flash  | Stable                      |
ESPEasy_v2.0.0-dev9_dev_4096.bin     | ESP8266 with 4Mb flash  | Stable + Test + Development |
ESPEasy_v2.0.0-dev9_test_4096.bin    | ESP8266 with 4Mb flash  | Stable + Test               |
ESPEasy_v2.0.0-dev9_normal_4096.bin  | ESP8266 with 4Mb flash  | Stable                      |
ESPEasy_v2.0.0-dev9_dev_8285.bin     | ESP8285                 | Stable + Test + Development |
ESPEasy_v2.0.0-dev9_test_8285.bin    | ESP8285                 | Stable + Test               |
ESPEasy_v2.0.0-dev9_normal_8285.bin  | ESP8285                 | Stable                      |

## More info

Details and discussion are on the Experimental forum: https://www.letscontrolit.com/forum/viewforum.php?f=18

We're also on IRC: #ESPEasy @freenode

=======
Releases are here: https://github.com/letscontrolit/ESPEasy-platformio/releases [![Build Status](https://travis-ci.org/letscontrolit/ESPEasy-platformio.svg?branch=master)](https://travis-ci.org/letscontrolit/ESPEasy-platformio)

Wiki: http://www.esp8266.nu
Forum: http://www.esp8266.nu/forum


## New release management

Hi, i'm trying to continue maintenance on ESPeasy.

I've already tried to make some improvements to speed up developement: https://github.com/letscontrolit/ESPEasy-platformio

 * I've created a simple wrapper repository to allow building via platformio.
 * The git-tag and version are stored in the firmware.bin file and viewable via the webgui.
 * There are now multiple plugin sets to speed up testing and releasing of new plugins:
   * Minimal: Only contains base plugins until this point, to keep firmware size below 512k for smaller ESP8266's.
   * Normal: All stable plugins
   * Testing: New plugins that need testing before they will be marked stable. (marked with [TESTING] in the webgui)
   * Development: Plugins that are not finished yet or are broken. (marked with [DEVELOPMENT] in the webgui)

   Every release will have 4 firmware files, so everyone can choose which plugin set they want without a need to compile them yourself. This way its possible to use a stable release of ESPeasy and still have some testing plugins that you absolutely need. 
   
 * All commits will be compiled automaticly via Travis
 * When a version in git is tagged, Travis will automaticly create a new release of the binaries on github.
 * Ideally we can now gradually move most play-ground plugins over to the main tree and add them to the approriate plugin set.
 * I would love to use different versioning numbering from now on, perhaps sematic versioning (http://semver.org), starting with 1.x.x since our internal plugin API is already stable.
 
Currently i'm merging the open pull requests and try to give some feedback on them if neccesary.
 
Edwin (psy0rz)
>>>>>>> 884205feea8f19e7ed1e7846246d99ff4ebb8872
