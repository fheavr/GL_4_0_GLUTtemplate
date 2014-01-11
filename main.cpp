/* Copyright (c) Russell Gillette
 * December 2013
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef _WIN32
#  include "GL/glew.h"
#  include "GL/freeglut.h"
# elif __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderUtils.h"
#include "ControlState.h"
#include "WorldState.h"
#include "RenderState.h"
#include "Mesh.h"
#include "MeshUtils.h"

WorldState *w_state;
RenderState *r_state[2];
// NOTE: defined in ControlState.h 
// ControlState c_state;

Mesh *g_mesh;

// the display loop, where all of the code that actually
// changes what you see goes
void display()
{
    /* limit framerate to 60 fps */
    double curr = 0;
    if ((curr = c_state.queryTimer()) < 0.016666667) // curr < ~ 1/60
        return;

    // start counting over
    c_state.resetTimer();

    // Clear the buffer we will draw into.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Setup camera projection.
    w_state->projection = glm::perspective(50.0f, c_state.aspectRatio(), 0.1f, 40.0f);

    // Setup camera position/orientation.
    w_state->modelview = glm::lookAt(
        c_state.viewDepth * glm::vec3(0.0f, 2.5f, 10.0f), // eye
                            glm::vec3(0.0f, 0.0f,  0.0f), // centre
                            glm::vec3(0.0f, 1.0f,  0.0f)  // up
    );

    /* this would let you rotate the view with arrow keys
     *  c_state.updateView(
     *      c_state.deltaArrLR() * DEGREES_PER_SECOND * curr,
     *      0,
     *      c_state.deltaArrUD() * DEPTH_PER_SECOND * curr - c_state.mouseScroll + 0.00001 );
     */
    c_state.updateView(0, 0, -c_state.mouseScroll );
    c_state.mouseScroll = 0;

    w_state->modelview = glm::rotate(w_state->modelview, c_state.viewPhi, glm::vec3(1, 0, 0));
    w_state->modelview = glm::rotate(w_state->modelview, c_state.viewTheta, glm::vec3(0, 1, 0));


    /***********************************
     * Mesh Code
     ***********************************/
    w_state->useProgram(0);

    w_state->loadTransforms();
    w_state->loadMaterials();
    w_state->loadLights();
    g_mesh->drawMesh();

    glutSwapBuffers();
}

// setup
int main(int argc, char *argv[])
{
    GLenum err = 0;
    /*********************************************
     * GLUT SETUP
     *********************************************/
    glutInit(&argc, argv);

    /*********************************************
     * STATE SETUP (initialize gl context)
     *********************************************/
    // must be setup before glew so that a valid openGL
    // context exists (created with the window)

    w_state = new WorldState();
    c_state.init(*w_state);
    glutDisplayFunc(display);

    /*********************************************
     * GLEW SETUP
     *********************************************/
    err = glewInit();
    if (err != GLEW_OK)
    {
        fputs("Failed to initialize the GLEW library", stderr);
        exit(EXIT_FAILURE);
    }

    /*********************************************
     * STATE SETUP (construct render states)
     *********************************************/
    // must be setup after glew so that GL array
    // objects exist

    r_state[0] = new RenderState(3);
    r_state[1] = new RenderState(3);

    /*********************************************
     * SHADER SETUP
     *********************************************/
    // read default shaders from file
    GLuint shaderProgram[2] = {0};
    GLuint shaders[2] = {0};

    buildShader(GL_VERTEX_SHADER, "default.vs.glsl", shaders[0]);
    buildShader(GL_FRAGMENT_SHADER, "default.fs.glsl", shaders[1]);

    // create default shader program
    shaderProgram[0] = buildProgram(2, shaders);

    // bind shader program
    w_state->setProgram(0, shaderProgram[0]);
    w_state->useProgram(0);

    // setup the transform matrices and uniform variables
    w_state->loadTransforms();
    w_state->loadLights();
    w_state->loadMaterials();

    /*********************************************
     * LOAD MESH
     *********************************************/
    
    g_mesh = loadMeshFromFile(*r_state[0], "Mesh/arma.obj");

    /*********************************************
     * SET GL STATE
     *********************************************/ 
    glEnable(GL_DEPTH_TEST);

    /*********************************************
     * RENDER LOOP
     *********************************************/
    c_state.resetTimer();
    glutMainLoop();

    /*********************************************
     * CLEAN UP
     *********************************************/
    delete g_mesh;

    exit(EXIT_SUCCESS);
}