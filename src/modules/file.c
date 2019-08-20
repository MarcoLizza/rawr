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

#include "file.h"

#include "../core/luax.h"

#include "../environment.h"
#include "../file.h"
#include "../log.h"

#include <string.h>

typedef struct _File_Class_t {
} File_Class_t;

static int file_read(lua_State *L);

static const struct luaL_Reg file_functions[] = {
    { "read", file_read },
    { NULL, NULL }
};

static const luaX_Const file_constants[] = {
    { NULL }
};

int file_loader(lua_State *L)
{
    return luaX_newmodule(L, NULL, file_functions, file_constants, 0, LUAX_CLASS(File_Class_t));
}

static int file_read(lua_State *L)
{
    luaX_checkcall(L, "s");
    const char *file = lua_tostring(L, 1);
#ifdef __DEBUG_API_CALLS__
    Log_write(LOG_LEVELS_DEBUG, "File.read() -> %s", file);
#endif
    Environment_t *environment = (Environment_t *)luaX_getuserdata(L, "environment");

    char pathfile[PATH_FILE_MAX] = {};
    strcpy(pathfile, environment->base_path);
    strcat(pathfile, file + 2);

    const char *result = file_load_as_string(pathfile, "rt");
    Log_write(LOG_LEVELS_DEBUG, "<FILE> file '%s' loaded at %p", pathfile, result);

    lua_pushstring(L, result);

    return 1;
}
