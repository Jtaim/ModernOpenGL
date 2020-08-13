### Introduction 
A project to learn basic OpenGL.

### Getting Started
Getting project setup:
1.	Get an IDE
	- Get Microsoft Visual Studio Community [download](https://www.visualstudio.com/vs/community/)
2.	Software dependencies
	- I'm using [vcpkg](https://github.com/Microsoft/vcpkg).  This a package manager for c\c++ libraries from Microsoft.
	- for this project I'm using GLFW, GLEW, GLM and SOIL2.
    - using the vcpkg I installed the file as ./vcpkg install "libfile":x64-windows-static
3.  References
	- [Learn OpenGL](https://learnopengl.com/)
	- [OpenGL](https://www.opengl.org/)
	- [GLFW](http://www.glfw.org/) is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop.
    - [GLEW](http://glew.sourceforge.net/) is an OpenGL Extension Wrangler Library (GLEW) for cross-platform open-source C/C++ extension loading library.
    - [GLM](https://glm.g-truc.net/0.9.9/index.html) is an OpenGL Mathematics (GLM) header only C++ mathematics library for graphics software.
    - [SOIL2](https://bitbucket.org/SpartanJ/soil2/src/default/) is a tiny C library used primarily for uploading textures into OpenGL.
	- [docs.GL](http://docs.gl/) excellent OpenGL reference

### Additional Project Settings
Once project is created there are some additional items to add to the project.<br>
I want can copy the above includes and libs into your project or just point to where they are.<br>
**Next steps is an example of a structure.**<br>
In the same folder as the .sln file create a folder for the dependencies "deps" and in that folder create 
these two additional folders named include and lib.<br>
In the include folder create and additional folder for the 3rd party includes just to keep things organized.
example: from the solution directory deps/include/GLFW.<br>
Place any dlls with the program executable if not using static libraries.  I will be using the static libraries

### Project Properties:

**NOTE:** make sure All Configurations is selected
1. Configuration Properties > VC++ Directories
	a. in the include directories include select edit then add this $(SolutionDir)deps\include\
	b. in the library directories include select edit then add this $(SolutionDir)deps\lib\
2. Configuration Properties > Linker > Input
	a. add the Additional Dependencies (glfw3.lib; glew.lib; soil2.lib opengl32.lib)
    b. **may need to do release and debug configurations separately additional dependencies.**
3. Set an entry point if needed for the program:
    a. Configuration Properties > Linker > System
        - ensure that the SubSystem is set to Console (/SUBSYSTEM:CONSOLE)

### Okay, I think that is it, happy coding. 
