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

#ifndef __GL_PRIMITIVE_H__
#define __GL_PRIMITIVE_H__

#include "common.h"
#include "context.h"

extern void GL_primitive_point(const GL_Context_t *context, GL_Point_t position, GL_Pixel_t index);
extern void GL_primitive_line(const GL_Context_t *context, GL_Point_t from, GL_Point_t to, GL_Pixel_t index);
extern void GL_primitive_hline(const GL_Context_t *context, GL_Point_t origin,  size_t width, GL_Pixel_t index);
extern void GL_primitive_vline(const GL_Context_t *context, GL_Point_t origin, size_t height, GL_Pixel_t index);

extern void GL_primitive_rectangle(const GL_Context_t *context, GL_Rectangle_t rectangle, GL_Pixel_t index);
extern void GL_primitive_filled_rectangle(const GL_Context_t *context, GL_Rectangle_t rectangle, GL_Pixel_t index);

extern void GL_primitive_circle(const GL_Context_t *context, GL_Point_t center, float radius, GL_Pixel_t index);
extern void GL_primitive_filled_circle(const GL_Context_t *context, GL_Point_t center, float radius, GL_Pixel_t index);

// extern void GL_primitive_ellipse(GL_Context_t *gl, const GL_Point_t center, const size_t xradius, const size_t yradius, const float angle, GL_Pixel_t color);
// extern void GL_primitive_filled_ellipse(GL_Context_t *gl, const GL_Point_t center, , const size_t xradius, const size_t yradius, const float angle, GL_Pixel_t color);

#endif  /* __GL_PRIMITIVE_H__ */