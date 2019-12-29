# DotMG Library

Recommended Arduino library for use with the [dotMG DIY handheld game console](https://modmatic.io/dotmg).

## Installation

1. Download this repo as a ZIP file to a known location on your machine.

2. Install the library using the following guide:

    [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries#toc5)

## Start up features

The `begin()` function, used to initialize the library, includes features that are intended to be available to all sketches using the library (unless the sketch developer has chosen to disable one or more of them to free up some code space):

## Using the library in a sketch

As with most libraries, to use `DotMG` in your sketch, you must include its header file at the start:

```cpp
#include <DotMG.h>
```

You must then create a `DotMG` class object:

```cpp
DotMG dmg;
```

To initialize the library, you must call its `begin()` function. This is usually done at the start of the sketch's `setup()` function:

```cpp
void setup()
{
  dmg.begin();
  // more setup code follows, if required
}
```

The rest of the `DotMG` functions will now be available for use.

Sample sketches have been included with the library as examples of how to use it. To load an example, for examination and uploading to the dotMG, using the Arduino IDE menus select:

`File > Examples > DotMG`

### Audio

The `BeepPin1` and `BeepPin2` classes are available from the [`Arduboy2BeepDotMG`](https://github.com/modmatic/Arduboy2BeepDotMG) library to generate simple square wave tones.

For more complex melodies, see the [`ArduboyTonesDotMG`](https://github.com/modmatic/ArduboyTonesDotMG) library.

### To make more code space available to sketches

#### Remove the text functions

If your sketch doesn't use any of the functions for displaying text, such as `setCursor()` and `print()`, you can remove them. You could do this if your sketch generates whatever text it requires by some other means. Removing the text functions frees up code by not including the font table and some code that is always pulled in by inheriting the [Arduino *Print* class](http://playground.arduino.cc/Code/Printclass).

To eliminate text capability in your sketch, when creating the library object simply use the `DotMGBase` class instead of `DotMG`:

For example, if the object will be named `dmg`:

Replace

```cpp
DotMG dmg;
```

with

```cpp
DotMGBase dmg;
```
