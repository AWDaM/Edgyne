# Edgyne
Edgyne is a basic 3D game engine created as a project for the university. The engine is done mimicking the Unity style, our version of it has a shader pipeline included to it.

### Authors
The authors of this engine are two students at the [UPC of barcelona](https://www.citm.upc.edu), studying the degree in engineering of development and design of video games

[Jaume Surís](https://github.com/Farmak09)

[Lorién Portella](https://github.com/Witiza)

### Engine Funcionality

- Importing FBX: To import a 3D model, drag and drop the file to the engine window. If everything is correct, the engine will generate a .edgymodel file in the Assets\Models folder which can be accessed from the Assets window.
- Updating the quadtree: The quadtree of the engine will be updated every time a GameObject is created or its switched to Static.
- Saving the Scene: By clicking in SaveScene, you save the currently open scene without changing the name. If you write a name in save scene as and press enter, the engine will check if the name already exists, and if not, will save the scene as a new one.

### Shader Funcionality

In video games, shaders are used as a way to add visual effects that would otherwise require extesive texture manipulation by creating a filter written as independent code.
This shader can be separated into different categories depending on their main functionality:
- A **vertex shader** runs once for each of the vertexes of the mesh it's attached to and it's usually made to hold the vertex's traits, like its position and color.
- A **pixel or fragment shader** is applied individually to each pixel in order to get more precise effects by modifying its traits without affecting the original mesh.
- A **geometric shader** is a more specific kind of shader used in order to modify a mesh's geometry by adding new primitives to it.
- A **tessellation shader** used to change the polygon of a mesh depending usually on the distance to the camera in order to have better quality the closer it gets.

### Display

![Alt Text](https://gyazo.com/dfd3213d3c32ce884529a9b1ec89f02d.gif)

![Alt Text](https://gyazo.com/a32a1c9cb88b683a2d2884938b297430.gif)

### Repository

- If you want to download new releases or just check the source code, you can go to our repository:
https://github.com/AWDaM/Edgyne

### MIT License

Copyright (c) 2018 Jaume Surís Valhondo, Lorién Portella López

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
