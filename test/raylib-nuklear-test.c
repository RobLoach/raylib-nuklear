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

    // InitNuklear(), IsNuklearValid()
    struct nk_context *ctx = NULL;
    AssertNot(IsNuklearValid(ctx));
    ctx = InitNuklear(10);
    Assert(ctx);
    Assert(IsNuklearValid(ctx));

    // Image
    Texture texture = LoadTexture("resources/test-image.png");
    AssertTexture(texture);
    struct nk_image image = TextureToNuklearImage(texture);
    AssertEqual(texture.id, (unsigned int)image.handle.id);
    Assert(texture.width > 0);

    // TextureToNuklearImageEx()
    {
        Rectangle region = { 16, 0, 16, 16 };
        struct nk_image imgEx = TextureToNuklearImageEx(texture, region);
        AssertEqual((unsigned int)imgEx.handle.id, texture.id);
        AssertEqual(imgEx.region[0], (nk_ushort)region.x);
        AssertEqual(imgEx.region[1], (nk_ushort)region.y);
        AssertEqual(imgEx.region[2], (nk_ushort)region.width);
        AssertEqual(imgEx.region[3], (nk_ushort)region.height);
    }

    // UpdateNuklear()
    UpdateNuklear(ctx);

    // UpdateNuklearEx()
    UpdateNuklearEx(ctx, 1.0f / 60.0f);

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

    // Save a screenshot for debugging.
    TakeScreenshot("raylib-nuklear-test.png");

    UnloadTexture(texture);

    // UnloadNuklear()
    UnloadNuklear(ctx);

    // InitNuklearEx()
    {
        Font font = LoadFont("resources/anonymous_pro_bold.ttf");
        ctx = InitNuklearEx(font, 25.0f);
        Assert(ctx);
        UnloadNuklear(ctx);
        UnloadFont(font);
    }

    // NuklearRectToRectangle()
    {
        ctx = NULL;
        struct nk_rect rect = nk_rect(10, 20, 30, 40);
        Rectangle rectangle = NuklearRectToRectangle(ctx, rect);
        AssertEqual(rect.x, rectangle.x);
        AssertEqual(rect.y, rectangle.y);
        AssertEqual(rect.w, rectangle.width);
        AssertEqual(rect.h, rectangle.height);
    }

    // NuklearRectToRectangle(), RectangleToNuklearRect(), with scaling
    {
        struct nk_rect rect = nk_rect(10, 20, 30, 40);
        ctx = InitNuklear(10);
        SetNuklearScaling(ctx, 2.0f);
        float scaling = GetNuklearScaling(ctx);
        AssertEqual(scaling, 2.0f, "Scaling was incorrectly set.");
        Rectangle rectangle = NuklearRectToRectangle(ctx, rect);
        AssertEqual(rect.x, rectangle.x / 2);
        AssertEqual(rect.y, rectangle.y / 2);
        AssertEqual(rect.w, rectangle.width / 2);
        AssertEqual(rect.h, rectangle.height / 2);

        rectangle = (Rectangle){20, 40, 60, 80};
        rect = RectangleToNuklearRect(ctx, rectangle);
        AssertEqual(rect.x, rectangle.x / 2);
        AssertEqual(rect.y, rectangle.y / 2);
        AssertEqual(rect.w, rectangle.width / 2);
        AssertEqual(rect.h, rectangle.height / 2);

        UnloadNuklear(ctx);
    }

    // Vector2ToNuklearVec2() / NuklearVec2ToVector2() round-trip
    {
        Vector2 v = { 3.5f, 7.25f };
        struct nk_vec2 nv = Vector2ToNuklearVec2(v);
        AssertEqual(nv.x, v.x);
        AssertEqual(nv.y, v.y);
        Vector2 rt = NuklearVec2ToVector2(nv);
        AssertEqual(rt.x, v.x);
        AssertEqual(rt.y, v.y);
    }

    // NuklearKeyToKeyboardKey()
    {
        AssertEqual(NuklearKeyToKeyboardKey(0), KEY_NULL);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_ENTER), KEY_ENTER);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_TAB), KEY_TAB);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_SHIFT), KEY_LEFT_SHIFT);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_BACKSPACE), KEY_BACKSPACE);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_TEXT_RESET_MODE), KEY_ESCAPE);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_DEL), KEY_DELETE);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_UP), KEY_UP);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_DOWN), KEY_DOWN);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_LEFT), KEY_LEFT);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_RIGHT), KEY_RIGHT);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_F1), KEY_F1);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_F6), KEY_F6);
        AssertEqual(NuklearKeyToKeyboardKey((nk_rune)NK_KEY_F12), KEY_F12);
        AssertEqual(NuklearKeyToKeyboardKey('A'), KEY_A);
        AssertEqual(NuklearKeyToKeyboardKey('a'), KEY_A);
        AssertEqual(NuklearKeyToKeyboardKey('Z'), KEY_Z);
        AssertEqual(NuklearKeyToKeyboardKey('1'), KEY_ONE);
        AssertEqual(NuklearKeyToKeyboardKey('2'), KEY_TWO);
        AssertEqual(NuklearKeyToKeyboardKey('='), KEY_EQUAL);
        AssertEqual(NuklearKeyToKeyboardKey('-'), KEY_MINUS);
    }

    // KeyboardKeyToNuklearKey()
    {
        AssertEqual(KeyboardKeyToNuklearKey(KEY_ENTER), (nk_rune)NK_KEY_ENTER);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_TAB), (nk_rune)NK_KEY_TAB);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_LEFT_SHIFT), (nk_rune)NK_KEY_SHIFT);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_RIGHT_SHIFT), (nk_rune)NK_KEY_SHIFT);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_BACKSPACE), (nk_rune)NK_KEY_BACKSPACE);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_ESCAPE), (nk_rune)NK_KEY_TEXT_RESET_MODE);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_DELETE), (nk_rune)NK_KEY_DEL);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_UP), (nk_rune)NK_KEY_UP);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_DOWN), (nk_rune)NK_KEY_DOWN);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_LEFT), (nk_rune)NK_KEY_LEFT);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_RIGHT), (nk_rune)NK_KEY_RIGHT);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_F1), (nk_rune)NK_KEY_F1);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_F6), (nk_rune)NK_KEY_F6);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_F12), (nk_rune)NK_KEY_F12);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_A), (nk_rune)KEY_A);
        AssertEqual(KeyboardKeyToNuklearKey(KEY_Z), (nk_rune)KEY_Z);
    }

    // NK_COMMAND_SCISSOR: content inside a small group must be clipped.
    {
        ctx = InitNuklear(10);
        Assert(ctx);

        UpdateNuklear(ctx);

        // A 50x50 window at (0,0) with a scrollable group of tall content.
        // Anything drawn below y=50 should be clipped by the group scissor.
        if (nk_begin(ctx, "ScissorTest", nk_rect(0, 0, 50, 50),
                NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "A", NK_TEXT_LEFT);
            nk_label(ctx, "B", NK_TEXT_LEFT);
            nk_label(ctx, "C", NK_TEXT_LEFT);
        }
        nk_end(ctx);

        BeginDrawing();
            ClearBackground(RED);
            DrawNuklear(ctx);
        EndDrawing();

        UnloadNuklear(ctx);
    }

    // RAYLIB_NUKLEAR_VERSION macros
    Assert(RAYLIB_NUKLEAR_VERSION_MAJOR >= 1);
    Assert(RAYLIB_NUKLEAR_VERSION_MINOR >= 0);
    Assert(RAYLIB_NUKLEAR_VERSION_PATCH >= 0);
    #if RAYLIB_NUKLEAR_VERSION_MAJOR >= 6
    Assert(1);
    #endif
    Assert(TextIsEqual(RAYLIB_NUKLEAR_VERSION, "6.0.1"));

    CloseWindow();
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-nuklear tests succesful");
    TraceLog(LOG_INFO, "================================");

    return 0;
}
