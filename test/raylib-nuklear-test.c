#include "raylib.h"

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

#include "raylib-assert.h"

int main(int argc, char *argv[]) {
    // Initialization
    SetTraceLogLevel(LOG_ALL);
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-nuklear-test");
    TraceLog(LOG_INFO, "================================");

    InitWindow(640, 480, "raylib-nuklear-tests");
    Assert(IsWindowReady());

    // Make sure we're running in the correct directory.
    Assert(argc > 0);
    const char* dir = GetDirectoryPath(argv[0]);
    Assert(ChangeDirectory(dir));

    // InitNuklear()
    struct nk_context *ctx = InitNuklear(10);
    Assert(ctx);

    // Image
    struct nk_image image = LoadNuklearImage("resources/test-image.png");
    Assert(image.handle.ptr);
    Texture texture = TextureFromNuklear(image);
    Assert(texture.width > 0);

    // UpdateNuklear()
    UpdateNuklear(ctx);

    // Nuklear GUI Code
    // https://github.com/Immediate-Mode-UI/Nuklear/wiki/Window
    if (nk_begin(ctx, "Nuklear", nk_rect(50, 50, 400, 400),
                    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
        nk_button_label(ctx, "Button");
        nk_layout_row_static(ctx, 256, 256, 1);
        nk_image(ctx, image);
    }
    nk_end(ctx);

    // Render
    BeginDrawing();
        ClearBackground(RAYWHITE);

        // DrawNuklear()
        DrawNuklear(ctx);
    EndDrawing();

    // UnloadNuklearImage()
    UnloadNuklearImage(image);

    // UnloadNuklear()
    UnloadNuklear(ctx);

    // InitNuklearEx()
    Font font = LoadFont("resources/anonymous_pro_bold.ttf");
    ctx = InitNuklearEx(font, 25.0f);
    Assert(ctx);
    UnloadNuklear(ctx);
    UnloadFont(font);

    // RectangleFromNuklear()
    {
        ctx = NULL;
        struct nk_rect rect = nk_rect(10, 20, 30, 40);
        Rectangle rectangle = RectangleFromNuklear(ctx, rect);
        AssertEqual(rect.x, rectangle.x);
        AssertEqual(rect.y, rectangle.y);
        AssertEqual(rect.w, rectangle.width);
        AssertEqual(rect.h, rectangle.height);
    }

    // RectangleFromNuklear(), RectangleToNuklear(), with scaling
    {
        struct nk_rect rect = nk_rect(10, 20, 30, 40);
        ctx = InitNuklear(10);
        SetNuklearScaling(ctx, 2.0f);
        float scaling = GetNuklearScaling(ctx);
        AssertEqual(scaling, 2.0f, "Scaling was incorrectly set.");
        Rectangle rectangle = RectangleFromNuklear(ctx, rect);
        AssertEqual(rect.x, rectangle.x / 2);
        AssertEqual(rect.y, rectangle.y / 2);
        AssertEqual(rect.w, rectangle.width / 2);
        AssertEqual(rect.h, rectangle.height / 2);

        rectangle = (Rectangle){20, 40, 60, 80};
        rect = RectangleToNuklear(ctx, rectangle);
        AssertEqual(rect.x, rectangle.x / 2);
        AssertEqual(rect.y, rectangle.y / 2);
        AssertEqual(rect.w, rectangle.width / 2);
        AssertEqual(rect.h, rectangle.height / 2);

        UnloadNuklear(ctx);
    }

    CloseWindow();
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-nuklear tests succesful");
    TraceLog(LOG_INFO, "================================");

    return 0;
}
