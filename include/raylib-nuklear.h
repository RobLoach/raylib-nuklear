/**********************************************************************************************
*
*   raylib-nuklear - Nuklear for Raylib.
*
*   FEATURES:
*       - Use the nuklear immediate-mode graphical user interface in raylib.
*
*   DEPENDENCIES:
*       - raylib https://www.raylib.com/
*       - nuklear modified version at vendor/nuklear/nuklear.h
*
*   LICENSE: zlib/libpng
*
*   raylib-nuklear is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2020 Rob Loach (@RobLoach)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RAYLIB_NUKLEAR_H
#define RAYLIB_NUKLEAR_H

#ifndef NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_STANDARD_VARARGS
#endif

#include "./raylib-nuklear-nuklear.h"
#include "raylib.h"

NK_API struct nk_context* InitNuklear();
NK_API void UpdateNuklear(struct nk_context * ctx);
NK_API void DrawNuklear(struct nk_context * ctx);
NK_API Color ColorFromNuklear(struct nk_color color);
NK_API void UnloadNuklear(struct nk_context * ctx);
NK_API struct nk_color ColorToNuklear(Color color);
NK_API struct nk_colorf ColorToNuklearF(Color color);
NK_API struct Color ColorFromNuklear(struct nk_color color);
NK_API struct Color ColorFromNuklearF(struct nk_colorf color);
NK_API struct Rectangle RectangleFromNuklear(struct nk_rect rect);
NK_API struct nk_rect RectangleToNuklear(Rectangle rect);

#endif  // RAYLIB_NUKLEAR_H

#ifdef RAYLIB_NUKLEAR_IMPLEMENTATION
#ifndef RAYLIB_NUKLEAR_IMPLEMENTATION_ONCE
#define RAYLIB_NUKLEAR_IMPLEMENTATION_ONCE

#ifndef NK_ASSERT
#define NK_ASSERT(condition) if (!(condition)) { TraceLog(LOG_WARNING, "NUKLEAR: Failed assert \"%s\" (%s:%i)", #condition, "nuklear_raylib_nuklear.h", __LINE__); }
#endif  // NK_ASSERT

// TODO: Replace NK_INCLUDE_DEFAULT_ALLOCATOR with MemAlloc() and MemFree()
#ifndef NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#endif  // NK_INCLUDE_DEFAULT_ALLOCATOR

#ifndef NK_BOOL
#define NK_INCLUDE_STANDARD_BOOL
#define NK_BOOL bool
#endif  // NK_BOOL

#define NK_IMPLEMENTATION
#include "./raylib-nuklear-nuklear.h"

NK_API float
nk_raylib_font_get_text_width(nk_handle handle, float height, const char *text, int len)
{
    if (TextLength(text) == 0) {
        return 0;
    }

    return MeasureText(text, 10);

    // TODO: Support local user fonts.
    // struct nk_user_font* userFont = (struct nk_user_font*)handle.ptr;
    // Font* font = (Font*)userFont->userdata;
    // if (userFont == (void*)0) {
    //     return MeasureText(text, 10);
    // }

    // return MeasureTextEx(font, text, 10, 1.0f);
}

NK_API void
nk_raylib_clipboard_paste(nk_handle usr, struct nk_text_edit *edit)
{
    const char *text = GetClipboardText();
    if (text != (void*)0) {
        nk_textedit_paste(edit, text, TextLength(text));
    }
    (void)usr;
}

NK_API void
nk_raylib_clipboard_copy(nk_handle usr, const char *text, int len) {
    SetClipboardText(text);
}

NK_API struct nk_context*
InitNuklear()
{
    struct nk_context* ctx = (struct nk_context*) calloc(1, sizeof(struct nk_context));

    // Initialize the default font.
    struct Font* font = (struct Font*) calloc(1, sizeof(struct Font));
    struct nk_user_font* userFont = (struct nk_user_font*) calloc(1, sizeof(struct nk_user_font));
    userFont->userdata = nk_handle_ptr(font);
    userFont->height = 10;
    userFont->width = nk_raylib_font_get_text_width;

    // Create the nuklear environment.
    if (nk_init_default(ctx, userFont) == 0) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to initialize nuklear.");
        return (void*)0;
    }

    // Clipboard
    ctx->clip.copy = nk_raylib_clipboard_copy;
    ctx->clip.paste = nk_raylib_clipboard_paste;
    ctx->clip.userdata = nk_handle_ptr(0);

    return ctx;
}

NK_API Color
ColorFromNuklear(struct nk_color color)
{
    Color rc;
    rc.a = color.a;
    rc.r = color.r;
    rc.g = color.g;
    rc.b = color.b;
    return rc;
}

NK_API struct nk_color
ColorToNuklear(Color color)
{
    struct nk_color rc;
    rc.a = color.a;
    rc.r = color.r;
    rc.g = color.g;
    rc.b = color.b;
    return rc;
}

NK_API Color
ColorFromNuklearF(struct nk_colorf color)
{
    return ColorFromNuklear(nk_rgba_cf(color));
}

NK_API struct nk_colorf
ColorToNuklearF(Color color)
{
    return nk_color_cf(ColorToNuklear(color));
}

NK_API void
DrawNuklear(struct nk_context * ctx)
{
    const struct nk_command *cmd;
    bool scissor_mode = false;

    nk_foreach(cmd, ctx) {
        switch (cmd->type) {
            case NK_COMMAND_NOP: {
                break;
            }

            case NK_COMMAND_SCISSOR: {
                // TODO: Verify if NK_COMMAND_SCISSOR works.
                const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
                BeginScissorMode(s->x, s->y, s->w, s->h);
                scissor_mode = true;
            } break;

            case NK_COMMAND_LINE: {
                const struct nk_command_line *l = (const struct nk_command_line *)cmd;
                Color color = ColorFromNuklear(l->color);
                Vector2 startPos = (Vector2){l->begin.x, l->begin.y};
                Vector2 endPos = (Vector2){l->end.x, l->end.y};

                DrawLineEx(startPos, endPos, l->line_thickness, color);
            } break;

            case NK_COMMAND_CURVE: {
                const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
                Color color = ColorFromNuklear(q->color);

                Vector2 points[4];
                points[0] = (Vector2){q->begin.x, q->begin.y};
                points[1] = (Vector2){q->ctrl[0].x, q->ctrl[0].y};
                points[2] = (Vector2){q->ctrl[1].x, q->ctrl[1].y};
                points[3] = (Vector2){q->end.x, q->end.y};
                // TODO: Confirm NK_COMMAND_CURVE
                DrawLineStrip(points, 4, color);
            } break;

            case NK_COMMAND_RECT: {
                const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
                Color color = ColorFromNuklear(r->color);
                Rectangle rect = (Rectangle){r->x, r->y, r->w, r->h};
                if (r->rounding > 0) {
                    // TODO: Figure our appropriate roundness.
                    float roundness = (float)r->rounding / 20.0f;
                    DrawRectangleRoundedLines(rect, roundness, 1, r->line_thickness, color);
                }
                else {
                    DrawRectangleLinesEx(rect, r->line_thickness, color);
                }
            } break;

            case NK_COMMAND_RECT_FILLED: {
                const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
                Color color = ColorFromNuklear(r->color);
                Rectangle rect = (Rectangle){r->x, r->y, r->w, r->h};
                if (r->rounding > 0) {
                    // TODO: Figure our appropriate roundness.
                    float roundness = (float)r->rounding / 20.0f;
                    DrawRectangleRounded(rect, roundness, 1, color);
                }
                else {
                    DrawRectangleRec(rect, color);
                }
            } break;

            case NK_COMMAND_RECT_MULTI_COLOR: {
                const struct nk_command_rect_multi_color* rectangle = (const struct nk_command_rect_multi_color *)cmd;
                Rectangle position = (Rectangle){rectangle->x, rectangle->y, rectangle->w, rectangle->h};
                Color left = ColorFromNuklear(rectangle->left);
                Color top = ColorFromNuklear(rectangle->top);
                Color bottom = ColorFromNuklear(rectangle->bottom);
                Color right = ColorFromNuklear(rectangle->right);
                DrawRectangleGradientEx(position, left, bottom, right, top);
            } break;

            case NK_COMMAND_CIRCLE: {
                const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
                Color color = ColorFromNuklear(c->color);
                DrawEllipseLines(c->x + c->w / 2, c->y + c->h / 2, c->w / 2, c->h / 2, color);
            } break;

            case NK_COMMAND_CIRCLE_FILLED: {
                const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
                Color color = ColorFromNuklear(c->color);
                DrawEllipse(c->x + c->w / 2, c->y + c->h / 2, c->w / 2, c->h / 2, color);
            } break;

            case NK_COMMAND_ARC: {
                TraceLog(LOG_WARNING, "NUKLEAR: Untested implementation NK_COMMAND_ARC");
                const struct nk_command_arc *a = (const struct nk_command_arc *)cmd;
                Color color = ColorFromNuklear(a->color);

                // TODO: Fix NK_COMMAND_ARC
                Vector2 center = {a->cx, a->cy};
                float radius = a->r;
                int startAngle = a->a[0];
                int endAngle = a->a[1];
                int segments = 20; // How many segments do we need?
                DrawCircleSectorLines(center, radius, startAngle, endAngle, segments, color);
            } break;

            case NK_COMMAND_ARC_FILLED: {
                // TODO: Fix NK_COMMAND_ARC_FILLED
                TraceLog(LOG_WARNING, "NUKLEAR: Untested implementation NK_COMMAND_ARC_FILLED");
                const struct nk_command_arc *a = (const struct nk_command_arc *)cmd;
                Color color = ColorFromNuklear(a->color);

                Vector2 center = {a->cx, a->cy};
                float radius = a->r;
                int startAngle = a->a[0];
                int endAngle = a->a[1];
                int segments = 20; // How many segments do we need?
                DrawCircleSector(center, radius, startAngle, endAngle, segments, color);
            } break;

            case NK_COMMAND_TRIANGLE: {
                const struct nk_command_triangle *t = (const struct nk_command_triangle*)cmd;
                Color color = ColorFromNuklear(t->color);
                DrawTriangleLines((Vector2){t->b.x, t->b.y}, (Vector2){t->a.x, t->a.y}, (Vector2){t->c.x, t->c.y}, color);
            } break;

            case NK_COMMAND_TRIANGLE_FILLED: {
                const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled*)cmd;
                Color color = ColorFromNuklear(t->color);
                // TODO: Fix needing counter-clockwise order?
                DrawTriangle((Vector2){t->b.x, t->b.y}, (Vector2){t->a.x, t->a.y}, (Vector2){t->c.x, t->c.y}, color);
            } break;

            case NK_COMMAND_POLYGON: {
                const struct nk_command_polygon *p = (const struct nk_command_polygon*)cmd;
                Color color = ColorFromNuklear(p->color);
                Vector2 points[p->point_count];

                for (int i = 0; i < p->point_count; i++) {
                    points[i] = (Vector2){p->points[i].x, p->points[i].y};
                }

                DrawTriangleFan(points, p->point_count, color);
            } break;

            case NK_COMMAND_POLYGON_FILLED: {
                const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled*)cmd;
                Color color = ColorFromNuklear(p->color);
                Vector2 points[p->point_count];

                for (int i = 0; i < p->point_count; i++) {
                    points[i] = (Vector2){p->points[i].x, p->points[i].y};
                }

                DrawTriangleFan(points, p->point_count, color);
            } break;

            case NK_COMMAND_POLYLINE: {
                const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
                Color color = ColorFromNuklear(p->color);
                int i;
                Vector2 points[p->point_count];
                for (i = 0; i < p->point_count; i++) {
                    points[i] = (Vector2){p->points[i].x, p->points[i].y};
                }
                // TODO: See if Polyline is correct.
                DrawTriangleStrip(points, p->point_count, color);
            } break;

            case NK_COMMAND_TEXT: {
                const struct nk_command_text *text = (const struct nk_command_text*)cmd;
                Color color = ColorFromNuklear(text->foreground);
                // Font* font = (Font*)text->font->userdata.ptr;

                Vector2 position = {text->x, text->y};
                DrawText((const char*)text->string, text->x, text->y, 10, color);
            } break;

            case NK_COMMAND_IMAGE: {
                // TODO: Fix NK_COMMAND_IMAGE
                TraceLog(LOG_WARNING, "NUKLEAR: Broken implementation NK_COMMAND_IMAGE");
                const struct nk_command_image *i = (const struct nk_command_image *)cmd;
                struct Image image;
                image.data = i->img.handle.ptr;
                image.width = i->w;
                image.height = i->h;
                image.mipmaps = 1;
                image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

                Texture texture = LoadTextureFromImage(image);
                Rectangle source = {0, 0, image.width, image.height};
                Rectangle dest = {i->x, i->y, i->w, i->h};
                Vector2 origin = {0, 0};
                float rotation = 0;
                Color tint = ColorFromNuklear(i->col);
                DrawTexturePro(texture, source, dest, origin, rotation, tint);
                UnloadTexture(texture);
            } break;

            case NK_COMMAND_CUSTOM: {
                // TODO: NK_COMMAND_CUSTOM
                TraceLog(LOG_WARNING, "NUKLEAR: Missing implementation NK_COMMAND_CUSTOM");
            } break;

            default: {
                TraceLog(LOG_WARNING, "NUKLEAR: Missing implementation %i", cmd->type);
            } break;
        }
    }

    // Finish the scissor mode if it was in use.
    if (scissor_mode) {
        EndScissorMode();
    }

    nk_clear(ctx);
}

NK_API int
nk_raylib_translate_mouse_button(int button)
{
    switch (button) {
        case MOUSE_LEFT_BUTTON:
            return NK_BUTTON_LEFT;
        case MOUSE_RIGHT_BUTTON:
            return NK_BUTTON_RIGHT;
        case MOUSE_MIDDLE_BUTTON:
            return NK_BUTTON_MIDDLE;
    }

    return NK_BUTTON_MAX;
}

/**
 * Returns 1 when pressed, 2 when released, or 0 when no change.
 */
