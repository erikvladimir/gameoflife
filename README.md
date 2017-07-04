# Game of life

## Section 1. Introduction

The executable can be run from command line. It is text-based but it uses unicode characters and ncurses library to delivered an improved visual output.

The size of the visual layout is fixed (it does not resize at the moment) and equal to 80x27 in characters. Please get sure your console windows is at least of that size to get a better experience.


## Section 2. Compile and Run

1. **Xcode project**
 A full xcode project is provided. This can be open, edited and tested.

3. **Make scripts**
As an alternative, a Makefile is also given. Yu can use the following commands:

```sh
make build run
```

## Section 3. Usage

The user can use the following **keys** when available:
* **[P]** : Play, animate game
* **[S]** : Stop, to pause
* **[X]** : Quit game

You can use your mouse to toggle the status of any cell at any time. However it is prefereable to pause the game first.

## Section 4. Requirements

You need to have **ncurses** and **unicode** support. Please check that your console it is at least **80x27 characters
**.

This application was tested on MacOS 10.12 and Debian Jessie with ncurses and unicode support on console.


