# Collaborative notepad client
A notepad application in which every client in a network can work upon a certain (user chosen) document.

![Project logo](https://github.com/MihaiBojescu/Collaborative-notepad/blob/master/res/app.png "Collaborative notepad")

# Dependencies
The compilation dependencies for this project are:
* glibc (GNU C Library)
* Qt framework 5.10

Dependencies can be installed like this:
* `sudo apt install glibc qt5-default` for Debian based distributions
* `sudo pacman -Sy glibc qt5` for Arch based distributions

# Compiling
Run `qmake Project.pro`, and then `make -f Makefile` in the root directory of the project.

# Usage
The keybings are the following:
* `CTRL + N` -> New file
* `CTRL + S` -> Save file
* `CTRL + O` -> Open file
* `CTRL + U` -> Open network file
* `CTRL + W` -> Close tab

The application is made to feel natural. A client can start to edit right away. Changes will be mirrored automatically to the server and also to the other connected client using the same file the used is using.