NK_API int nk_raylib_input_changed(int key) {
    if (IsKeyPressed(key)) {
        return 1;
    }
    else if (IsKeyReleased(key)) {
        return 0;
    }
    return -1;
}

NK_API void nk_raylib_input_keyboard(struct nk_context * ctx)
{
    int down;
    if ((down = nk_raylib_input_changed(KEY_LEFT_SHIFT)) >= 0) {
        nk_input_key(ctx, NK_KEY_SHIFT, down);
    }
    if ((down = nk_raylib_input_changed(KEY_RIGHT_SHIFT)) >= 0) {
        nk_input_key(ctx, NK_KEY_SHIFT, down);
    }
    if ((down = nk_raylib_input_changed(KEY_LEFT_CONTROL)) >= 0) {
        nk_input_key(ctx, NK_KEY_CTRL, down);
    }
    if ((down = nk_raylib_input_changed(KEY_RIGHT_CONTROL)) >= 0) {
        nk_input_key(ctx, NK_KEY_CTRL, down);
    }
    if ((down = nk_raylib_input_changed(KEY_DELETE)) >= 0) {
        nk_input_key(ctx, NK_KEY_DEL, down);
    }
    if ((down = nk_raylib_input_changed(KEY_ENTER)) >= 0) {
        nk_input_key(ctx, NK_KEY_ENTER, down);
    }
    if ((down = nk_raylib_input_changed(KEY_TAB)) >= 0) {
        nk_input_key(ctx, NK_KEY_TAB, down);
    }
    if ((down = nk_raylib_input_changed(KEY_BACKSPACE)) >= 0) {
        nk_input_key(ctx, NK_KEY_BACKSPACE, down);
    }
    if ((down = nk_raylib_input_changed(KEY_C)) >= 0) {
        nk_input_key(ctx, NK_KEY_COPY, (down == 1) && IsKeyDown(KEY_LEFT_CONTROL));
    }
    if ((down = nk_raylib_input_changed(KEY_X)) >= 0) {
        nk_input_key(ctx, NK_KEY_CUT, (down == 1) && IsKeyDown(KEY_LEFT_CONTROL));
    }
    if ((down = nk_raylib_input_changed(KEY_V)) >= 0) {
        nk_input_key(ctx, NK_KEY_PASTE, (down == 1) && IsKeyDown(KEY_LEFT_CONTROL));
    }
    if ((down = nk_raylib_input_changed(KEY_UP)) >= 0) {
        nk_input_key(ctx, NK_KEY_UP, down);
    }
    if ((down = nk_raylib_input_changed(KEY_DOWN)) >= 0) {
        nk_input_key(ctx, NK_KEY_DOWN, down);
    }
    if ((down = nk_raylib_input_changed(KEY_LEFT)) >= 0) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
        }
        else {
            nk_input_key(ctx, NK_KEY_LEFT, down);
        }
    }
    if ((down = nk_raylib_input_changed(KEY_RIGHT)) >= 0) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
        }
        else {
            nk_input_key(ctx, NK_KEY_RIGHT, down);
        }
    }

    // TODO: Add remaining keyboard actions.
    // NK_KEY_TEXT_INSERT_MODE
    // NK_KEY_TEXT_REPLACE_MODE
    // NK_KEY_TEXT_RESET_MODE
    // NK_KEY_TEXT_LINE_START
    // NK_KEY_TEXT_LINE_END
    // NK_KEY_TEXT_START
    // NK_KEY_TEXT_END
    // NK_KEY_TEXT_UNDO
    // NK_KEY_TEXT_REDO
    // NK_KEY_TEXT_SELECT_ALL
    // NK_KEY_TEXT_WORD_LEFT
    // NK_KEY_TEXT_WORD_RIGHT
    // NK_KEY_SCROLL_START
    // NK_KEY_SCROLL_END
    // NK_KEY_SCROLL_DOWN
    // NK_KEY_SCROLL_UP

    // TODO: Verify that this unicode keyboard input works.
    for (int i = 32; i < 126; i++) {
        if (IsKeyPressed(i)) {
            nk_input_unicode(ctx, i);
        }
    }
}

