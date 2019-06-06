/*
 * Copyright (c) 2019 Marco Lizza (marco.lizza@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef __GL_PROGRAM_H__
#define __GL_PROGRAM_H__

#include <stdbool.h>

#include "common.h"

typedef struct _GL_Program_t {
    GLuint id;
} GL_Program_t;

typedef enum _GL_Program_Uniforms_t {
    GL_PROGRAM_UNIFORM_INT,
    GL_PROGRAM_UNIFORM_FLOAT,
    GL_PROGRAM_UNIFORM_VEC2,
    GL_PROGRAM_UNIFORM_VEC3,
    GL_PROGRAM_UNIFORM_VEC4,
    GL_PROGRAM_UNIFORM_VEC2I,
    GL_PROGRAM_UNIFORM_VEC3I,
    GL_PROGRAM_UNIFORM_VEC4I,
    GL_PROGRAM_UNIFORM_TEXTURE
} GL_Program_Uniforms_t;

extern bool GL_program_create(GL_Program_t *program, const char *vertex_shader, const char *fragment_shader);
extern void GL_program_delete(GL_Program_t *program);
extern void GL_program_send(const GL_Program_t *program, const char *id, GL_Program_Uniforms_t type, size_t count, const void *value);
extern void GL_program_use(const GL_Program_t *program);

#endif  /* __GL_PROGRAM_H__ */