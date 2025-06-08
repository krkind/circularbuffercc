# Circular Buffer
This repository contains a circular buffer or a ring buffer implementation in C++ code suitable for embedded systems. The impementation uses std::mutex type for making the class thread safe. The code follows the Google C++ Style Guide but with 2 exceptions. Uses 4 spaces instead of 2 and follows STL naming conventions.

## Unittest

The added unittest uses the googletest framework and the CMake build system.

### Follow the steps below for building the unittest

1. Install the CMake,  
   if you run on an Ubuntu machine: ```sudo apt install cmake```  
   if you run on an Nixos machine: ```nix develop -c $SHELL```

2. Clone this repo and create a new folder called e.g. *build* inside the repo: ```mkdir -p build```

3. Now you shall have the following directory structure:
 ```
   <your path>
      | circularbuffercc
         | build
 ```
4. Change directory to the new directory ```build```.

5. Generate the makefiles and build the unit test:

   ```<your path>/circularbuffercc/build$ cmake .. -G Ninja```

   ```<your path>/circularbuffercc/build$ ninja```

6. Now execute the circularbufffer-gtest:

   ```<your path>/circularbuffercc/build$ test/circularbuffer-gtest```

7. You can also do

   ```<your path>/circularbuffercc/build$ ninja test```