NK_API void
UpdateNuklear(struct nk_context * ctx)
{
    nk_input_begin(ctx);
    {
        // Mouse
        for (int button = MOUSE_LEFT_BUTTON; button <= MOUSE_MIDDLE_BUTTON; button++) {
        nk_input_button(ctx, nk_raylib_translate_mouse_button(button), GetMouseX(), GetMouseY(), IsMouseButtonDown(button));
        }
        nk_input_motion(ctx, GetMouseX(), GetMouseY());

        // Mouse Wheel
        int mouseWheel = GetMouseWheelMove();
        if (mouseWheel != 0) {
            struct nk_vec2 mouseWheelMove = (struct nk_vec2){0, mouseWheel};
            nk_input_scroll(ctx, mouseWheelMove);
        }

        // Keyboard
        nk_raylib_input_keyboard(ctx);
    }
    nk_input_end(ctx);
}

NK_API void
UnloadNuklear(struct nk_context * ctx)
{
    // Unload the given font.
    struct Font* font = (struct Font*)ctx->style.font->userdata.ptr;
    if (font != (void*)0) {
        UnloadFont(*font);
    }

    // Unload the nuklear context.
    nk_free(ctx);
}

NK_API struct
Rectangle RectangleFromNuklear(struct nk_rect rect)
{
    Rectangle output;
    output.x = rect.x;
    output.y = rect.y;
    output.width = rect.w;
    output.height = rect.h;
    return output;
}

NK_API struct
nk_rect RectangleToNuklear(Rectangle rect)
{
    return nk_rect(rect.x, rect.y, rect.width, rect.height);
}

#endif  // RAYLIB_NUKLEAR_IMPLEMENTATION_ONCE
#endif  // RAYLIB_NUKLEAR_IMPLEMENTATION
