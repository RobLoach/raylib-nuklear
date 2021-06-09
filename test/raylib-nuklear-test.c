#include <assert.h>

#include "raylib.h"

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

int main(int argc, char *argv[]) {
    // Initialization
    SetTraceLogLevel(LOG_ALL);
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-nuklear-test");
    TraceLog(LOG_INFO, "================================");

    InitWindow(640, 480, "raylib-nuklear-tests");
    assert(IsWindowReady());

    // Make sure we're running in the correct directory.
    assert(argc > 0);
    const char* dir = GetDirectoryPath(argv[0]);
    assert(ChangeDirectory(dir));

    // InitNuklear()
    struct nk_context *ctx = InitNuklear(10);
    assert(ctx);

    // UpdateNuklear()
    UpdateNuklear(ctx);

    // Nuklear GUI Code
    // https://github.com/Immediate-Mode-UI/Nuklear/wiki/Window
    if (nk_begin(ctx, "Nuklear", nk_rect(100, 100, 220, 220),
                    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
        if (nk_button_label(ctx, "Button")) {
            /* event handling */
        }
    }
    nk_end(ctx);

    // Render
    BeginDrawing();
        ClearBackground(RAYWHITE);

        // DrawNuklear()
        DrawNuklear(ctx);

    EndDrawing();

    // UnloadNuklear()
    UnloadNuklear(ctx);

    // InitNuklearEx()
    Font font = LoadFont("resources/anonymous_pro_bold.ttf");
    ctx = InitNuklearEx(font, 25.0f);
    assert(ctx);
    UnloadNuklear(ctx);
    UnloadFont(font);

    // RectangleFromNuklear()
    struct nk_rect rect = nk_rect(10, 20, 30, 40);
    Rectangle rectangle = RectangleFromNuklear(rect);
    assert(rect.x == rectangle.x);
    assert(rect.y == rectangle.y);
    assert(rect.w == rectangle.width);
    assert(rect.h == rectangle.height);

    CloseWindow();
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-nuklear tests succesful");
    TraceLog(LOG_INFO, "================================");

    return 0;
}
