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

#ifndef OBJECTMESH_H
#define OBJECTMESH_H

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

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "VBuffer.h"
#include "IBuffer.h"
#include "RenderState.h"

using namespace std;

/**
 * Storage for all of the vertex attributes needed for rending a single mesh,
 * as well as any state data specific to that mesh. (such as object model transforms)
 */
class Mesh
{
public:
    glm::mat4 m_MV;  // object's model transformations

    Mesh(): m_num_elem(0), m_num_vbos(0), m_state(NULL), m_vbos(NULL)
    {}
    ~Mesh();
    // takes the number of vbos and then a state which has enough space allocated for those vbos
    // AND an ibo
    void init(int num_buffers, RenderState &state);
    void drawMesh();

    // these functions copy the the data to our internally managed memory, to modify
    // existing memory, use "GetData()
    void loadVBuffer(int buffer_num, int size, GLubyte *data, int data_offset, int num_attr, attrib_info *attr_info); // load data into the specified vertex buffer
    void loadIBuffer(int num_elem, int elem_size, int *data); // load data into the index buffer

    void analyzeAttr(int attrib_num, float *&attrib_ptr, int &stride);

    // a list of indexes is not always desired, and so can be ignored
    void analyzeMesh(int &num_elements, int **indices = NULL);

    // return a pointer to our internal copy of buffer i
    const float *getData(int i) const {return m_vbos[i].m_local_data;}
    float *getData(int i) {return m_vbos[i].m_local_data;}

    int    getNumVBO() {return m_num_vbos;}
    void   syncGPU(int base, int extent=0);     // copy the changes to our local data to the GPU

private:

    void resizeBuffer(int size);            // resize the internal store for the buffer

    GLsizei  m_num_elem;
    GLsizei  m_num_vbos;
    IBuffer  m_ibo;

    RenderState    *m_state;
    VBuffer        *m_vbos;
};

#endif