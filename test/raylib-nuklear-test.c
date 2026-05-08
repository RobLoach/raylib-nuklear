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

    // NkKeyToKeyboardKey()
    {
        AssertEqual(NkKeyToKeyboardKey(0), KEY_NULL);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_ENTER), KEY_ENTER);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_TAB), KEY_TAB);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_SHIFT), KEY_LEFT_SHIFT);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_BACKSPACE), KEY_BACKSPACE);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_TEXT_RESET_MODE), KEY_ESCAPE);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_DEL), KEY_DELETE);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_UP), KEY_UP);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_DOWN), KEY_DOWN);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_LEFT), KEY_LEFT);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_RIGHT), KEY_RIGHT);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_F1), KEY_F1);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_F6), KEY_F6);
        AssertEqual(NkKeyToKeyboardKey((nk_rune)NK_KEY_F12), KEY_F12);
        AssertEqual(NkKeyToKeyboardKey('A'), KEY_A);
        AssertEqual(NkKeyToKeyboardKey('a'), KEY_A);
        AssertEqual(NkKeyToKeyboardKey('Z'), KEY_Z);
        AssertEqual(NkKeyToKeyboardKey('!'), KEY_ONE);
        AssertEqual(NkKeyToKeyboardKey('@'), KEY_TWO);
        AssertEqual(NkKeyToKeyboardKey('+'), KEY_EQUAL);
        AssertEqual(NkKeyToKeyboardKey('_'), KEY_MINUS);
    }

    // KeyboardKeyToNkKey()
    {
        AssertEqual(KeyboardKeyToNkKey(KEY_ENTER), (nk_rune)NK_KEY_ENTER);
        AssertEqual(KeyboardKeyToNkKey(KEY_TAB), (nk_rune)NK_KEY_TAB);
        AssertEqual(KeyboardKeyToNkKey(KEY_LEFT_SHIFT), (nk_rune)NK_KEY_SHIFT);
        AssertEqual(KeyboardKeyToNkKey(KEY_RIGHT_SHIFT), (nk_rune)NK_KEY_SHIFT);
        AssertEqual(KeyboardKeyToNkKey(KEY_BACKSPACE), (nk_rune)NK_KEY_BACKSPACE);
        AssertEqual(KeyboardKeyToNkKey(KEY_ESCAPE), (nk_rune)NK_KEY_TEXT_RESET_MODE);
        AssertEqual(KeyboardKeyToNkKey(KEY_DELETE), (nk_rune)NK_KEY_DEL);
        AssertEqual(KeyboardKeyToNkKey(KEY_UP), (nk_rune)NK_KEY_UP);
        AssertEqual(KeyboardKeyToNkKey(KEY_DOWN), (nk_rune)NK_KEY_DOWN);
        AssertEqual(KeyboardKeyToNkKey(KEY_LEFT), (nk_rune)NK_KEY_LEFT);
        AssertEqual(KeyboardKeyToNkKey(KEY_RIGHT), (nk_rune)NK_KEY_RIGHT);
        AssertEqual(KeyboardKeyToNkKey(KEY_F1), (nk_rune)NK_KEY_F1);
        AssertEqual(KeyboardKeyToNkKey(KEY_F6), (nk_rune)NK_KEY_F6);
        AssertEqual(KeyboardKeyToNkKey(KEY_F12), (nk_rune)NK_KEY_F12);
        AssertEqual(KeyboardKeyToNkKey(KEY_A), (nk_rune)KEY_A);
        AssertEqual(KeyboardKeyToNkKey(KEY_Z), (nk_rune)KEY_Z);
    }

    CloseWindow();
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-nuklear tests succesful");
    TraceLog(LOG_INFO, "================================");

    return 0;
}
