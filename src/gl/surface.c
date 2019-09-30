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

#include "surface.h"

#include "gl.h"

#ifdef DEBUG
  #include <stb/stb_leakcheck.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../log.h"

bool GL_surface_load(GL_Surface_t *surface, const char *pathfile, GL_Surface_Callback_t callback, void *parameters)
{
    int width, height, components;
    void *data = stbi_load(pathfile, &width, &height, &components, STBI_rgb_alpha); //STBI_default);
    if (!data) {
        Log_write(LOG_LEVELS_ERROR, "<GL> can't load surface from '%s': %s", pathfile, stbi_failure_reason());
        return false;
    }
    GL_surface_create(surface, width, height);
    if (callback != NULL) {
        callback(parameters, surface, data);
    }
    stbi_image_free(data);

    Log_write(LOG_LEVELS_DEBUG, "<GL> surface '%s' loaded at #%p (%dx%d w/ %d)", pathfile, surface->data, width, height, components);

    return true;
}

bool GL_surface_decode(GL_Surface_t *surface, const void *buffer, size_t size, const GL_Surface_Callback_t callback, void *parameters)
{
    int width, height, components;
    void *data = stbi_load_from_memory(buffer, size, &width, &height, &components, STBI_rgb_alpha); //STBI_default);
    if (!data) {
        Log_write(LOG_LEVELS_ERROR, "<GL> can't decoder surface from #%p: %s", data, stbi_failure_reason());
        return false;
    }
    GL_surface_create(surface, width, height);
    if (callback != NULL) {
        callback(parameters, surface, data);
    }
    stbi_image_free(data);

    Log_write(LOG_LEVELS_DEBUG, "<GL> surface decoded at #%p (%dx%d w/ %d)", surface->data, width, height, components);

    return true;
}

bool GL_surface_create(GL_Surface_t *surface, size_t width, size_t height)
{
    void *data = malloc(width * height * sizeof(GL_Pixel_t));
    if (!data) {
        return false;
    }

    void **data_rows = malloc(height * sizeof(void *));
    if (!data_rows) {
        free(data);
        return false;
    }
    for (size_t i = 0; i < height; ++i) {
        data_rows[i] = (GL_Pixel_t *)data + (width * i);
    }

    Log_write(LOG_LEVELS_DEBUG, "<GL> surface created at #%p (%dx%d)", data, width, height);

    *surface = (GL_Surface_t){
            .width = width,
            .height = height,
            .data = data,
            .data_rows = data_rows,
            .data_size = width * height
        };

    return true;
}

void GL_surface_delete(GL_Surface_t *surface)
{
    free(surface->data);
    free(surface->data_rows);
    Log_write(LOG_LEVELS_DEBUG, "<GL> surface at #%p deleted", surface->data);

    *surface = (GL_Surface_t){};
}