# LFU cache project

This is a repository of group project in Konstantin Vladimirov's informatics group.

## The description of the algorythm

LFU means least frequently used. The element which was requested the least number of times removed from the list.

More information about implementation is on the [website](http://dhruvbird.com/lfu.pdf "website").

## Installation

1. Make sure that CMake and git is installed on your computer.

2. Then you need to run:            

        git clone git@github.com:sovus-tartar LFU_cache_project.git

3. Then go to the "LFU_cache_project" directory:

        cd LFU_cache_project

4. Create a new folder "build" and go to it:

        mkdir build
        cd build

5. Run CMake:

        cmake ../
        cmake --build ./

6. The program in the file called "lfu". Enjoy!