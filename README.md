### Introduction 
A project to learn basic OpenGL.

### Getting Started
Getting project setup:
1.	Get an IDE
	- Get Microsoft Visual Studio Community [download](https://www.visualstudio.com/vs/community/)
2.	Software dependencies
	- I'm using [vcpkg](https://github.com/Microsoft/vcpkg).  This a package manager for C\C++ libraries from Microsoft.
	- for this project I'm using GLFW, GLAD, GLM and SOIL2.
    - Installed the files above (replace libfile with names above) <br> > ./vcpkg install "libfile":x64-windows-static
    - All installed libraries are immediately ready to be #include'd and used in your project without additional configuration. <br> > .\vcpkg\vcpkg integrate install

3.  References
	- [Learn OpenGL](https://learnopengl.com/)
	- [OpenGL](https://www.opengl.org/)
	- [GLAD](https://github.com/Dav1dde/glad) Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs
    - [GLM](https://glm.g-truc.net/0.9.9/index.html) OpenGL Mathematics (GLM) header only C++ mathematics library for graphics software.
    - [SOIL2](https://bitbucket.org/SpartanJ/soil2/src/default/) Tiny C library used primarily for uploading textures into OpenGL.
	- [docs.GL](http://docs.gl/) OpenGL reference

### Project Properties:

**NOTE:** make sure All Configurations is selected
1. Configuration Properties > General
	- set the output directory to ./../bin/ to provide common place for the executable
2. Configuration Properties > Linker > general
    - change the Output File from ProjectDir to OutDir 
2. Configuration Properties > Linker > Input
	a. add the Additional Dependencies (glfw3.lib; glew.lib; soil2.lib opengl32.lib) keep the defaults already there.
    b. **may need to do release and debug configurations separately additional dependencies.**
3. Set an entry point if needed for the program:
    a. Configuration Properties > Linker > System
        - ensure that the SubSystem is set to Console (/SUBSYSTEM:CONSOLE)

### Okay, I think that is it, happy coding. 
