#include "display.h"

#include "log.h"

#include <memory.h>

#define UNCAPPED_FPS        0

bool Display_initialize(Display_t *display, const Display_Configuration_t *configuration, const char *title)
{
    display->configuration = *configuration;

    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(0, 0, title); // Initially fit the screen-size automatically.

    int display_width = GetScreenWidth();
    int display_height = GetScreenHeight();

    display->window_width = configuration->width;
    display->window_height = configuration->height;
    display->window_scale = 1;

    if (configuration->autofit) {
        Log_write(LOG_LEVELS_DEBUG, "Display size is %d x %d", display_width, display_height);

        for (int s = 1; ; ++s) {
            int w = configuration->width * s;
            int h = configuration->height * s;
            if ((w > display_width) || (h > display_height)) {
                break;
            }
            display->window_width = w;
            display->window_height = h;
            display->window_scale = s;
        }
    }

    Log_write(LOG_LEVELS_DEBUG, "Window size is %d x %d (%dx)", display->window_width, display->window_height,
        display->window_scale);

    int x = (display_width - display->window_width) / 2;
    int y = (display_height - display->window_height) / 2;

    if (configuration->hide_cursor) {
        HideCursor();
    }

    SetWindowPosition(x, y);
    SetWindowSize(display->window_width, display->window_height);
    UnhideWindow();
    if (configuration->fullscreen) {
        ToggleFullscreen();
    }

    SetTargetFPS(UNCAPPED_FPS);

    display->offscreen = LoadRenderTexture(configuration->width, configuration->height);
    SetTextureFilter(display->offscreen.texture, FILTER_POINT); // Nearest-neighbour scaling.

    display->offscreen_source = (Rectangle){ 0.0f, 0.0f, (float)display->offscreen.texture.width, (float)-display->offscreen.texture.height };
    display->offscreen_destination = (Rectangle){ 0.0f, 0.0f, (float)display->window_width, (float)display->window_height };
    display->offscreen_origin = (Vector2){ 0.0f, 0.0f };

    return true;
}

bool Display_shouldClose(Display_t *display)
{
    return WindowShouldClose();
}

void Display_renderBegin(Display_t *display, void callback(void))
{
    BeginTextureMode(display->offscreen);
        ClearBackground(BLACK);
        if (callback) {
            callback();
        }
        // BeginShaderMode()
}

void Display_renderEnd(Display_t *display, void callback(void), const double fps, const double delta_time)
{
    EndTextureMode();
    BeginDrawing();
        if (callback) {
            callback();
        }
        DrawTexturePro(display->offscreen.texture, display->offscreen_source, display->offscreen_destination,
            display->offscreen_origin, 0.0f, WHITE);

        if (display->configuration.display_fps) {
            DrawText(FormatText("[ %.0f fps | %.3fs ]", fps, delta_time), 0, 0, 10, (Color){ 255, 255, 255, 128 });
        }
    EndDrawing();
}

void Display_terminate(Display_t *display)
{
    UnloadRenderTexture(display->offscreen);
    CloseWindow();
}