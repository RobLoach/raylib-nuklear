/**********************************************************************************************
*
*   raylib-nuklear v6.0.1 - Nuklear GUI for Raylib.
*
*   FEATURES:
*       - Use the Nuklear immediate-mode graphical user interface in raylib.
*
*   DEPENDENCIES:
*       - raylib 6.0+ https://www.raylib.com/
*       - Nuklear https://github.com/Immediate-Mode-UI/Nuklear
*
*   LICENSE: zlib/libpng
*
*   raylib-nuklear is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2026 Rob Loach (@RobLoach)
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

#define RAYLIB_NUKLEAR_VERSION "6.0.1"
#define RAYLIB_NUKLEAR_VERSION_MAJOR 6
#define RAYLIB_NUKLEAR_VERSION_MINOR 0
#define RAYLIB_NUKLEAR_VERSION_PATCH 1

#include "raylib.h"

// Nuklear defines
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_STANDARD_BOOL
#define NK_INCLUDE_COMMAND_USERDATA
#define NK_KEYSTATE_BASED_INPUT
#ifdef RAYLIB_NUKLEAR_INCLUDE_DEFAULT_FONT
    #ifndef NK_INCLUDE_FONT_BAKING
        #define NK_INCLUDE_FONT_BAKING
    #endif
    #ifndef NK_INCLUDE_DEFAULT_FONT
        #define NK_INCLUDE_DEFAULT_FONT
    #endif
    // Raylib already provides stbrp (rect pack) symbols with external linkage
    #ifndef NK_NO_STB_RECT_PACK_IMPLEMENTATION
        #define NK_NO_STB_RECT_PACK_IMPLEMENTATION
    #endif
#endif

#ifndef NK_ASSERT
#ifdef NDEBUG
#define NK_ASSERT(condition) ((void)0)
#else
#define NK_ASSERT(condition) do { if (!(condition)) { TraceLog(LOG_ERROR, "NUKLEAR: Failed assert \"%s\" (%s:%i)", #condition, "nuklear.h", __LINE__); }} while (0)
#endif  // NDEBUG
#endif  // NK_ASSERT

#include "nuklear.h"

#ifdef __cplusplus
extern "C" {
#endif

NK_API struct nk_context* InitNuklear(int fontSize);                // Initialize the Nuklear GUI context using raylib's font
NK_API struct nk_context* InitNuklearEx(Font font, float fontSize); // Initialize the Nuklear GUI context, with a custom font
NK_API bool IsNuklearValid(struct nk_context* ctx);                 // Checks if Nuklear was loaded successfully.
NK_API Font LoadFontFromNuklear(int fontSize);                      // Loads the default Nuklear font
NK_API void UpdateNuklear(struct nk_context * ctx);                 // Update the input state and internal components for Nuklear
NK_API void UpdateNuklearEx(struct nk_context * ctx, float deltaTime); // Update the input state and internal components for Nuklear, with a custom frame time
NK_API void DrawNuklear(struct nk_context * ctx);                   // Render the Nuklear GUI on the screen
NK_API void UnloadNuklear(struct nk_context * ctx);                 // Deinitialize the Nuklear context
NK_API struct nk_color ColorToNuklearColor(Color color);                 // Convert a raylib Color to a Nuklear color object
NK_API struct nk_colorf ColorToNuklearColorF(Color color);               // Convert a raylib Color to a Nuklear floating color
NK_API Color NuklearColorToColor(struct nk_color color);               // Convert a Nuklear color to a raylib Color
NK_API Color NuklearColorFToColor(struct nk_colorf color);             // Convert a Nuklear floating color to a raylib Color
NK_API Rectangle NuklearRectToRectangle(struct nk_context * ctx, struct nk_rect rect); // Convert a Nuklear rectangle to a raylib Rectangle
NK_API struct nk_rect RectangleToNuklearRect(struct nk_context * ctx, Rectangle rect); // Convert a raylib Rectangle to a Nuklear Rectangle
NK_API struct nk_image TextureToNuklearImage(Texture texture);               // Get a Nuklear image from a Texture
NK_API struct nk_vec2 Vector2ToNuklearVec2(Vector2 vec);                     // Convert a raylib Vector2 to a Nuklear nk_vec2
NK_API Vector2 NuklearVec2ToVector2(struct nk_vec2 vec);                     // Convert a Nuklear nk_vec2 to a raylib Vector2
NK_API struct nk_image TextureToNuklearImageEx(Texture texture, Rectangle region); // Get a Nuklear image from a sub-region of a Texture
NK_API void SetNuklearScaling(struct nk_context * ctx, float scaling); // Sets the scaling for the given Nuklear context
NK_API float GetNuklearScaling(struct nk_context * ctx);            // Retrieves the scaling of the given Nuklear context
NK_API KeyboardKey NuklearKeyToKeyboardKey(nk_rune key);                 // Convert an nk_rune key binding to a raylib KeyboardKey
NK_API nk_rune KeyboardKeyToNuklearKey(KeyboardKey key);                 // Convert a raylib KeyboardKey to an nk_rune key binding

// Internal Nuklear functions
NK_API float nk_raylib_font_get_text_width(nk_handle handle, float height, const char *text, int len);
NK_API float nk_raylib_font_get_text_width_user_font(nk_handle handle, float height, const char *text, int len);
NK_API void nk_raylib_clipboard_paste(nk_handle usr, struct nk_text_edit *edit);
NK_API void nk_raylib_clipboard_copy(nk_handle usr, const char *text, int len);
NK_API void* nk_raylib_malloc(nk_handle unused, void *old, nk_size size);
NK_API void nk_raylib_mfree(nk_handle unused, void *ptr);
NK_API struct nk_context* InitNuklearContext(struct nk_user_font* userFont);
NK_API void nk_raylib_input_keyboard(struct nk_context * ctx);
NK_API void nk_raylib_input_mouse(struct nk_context * ctx);

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_NUKLEAR_H

#ifdef RAYLIB_NUKLEAR_IMPLEMENTATION
#ifndef RAYLIB_NUKLEAR_IMPLEMENTATION_ONCE
#define RAYLIB_NUKLEAR_IMPLEMENTATION_ONCE

#include <stddef.h> // NULL
#include <math.h> // cosf, sinf, sqrtf

// Math
#ifndef NK_COS
#define NK_COS cosf
#endif  // NK_COS
#ifndef NK_SIN
#define NK_SIN sinf
#endif  // NK_SIN
#ifndef NK_INV_SQRT
#define NK_INV_SQRT(value) (1.0f / sqrtf(value))
#endif  // NK_INV_SQRT

#define NK_IMPLEMENTATION
#include "nuklear.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RAYLIB_NUKLEAR_DEFAULT_FONTSIZE
/**
 * The default font size that is used when a font size is not provided.
 */
#define RAYLIB_NUKLEAR_DEFAULT_FONTSIZE 13
#endif  // RAYLIB_NUKLEAR_DEFAULT_FONTSIZE

#ifndef RAYLIB_NUKLEAR_FONT_SPACING_RATIO
/**
 * For user fonts, spacing is determined by the font size multiplied by RAYLIB_NUKLEAR_FONT_SPACING_RATIO.
 *
 * The default font uses raylib's internal spacing logic (fontSize / 10) instead.
 */
#define RAYLIB_NUKLEAR_FONT_SPACING_RATIO 0.01f
#endif // RAYLIB_NUKLEAR_FONT_SPACING_RATIO

#ifndef RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS
/**
 * The amount of segments used when drawing an arc.
 *
 * @see NK_COMMAND_ARC_FILLED
 */
#define RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS 20
#endif  // RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS

#ifndef RAYLIB_NUKLEAR_DOUBLE_CLICK_THRESHOLD
/**
 * The amount of time required to wait to determine a mouse click as a double click.
 *
 * @see nk_raylib_input_mouse()
 */
#define RAYLIB_NUKLEAR_DOUBLE_CLICK_THRESHOLD 0.3
#endif  // RAYLIB_NUKLEAR_DOUBLE_CLICK_THRESHOLD

/**
 * The user data that's leverages internally through Nuklear.
 */
typedef struct NuklearUserData {
    float scaling; // The scaling of the Nuklear user interface.
} NuklearUserData;

/**
 * Nuklear callback; Get the width of the given text.
 *
 * @internal
 */
NK_API float
nk_raylib_font_get_text_width(nk_handle handle, float height, const char *text, int len)
{
    NK_UNUSED(handle);

    if (len > 0) {
        Font font = GetFontDefault();
        if (font.texture.id == 0) return 0;

        // Match raylib's MeasureText() internal spacing behavior, but use MeasureTextEx()
        // to avoid int truncation that accumulates errors when measuring character by character.
        int defaultFontSize = 10;
        int fontSize = (int)height;
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        float spacing = (float)(fontSize / defaultFontSize);

        // Grab the text with the cropped length so that it only measures the desired string length.
        const char* subtext = TextSubtext(text, 0, len);

        // MeasureTextEx adds spacing between characters (len-1 times), but Nuklear expects
        // one spacing per character, so add one more spacing unit to compensate.
        return MeasureTextEx(font, subtext, (float)fontSize, spacing).x + spacing;
    }

    return 0;
}

/**
 * Nuklear callback; Get the width of the given text (userFont version)
 *
 * @internal
 */
NK_API float
nk_raylib_font_get_text_width_user_font(nk_handle handle, float height, const char *text, int len)
{
    if (len > 0) {
        // Grab the text with the cropped length so that it only measures the desired string length.
        const char* subtext = TextSubtext(text, 0, len);

        // Spacing is determined by the font size multiplied by RAYLIB_NUKLEAR_FONT_SPACING_RATIO.
        // Raylib only counts the spacing between characters, but Nuklear expects one spacing to be
        // counter for every character in the string:
        return MeasureTextEx(*(Font*)handle.ptr, subtext, height, height * RAYLIB_NUKLEAR_FONT_SPACING_RATIO).x + height * RAYLIB_NUKLEAR_FONT_SPACING_RATIO;
    }

    return 0;
}

/**
 * Nuklear callback; Paste the current clipboard.
 *
 * @internal
 */
NK_API void
nk_raylib_clipboard_paste(nk_handle usr, struct nk_text_edit *edit)
{
    const char *text = GetClipboardText();
    NK_UNUSED(usr);
    if (text != NULL) {
        nk_textedit_paste(edit, text, (int)TextLength(text));
    }
}

/**
 * Nuklear callback; Copy the given text.
 *
 * @internal
 */
NK_API void
nk_raylib_clipboard_copy(nk_handle usr, const char *text, int len)
{
    NK_UNUSED(usr);
    char* trimmedText = (char*)MemAlloc((unsigned int)(sizeof(char) * (size_t)(len + 1)));
    if(!trimmedText)
        return;
    nk_memcopy(trimmedText, text, (nk_size)len);
    trimmedText[len] = 0;
    SetClipboardText(trimmedText);
    MemFree(trimmedText);
}

/**
 * Nuklear callback; Allocate memory for Nuklear.
 *
 * @internal
 */
NK_API void*
nk_raylib_malloc(nk_handle unused, void *old, nk_size size)
{
    NK_UNUSED(unused);
    NK_UNUSED(old);
    return MemAlloc((unsigned int)size);
}

/**
 * Nuklear callback; Free memory for Nuklear.
 */
NK_API void
nk_raylib_mfree(nk_handle unused, void *ptr)
{
    NK_UNUSED(unused);
    MemFree(ptr);
}

/**
 * Initialize the Nuklear context for use with Raylib, with the given Nuklear user font.
 *
 * @param userFont The Nuklear user font to initialize the Nuklear context with.
 *
 * @internal
 */
NK_API struct nk_context*
InitNuklearContext(struct nk_user_font* userFont)
{
    struct nk_context* ctx = (struct nk_context*)MemAlloc(sizeof(struct nk_context));
    if (ctx == NULL) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to initialize nuklear memory");
        return NULL;
    }

    struct NuklearUserData* userData = (struct NuklearUserData*)MemAlloc(sizeof(struct NuklearUserData));
    if (userData == NULL) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to initialize nuklear user data");
        MemFree(ctx);
        return NULL;
    }

    // Allocator
    struct nk_allocator alloc;
    alloc.userdata = nk_handle_ptr(0);
    alloc.alloc = nk_raylib_malloc;
    alloc.free = nk_raylib_mfree;

    // Initialize the context.
    if (!nk_init(ctx, &alloc, userFont)) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to initialize nuklear");
        MemFree(ctx);
        MemFree(userData);
        return NULL;
    }

    // Clipboard
    ctx->clip.copy = nk_raylib_clipboard_copy;
    ctx->clip.paste = nk_raylib_clipboard_paste;
    ctx->clip.userdata = nk_handle_ptr(0);

    // Set the internal user data.
    userData->scaling = 1.0f;
    nk_handle userDataHandle;
    userDataHandle.id = 1;
    userDataHandle.ptr = (void*)userData;
    nk_set_user_data(ctx, userDataHandle);

    TraceLog(LOG_INFO, "NUKLEAR: Initialized GUI");

    return ctx;
}

/**
 * Initialize the Nuklear context for use with Raylib.
 *
 * @param fontSize The size of the font to use for GUI text. Use 0 to use the default font size of 10.
 *
 * @return The nuklear context, or NULL on error.
 */
NK_API struct nk_context*
InitNuklear(int fontSize)
{
    // User font.
    struct nk_user_font* userFont = (struct nk_user_font*)MemAlloc(sizeof(struct nk_user_font));
    if (userFont == NULL) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to allocate nuklear font");
        return NULL;
    }

    // Use the default font size if desired.
    if (fontSize <= 0) {
        fontSize = RAYLIB_NUKLEAR_DEFAULT_FONTSIZE;
    }

    userFont->height = (float)fontSize;
    userFont->width = nk_raylib_font_get_text_width;
    userFont->userdata = nk_handle_ptr(0);

    // Nuklear context.
    return InitNuklearContext(userFont);
}

/**
 * Initialize the Nuklear context for use with Raylib, with a supplied custom font.
 *
 * @param font The custom raylib font to use with Nuklear.
 * @param fontSize The desired size of the font. Use 0 to set the default size of 10.
 *
 * @return The nuklear context, or NULL on error.
 */
NK_API struct nk_context*
InitNuklearEx(Font font, float fontSize)
{
    // Copy the font to a new raylib font pointer.
    struct Font* newFont = (struct Font*)MemAlloc(sizeof(struct Font));
    if (newFont == NULL) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to allocate font");
        return NULL;
    }

    // Use the default font size if desired.
    if (fontSize <= 0.0f) {
        fontSize = (float)RAYLIB_NUKLEAR_DEFAULT_FONTSIZE;
    }
    newFont->baseSize = font.baseSize;
    newFont->glyphCount = font.glyphCount;
    newFont->glyphPadding = font.glyphPadding;
    newFont->glyphs = font.glyphs;
    newFont->recs = font.recs;
    newFont->texture = font.texture;

    // Create the nuklear user font.
    struct nk_user_font* userFont = (struct nk_user_font*)MemAlloc(sizeof(struct nk_user_font));
    if (userFont == NULL) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to allocate nuklear font");
        MemFree(newFont);
        return NULL;
    }
    userFont->userdata = nk_handle_ptr(newFont);
    userFont->height = fontSize;
    userFont->width = nk_raylib_font_get_text_width_user_font;

    // Nuklear context.
    return InitNuklearContext(userFont);
}

/**
 * Checks if Nuklear was loaded successfully.
 *
 * @param ctx The Nuklear context.
 * @return True when the Nuklear context was loaded successfully, false otherwise.
 */
NK_API bool IsNuklearValid(struct nk_context* ctx) {
    return ctx != NULL;
}

/**
 * Load the default Nuklear font. Requires `RAYLIB_NUKLEAR_INCLUDE_DEFAULT_FONT` to be defined.
 *
 * @param size The size of the font to load (optional). Provide 0 if you'd like to use the default size from Nuklear.
 *
 * @return The loaded font, or an empty font on error.
 *
 * @code
 * #define RAYLIB_NUKLEAR_INCLUDE_DEFAULT_FONT
 * #include "raylib-nuklear.h"
 * Font font = LoadFontFromNuklear(0);
 * @endcode
 */
NK_API Font LoadFontFromNuklear(int size) {
#ifndef RAYLIB_NUKLEAR_INCLUDE_DEFAULT_FONT
    (void)size;
    TraceLog(LOG_ERROR, "NUKLEAR: RAYLIB_NUKLEAR_INCLUDE_DEFAULT_FONT must be defined to use LoadFontFromNuklear()");
    return CLITERAL(Font) {0};
#else
    if (size <= 0) {
        size = RAYLIB_NUKLEAR_DEFAULT_FONTSIZE;
    }

    #ifndef RAYLIB_NUKLEAR_DEFAULT_FONT_GLYPHS
    /**
     * The amount of glyphs to load for the default font.
     */
    #define RAYLIB_NUKLEAR_DEFAULT_FONT_GLYPHS 95
    #endif

    // Decode base85 -> compressed binary -> raw TTF
    int compressed_size = (((int)nk_strlen(nk_proggy_clean_ttf_compressed_data_base85) + 4) / 5) * 4;
    unsigned char *compressed_data = (unsigned char*)MemAlloc((unsigned int)compressed_size);
    nk_decode_85(compressed_data, (const unsigned char*)nk_proggy_clean_ttf_compressed_data_base85);

    unsigned int ttf_size = nk_decompress_length(compressed_data);
    unsigned char *ttf_data = (unsigned char*)MemAlloc(ttf_size);
    nk_decompress(ttf_data, compressed_data, (unsigned int)compressed_size);
    MemFree(compressed_data);

    Font font = LoadFontFromMemory(".ttf", ttf_data, (int)ttf_size, size, NULL, RAYLIB_NUKLEAR_DEFAULT_FONT_GLYPHS);
    MemFree(ttf_data);
    return font;
#endif
}

/**
 * Convert the given Nuklear color to a raylib color.
 */
NK_API Color
NuklearColorToColor(struct nk_color color)
{
    Color rc;
    rc.a = color.a;
    rc.r = color.r;
    rc.g = color.g;
    rc.b = color.b;
    return rc;
}

/**
 * Convert the given raylib color to a Nuklear color.
 */
NK_API struct nk_color
ColorToNuklearColor(Color color)
{
    struct nk_color rc;
    rc.a = color.a;
    rc.r = color.r;
    rc.g = color.g;
    rc.b = color.b;
    return rc;
}

/**
 * Convert the given Nuklear float color to a raylib color.
 */
NK_API Color
NuklearColorFToColor(struct nk_colorf color)
{
    return NuklearColorToColor(nk_rgba_cf(color));
}

/**
 * Convert the given raylib color to a raylib float color.
 */
NK_API struct nk_colorf
ColorToNuklearColorF(Color color)
{
    return nk_color_cf(ColorToNuklearColor(color));
}

/**
 * Convert the given Nuklear image to a raylib Texture
 *
 * @internal
 */
static struct Texture
NuklearImageToTexture(struct nk_image img)
{
	Texture texture = {0};
	texture.id = (unsigned int)img.handle.id;
	texture.width = (int)img.w;
	texture.height = (int)img.h;
	texture.mipmaps = 1;
	return texture;
}

/**
 * Draw a filled polygon using Nuklear values.
 *
 * @see nk_rawfb_fill_polygon()
 * @see https://github.com/Immediate-Mode-UI/Nuklear/blob/master/demo/rawfb/nuklear_rawfb.h
 *
 * @internal
 * @todo Replace this software rendering with a GPU renderer
 */
static void raylib_nuklear_draw_polygon_fill(float scale, const struct nk_vec2i *pnts, int count, Color col) {
    int i = 0;
    #ifndef RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS
    #define RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS 64
    #endif
    int left = 10000, top = 10000, bottom = 0, right = 0;
    int nodes, nodeX[RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS], pixelX, pixelY, j, swap ;

    if (count == 0) return;
    if (count > RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS) {
        TraceLog(LOG_WARNING, "NUKLEAR: NK_COMMAND_POLYGON_FILLED point count %d exceeds max %d, truncating. Define RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS to increase.", count, RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS);
        count = RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS;
    }

    /* Build scaled copy of points */
    int scaled_x[RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS];
    int scaled_y[RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS];
    for (i = 0; i < count; i++) {
        scaled_x[i] = (int)(pnts[i].x * scale);
        scaled_y[i] = (int)(pnts[i].y * scale);
    }

    /* Get polygon dimensions */
    for (i = 0; i < count; i++) {
        if (left > scaled_x[i])
            left = scaled_x[i];
        if (right < scaled_x[i])
            right = scaled_x[i];
        if (top > scaled_y[i])
            top = scaled_y[i];
        if (bottom < scaled_y[i])
            bottom = scaled_y[i];
    } bottom++; right++;

    /* Polygon scanline algorithm released under public-domain by Darel Rex Finley, 2007 */
    /*  Loop through the rows of the image. */
    for (pixelY = top; pixelY < bottom; pixelY ++) {
        nodes = 0; /*  Build a list of nodes. */
        j = count - 1;
        for (i = 0; i < count; i++) {
            if (((scaled_y[i] < pixelY) && (scaled_y[j] >= pixelY)) ||
                ((scaled_y[j] < pixelY) && (scaled_y[i] >= pixelY))) {
                nodeX[nodes++]= (int)((float)scaled_x[i]
                     + ((float)pixelY - (float)scaled_y[i]) / ((float)scaled_y[j] - (float)scaled_y[i])
                     * ((float)scaled_x[j] - (float)scaled_x[i]));
            } j = i;
        }

        /*  Sort the nodes, via a simple “Bubble” sort. */
        i = 0;
        while (i < nodes - 1) {
            if (nodeX[i] > nodeX[i+1]) {
                swap = nodeX[i];
                nodeX[i] = nodeX[i+1];
                nodeX[i+1] = swap;
                if (i) i--;
            } else i++;
        }
        /*  Fill the pixels between node pairs. */
        for (i = 0; i < nodes; i += 2) {
            if (nodeX[i+0] >= right) break;
            if (nodeX[i+1] > left) {
                if (nodeX[i+0] < left) nodeX[i+0] = left ;
                if (nodeX[i+1] > right) nodeX[i+1] = right;
                for (pixelX = nodeX[i]; pixelX < nodeX[i + 1]; pixelX++) {
                    DrawPixel(pixelX, pixelY, col);
                }

            }
        }
    }
    #undef RAYLIB_NUKLEAR_POLYGON_FILL_MAX_POINTS
}

/**
 * Draw the given Nuklear context in raylib.
 *
 * @param ctx The nuklear context.
 */
NK_API void
DrawNuklear(struct nk_context * ctx)
{
    // Protect against drawing when there's nothing to draw.
    if (ctx == NULL) {
        return;
    }

    const struct nk_command *cmd;
    const float scale = GetNuklearScaling(ctx);

    nk_foreach(cmd, ctx) {
        switch (cmd->type) {
            case NK_COMMAND_NOP: {
                break;
            }

            case NK_COMMAND_SCISSOR: {
                const struct nk_command_scissor *s = (const struct nk_command_scissor*)cmd;
                if (s->x <= -8000) {
                    EndScissorMode();
                } else {
                    BeginScissorMode((int)(s->x * scale), (int)(s->y * scale), (int)(s->w * scale), (int)(s->h * scale));
                }
            } break;

            case NK_COMMAND_LINE: {
                const struct nk_command_line *l = (const struct nk_command_line *)cmd;
                Color color = NuklearColorToColor(l->color);
                Vector2 startPos = CLITERAL(Vector2) {(float)l->begin.x * scale, (float)l->begin.y * scale};
                Vector2 endPos = CLITERAL(Vector2) {(float)l->end.x * scale, (float)l->end.y * scale};
                DrawLineEx(startPos, endPos, l->line_thickness * scale, color);
            } break;

            case NK_COMMAND_CURVE: {
                const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
                Color color = NuklearColorToColor(q->color);
                Vector2 begin = CLITERAL(Vector2) {(float)q->begin.x * scale, (float)q->begin.y * scale};
                Vector2 controlPoint1 = CLITERAL(Vector2) {(float)q->ctrl[0].x * scale, (float)q->ctrl[0].y * scale};
                Vector2 controlPoint2 = CLITERAL(Vector2) {(float)q->ctrl[1].x * scale, (float)q->ctrl[1].y * scale};
                Vector2 end = CLITERAL(Vector2) {(float)q->end.x * scale, (float)q->end.y * scale};
                DrawSplineSegmentBezierCubic(begin, controlPoint1, controlPoint2, end, (float)q->line_thickness * scale, color);
            } break;

            case NK_COMMAND_RECT: {
                const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
                Color color = NuklearColorToColor(r->color);
                Rectangle rect = CLITERAL(Rectangle) {(float)r->x * scale, (float)r->y * scale, (float)r->w * scale, (float)r->h * scale};
                // We manually draw the rectangle with rounding instead of using DrawRectangleEx(), as we have more control over the corners.
                if (r->rounding > 0) {
                    float rad = NK_MIN((float)r->rounding * scale, NK_MIN(rect.width, rect.height) * 0.5f);
                    float thick = (float)r->line_thickness * scale;
                    // Straight edges
                    DrawRectangleRec((Rectangle){rect.x + rad, rect.y, rect.width - 2.0f * rad, thick}, color);
                    DrawRectangleRec((Rectangle){rect.x + rad, rect.y + rect.height - thick, rect.width - 2.0f * rad, thick}, color);
                    DrawRectangleRec((Rectangle){rect.x, rect.y + rad, thick, rect.height - 2.0f * rad}, color);
                    DrawRectangleRec((Rectangle){rect.x + rect.width - thick, rect.y + rad, thick, rect.height - 2.0f * rad}, color);
                    // Corner quarter-arc rings
                    DrawRing((Vector2){rect.x + rad, rect.y + rad}, NK_MAX(rad - thick, 0.0f), rad, 180.0f, 270.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                    DrawRing((Vector2){rect.x + rect.width - rad,  rect.y + rad}, NK_MAX(rad - thick, 0.0f), rad, 270.0f, 360.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                    DrawRing((Vector2){rect.x + rect.width - rad, rect.y + rect.height - rad}, NK_MAX(rad - thick, 0.0f), rad,   0.0f,  90.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                    DrawRing((Vector2){rect.x + rad, rect.y + rect.height - rad}, NK_MAX(rad - thick, 0.0f), rad,  90.0f, 180.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                }
                else {
                    DrawRectangleLinesEx(rect, r->line_thickness * scale, color);
                }
            } break;

            case NK_COMMAND_RECT_FILLED: {
                const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
                Color color = NuklearColorToColor(r->color);
                Rectangle rect = CLITERAL(Rectangle) {(float)r->x * scale, (float)r->y * scale, (float)r->w * scale, (float)r->h * scale};
                // We manually draw the rectangle with rounding instead of using DrawRectangleEx(), as we have more control over the corners.
                if (r->rounding > 0) {
                    float rad = NK_MIN((float)r->rounding * scale, NK_MIN(rect.width, rect.height) * 0.5f);
                    // Center vertical strip + top/bottom horizontal strips
                    DrawRectangleRec((Rectangle){rect.x, rect.y + rad, rect.width, rect.height - 2.0f * rad}, color);
                    DrawRectangleRec((Rectangle){rect.x + rad, rect.y, rect.width - 2.0f * rad, rad}, color);
                    DrawRectangleRec((Rectangle){rect.x + rad, rect.y + rect.height - rad, rect.width - 2.0f * rad, rad}, color);
                    // Corner quarter-circle arcs
                    DrawCircleSector((Vector2){rect.x + rad, rect.y + rad}, rad, 180.0f, 270.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                    DrawCircleSector((Vector2){rect.x + rect.width - rad, rect.y + rad}, rad, 270.0f, 360.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                    DrawCircleSector((Vector2){rect.x + rect.width - rad, rect.y + rect.height - rad}, rad, 0.0f, 90.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                    DrawCircleSector((Vector2){rect.x + rad, rect.y + rect.height - rad}, rad, 90.0f, 180.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                }
                else {
                    DrawRectangleRec(rect, color);
                }
            } break;

            case NK_COMMAND_RECT_MULTI_COLOR: {
                const struct nk_command_rect_multi_color* rectangle = (const struct nk_command_rect_multi_color *)cmd;
                Rectangle position = {(float)rectangle->x * scale, (float)rectangle->y * scale, (float)rectangle->w * scale, (float)rectangle->h * scale};
                // raylib's DrawRectangleGradientEx() takes corners in the order
                // (topLeft, bottomLeft, bottomRight, topRight). Nuklear maps its
                // rect_multi_color fields to corners as left=topLeft, top=topRight,
                // right=bottomRight, bottom=bottomLeft (see nk_draw_list_fill_rect_multi_color).
                DrawRectangleGradientEx(position,
                    NuklearColorToColor(rectangle->left),   // Top Left
                    NuklearColorToColor(rectangle->bottom), // Bottom Left
                    NuklearColorToColor(rectangle->right),  // Bottom Right
                    NuklearColorToColor(rectangle->top)     // Top Right
                );
            } break;

            case NK_COMMAND_CIRCLE: {
                const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
                Color color = NuklearColorToColor(c->color);
                float cx = (c->x + c->w / 2.0f) * scale;
                float cy = (c->y + c->h / 2.0f) * scale;
                float thick = (float)c->line_thickness * scale;
                if (c->w == c->h) {
                    float outerR = c->w * scale / 2.0f;
                    float innerR = NK_MAX(outerR - thick, 0.0f);
                    DrawRing((Vector2){cx, cy}, innerR, outerR, 0.0f, 360.0f, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                }
                else {
                    unsigned short size = (unsigned short)(thick * 2.0f);
                    for (unsigned short i = 0; i < size; i++) {
                        DrawEllipseLines((int)cx, (int)cy, c->w * scale / 2.0f - (float)i / 2.0f, c->h * scale / 2.0f - (float)i / 2.0f, color);
                    }
                }
            } break;

            case NK_COMMAND_CIRCLE_FILLED: {
                const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
                Color color = NuklearColorToColor(c->color);
                DrawEllipse((int)(c->x * scale + c->w * scale / 2.0f), (int)(c->y * scale + c->h * scale / 2.0f), (int)(c->w * scale / 2), (int)(c->h * scale / 2), color);
            } break;

            case NK_COMMAND_ARC: {
                const struct nk_command_arc *a = (const struct nk_command_arc*)cmd;
                Color color = NuklearColorToColor(a->color);
                Vector2 center = CLITERAL(Vector2) {(float)a->cx * scale, (float)a->cy * scale};
                float outerRadius = a->r * scale;
                float thick = a->line_thickness * scale;
                float innerRadius = outerRadius - thick;
                if (innerRadius < 0.0f) innerRadius = 0.0f;
                DrawRing(center, innerRadius, outerRadius, a->a[0] * RAD2DEG, a->a[1] * RAD2DEG, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
                Vector2 p0 = {center.x + outerRadius * cosf(a->a[0]), center.y + outerRadius * sinf(a->a[0])};
                Vector2 p1 = {center.x + outerRadius * cosf(a->a[1]), center.y + outerRadius * sinf(a->a[1])};
                DrawLineEx(center, p0, thick, color);
                DrawLineEx(center, p1, thick, color);
            } break;

            case NK_COMMAND_ARC_FILLED: {
                const struct nk_command_arc_filled *a = (const struct nk_command_arc_filled*)cmd;
                Color color = NuklearColorToColor(a->color);
                Vector2 center = CLITERAL(Vector2) {(float)a->cx * scale, (float)a->cy * scale};
                DrawRing(center, 0, a->r * scale, a->a[0] * RAD2DEG, a->a[1] * RAD2DEG, RAYLIB_NUKLEAR_DEFAULT_ARC_SEGMENTS, color);
            } break;

            case NK_COMMAND_TRIANGLE: {
                const struct nk_command_triangle *t = (const struct nk_command_triangle*)cmd;
                Color color = NuklearColorToColor(t->color);
                Vector2 point1 = CLITERAL(Vector2) {(float)t->b.x * scale, (float)t->b.y * scale};
                Vector2 point2 = CLITERAL(Vector2) {(float)t->a.x * scale, (float)t->a.y * scale};
                Vector2 point3 = CLITERAL(Vector2) {(float)t->c.x * scale, (float)t->c.y * scale};

                DrawLineEx(point1, point2, t->line_thickness * scale, color);
                DrawLineEx(point2, point3, t->line_thickness * scale, color);
                DrawLineEx(point3, point1, t->line_thickness * scale, color);
                // TODO: Add line thickness to DrawTriangleLines(), maybe via a DrawTriangleLinesEx()?
                // DrawTriangleLines(point1, point2, point3, color);
            } break;

            case NK_COMMAND_TRIANGLE_FILLED: {
                const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled*)cmd;
                Color color = NuklearColorToColor(t->color);
                Vector2 point1 = CLITERAL(Vector2) {(float)t->b.x * scale, (float)t->b.y * scale};
                Vector2 point2 = CLITERAL(Vector2) {(float)t->a.x * scale, (float)t->a.y * scale};
                Vector2 point3 = CLITERAL(Vector2) {(float)t->c.x * scale, (float)t->c.y * scale};
                DrawTriangle(point1, point2, point3, color);
            } break;

            case NK_COMMAND_POLYGON: {
                const struct nk_command_polygon *p = (const struct nk_command_polygon*)cmd;
                Color color = NuklearColorToColor(p->color);
                float thickness = (float)p->line_thickness * scale;
                for (unsigned short i = 0; i < p->point_count - 1; i++) {
                    Vector2 start = {(float)p->points[i].x * scale, (float)p->points[i].y * scale};
                    Vector2 end = {(float)p->points[i + 1].x * scale, (float)p->points[i + 1].y * scale};
                    DrawLineEx(start, end, thickness, color);
                }
                Vector2 last = {(float)p->points[p->point_count - 1].x * scale, (float)p->points[p->point_count - 1].y * scale};
                Vector2 first = {(float)p->points[0].x * scale, (float)p->points[0].y * scale};
                DrawLineEx(last, first, thickness, color);
            } break;

            case NK_COMMAND_POLYGON_FILLED: {
                const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled*)cmd;
                raylib_nuklear_draw_polygon_fill(scale, p->points, p->point_count, NuklearColorToColor(p->color));
            } break;

            case NK_COMMAND_POLYLINE: {
                const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
                Color color = NuklearColorToColor(p->color);
                for (unsigned short i = 0; i < p->point_count - 1; i++) {
                    Vector2 start = {(float)p->points[i].x * scale, (float)p->points[i].y * scale};
                    Vector2 end = {(float)p->points[i + 1].x * scale, (float)p->points[i + 1].y * scale};
                    DrawLineEx(start, end, p->line_thickness * scale, color);
                }
            } break;

            case NK_COMMAND_TEXT: {
                const struct nk_command_text *text = (const struct nk_command_text*)cmd;
                Color color = NuklearColorToColor(text->foreground);
                float fontSize = text->font->height * scale;
                Font* font = (Font*)text->font->userdata.ptr;
                if (font != NULL) {
                    Vector2 position = {(float)text->x * scale, (float)text->y * scale};
                    DrawTextEx(*font, (const char*)text->string, position, fontSize, fontSize * RAYLIB_NUKLEAR_FONT_SPACING_RATIO, color);
                }
                else {
                    DrawText((const char*)text->string, (int)(text->x * scale), (int)(text->y * scale), (int)fontSize, color);
                }
            } break;

            case NK_COMMAND_IMAGE: {
                const struct nk_command_image *i = (const struct nk_command_image *)cmd;
                Texture texture = NuklearImageToTexture(i->img);
                Rectangle source = CLITERAL(Rectangle) {(float)i->img.region[0], (float)i->img.region[1], (float)i->img.region[2], (float)i->img.region[3]};
                Rectangle dest = CLITERAL(Rectangle) {(float)i->x * scale, (float)i->y * scale, (float)i->w * scale, (float)i->h * scale};
                Vector2 origin = CLITERAL(Vector2) {0, 0};
                Color tint = NuklearColorToColor(i->col);
                DrawTexturePro(texture, source, dest, origin, 0, tint);
            } break;

            case NK_COMMAND_CUSTOM: {
                const struct nk_command_custom *custom = (const struct nk_command_custom *)cmd;
                custom->callback(NULL, (short)(custom->x * scale), (short)(custom->y * scale), (unsigned short)(custom->w * scale), (unsigned short)(custom->h * scale), custom->callback_data);
            } break;

            default: {
                TraceLog(LOG_WARNING, "NUKLEAR: Missing implementation %i", cmd->type);
            } break;
        }
    }

    nk_clear(ctx);
}

/**
 * @see nk_raylib_keyboard_checks
 */
struct nk_raylib_input_keyboard_check {
    int key;
    int input_key;
    bool needs_ctrl_cmd; // true = only active when ctrl or command is held
};

/**
 * The number of entries in the nk_raylib_keyboard_checks map.
 */
#define NK_RAYLIB_INPUT_KEYBOARD_CHECK_NUM 26

/**
 * A map determining the Nuklear to raylib keys.
 */
static const struct nk_raylib_input_keyboard_check nk_raylib_keyboard_checks[NK_RAYLIB_INPUT_KEYBOARD_CHECK_NUM] = {
    {KEY_DELETE,    NK_KEY_DEL,             false},
    {KEY_ENTER,     NK_KEY_ENTER,           false},
    {KEY_BACKSPACE, NK_KEY_BACKSPACE,       false},
    {KEY_C,         NK_KEY_COPY,            true},
    {KEY_V,         NK_KEY_PASTE,           true},
    {KEY_B,         NK_KEY_TEXT_LINE_START, true},
    {KEY_E,         NK_KEY_TEXT_LINE_END,   true},
    {KEY_Z,         NK_KEY_TEXT_UNDO,       true},
    {KEY_R,         NK_KEY_TEXT_REDO,       true},
    {KEY_A,         NK_KEY_TEXT_SELECT_ALL, true},
    {KEY_LEFT,      NK_KEY_TEXT_WORD_LEFT,  true},
    {KEY_RIGHT,     NK_KEY_TEXT_WORD_RIGHT, true},
    {KEY_ESCAPE,    NK_KEY_TEXT_RESET_MODE, false},
    {KEY_X,         NK_KEY_CUT,             true},
    {KEY_F1,        NK_KEY_F1,              false},
    {KEY_F2,        NK_KEY_F2,              false},
    {KEY_F3,        NK_KEY_F3,              false},
    {KEY_F4,        NK_KEY_F4,              false},
    {KEY_F5,        NK_KEY_F5,              false},
    {KEY_F6,        NK_KEY_F6,              false},
    {KEY_F7,        NK_KEY_F7,              false},
    {KEY_F8,        NK_KEY_F8,              false},
    {KEY_F9,        NK_KEY_F9,              false},
    {KEY_F10,       NK_KEY_F10,             false},
    {KEY_F11,       NK_KEY_F11,             false},
    {KEY_F12,       NK_KEY_F12,             false}
};

/**
 * Toggles keyboard insert mode.
 */
static bool nk_raylib_insert_mode = true;

/**
 * Update the Nuklear context for the keyboard input from raylib.
 *
 * @param ctx The nuklear context.
 *
 * @internal
 */
NK_API void
nk_raylib_input_keyboard(struct nk_context * ctx)
{
    bool control = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    bool command = IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);
    bool alt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
    bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    for (int i = 0; i < NK_RAYLIB_INPUT_KEYBOARD_CHECK_NUM; i++) {
        bool modifier_active = !nk_raylib_keyboard_checks[i].needs_ctrl_cmd || (control || command);
        nk_input_key(ctx, (enum nk_keys)nk_raylib_keyboard_checks[i].input_key,
            IsKeyDown(nk_raylib_keyboard_checks[i].key) && modifier_active);
    }

    nk_input_key(ctx, NK_KEY_ALT, alt);
    nk_input_key(ctx, NK_KEY_SHIFT, shift);
    nk_input_key(ctx, NK_KEY_CTRL, control || command);

    nk_input_key(ctx, NK_KEY_LEFT, IsKeyDown(KEY_LEFT) && !control && !command);
    nk_input_key(ctx, NK_KEY_RIGHT, IsKeyDown(KEY_RIGHT) && !control && !command);
    nk_input_key(ctx, NK_KEY_UP, IsKeyDown(KEY_UP));
    nk_input_key(ctx, NK_KEY_DOWN, IsKeyDown(KEY_DOWN));
    nk_input_key(ctx, NK_KEY_TEXT_START, IsKeyDown(KEY_HOME) && !control);
    nk_input_key(ctx, NK_KEY_TEXT_END, IsKeyDown(KEY_END) && !control);
    nk_input_key(ctx, NK_KEY_SCROLL_START, IsKeyDown(KEY_HOME) && control);
    nk_input_key(ctx, NK_KEY_SCROLL_END, IsKeyDown(KEY_END) && control);
    nk_input_key(ctx, NK_KEY_SCROLL_DOWN, IsKeyDown(KEY_PAGE_DOWN));
    nk_input_key(ctx, NK_KEY_SCROLL_UP, IsKeyDown(KEY_PAGE_UP));

    nk_input_key(ctx, NK_KEY_TAB, IsKeyDown(KEY_TAB));

    if (IsKeyPressed(KEY_INSERT)) nk_raylib_insert_mode = !nk_raylib_insert_mode;
    nk_input_key(ctx, NK_KEY_TEXT_INSERT_MODE, nk_raylib_insert_mode);
    nk_input_key(ctx, NK_KEY_TEXT_REPLACE_MODE, !nk_raylib_insert_mode);

    int code;
    while ((code = GetCharPressed()) != 0) {
        nk_input_unicode(ctx, (nk_rune)code);
    }
}

/**
 * Update the Nuklear context for the mouse input from raylib.
 *
 * @param ctx The nuklear context.
 *
 * @internal
 */
NK_API void
nk_raylib_input_mouse(struct nk_context * ctx)
{
    const float scale = GetNuklearScaling(ctx);
    const int mouseX = (int)((float)GetMouseX() / scale);
    const int mouseY = (int)((float)GetMouseY() / scale);

    nk_input_motion(ctx, mouseX, mouseY);
    nk_input_button(ctx, NK_BUTTON_LEFT, mouseX, mouseY, IsMouseButtonDown(MOUSE_LEFT_BUTTON));
    nk_input_button(ctx, NK_BUTTON_RIGHT, mouseX, mouseY, IsMouseButtonDown(MOUSE_RIGHT_BUTTON));
    nk_input_button(ctx, NK_BUTTON_MIDDLE, mouseX, mouseY, IsMouseButtonDown(MOUSE_MIDDLE_BUTTON));

    // Appears raylib mislabels the BACK/FORWARD mouse enums. X1/X2 are SIDE/EXTRA.
    nk_input_button(ctx, NK_BUTTON_X1, mouseX, mouseY, IsMouseButtonDown(MOUSE_BUTTON_SIDE));
    nk_input_button(ctx, NK_BUTTON_X2, mouseX, mouseY, IsMouseButtonDown(MOUSE_BUTTON_EXTRA));

    // Double Click
    {
        static double lastLeftPressTime = 0.0;
        static nk_bool doubleClicking = nk_false;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            double now = GetTime();
            doubleClicking = (now - lastLeftPressTime <= RAYLIB_NUKLEAR_DOUBLE_CLICK_THRESHOLD) ? nk_true : nk_false;
            lastLeftPressTime = now;
        }
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            doubleClicking = nk_false;
        }
        nk_input_button(ctx, NK_BUTTON_DOUBLE, mouseX, mouseY, doubleClicking);
    }

    // Mouse Wheel
    Vector2 mouseWheel = GetMouseWheelMoveV();
    if (mouseWheel.x != 0.0f || mouseWheel.y != 0.0f) {
        struct nk_vec2 scroll = {mouseWheel.x, mouseWheel.y};
        nk_input_scroll(ctx, scroll);
    }
}

/**
 * Update the Nuklear context for raylib's state.
 *
 * @param ctx The nuklear context to act upon.
 */
NK_API void
UpdateNuklear(struct nk_context * ctx)
{
    UpdateNuklearEx(ctx, GetFrameTime());
}

/**
 * Update the Nuklear context for raylib's state.
 *
 * @param ctx The nuklear context to act upon.
 * @param deltaTime Time in seconds since last frame.
 */
NK_API void
UpdateNuklearEx(struct nk_context * ctx, float deltaTime)
{
    // Update the time that has changed since last frame.
    ctx->delta_time_seconds = deltaTime;

    // Update the input state.
    nk_input_begin(ctx);
    {
        nk_raylib_input_mouse(ctx);
        nk_raylib_input_keyboard(ctx);
    }
    nk_input_end(ctx);
}

/**
 * Unload the given Nuklear context, along with all internal raylib textures.
 *
 * @param ctx The nuklear context.
 */
NK_API void
UnloadNuklear(struct nk_context * ctx)
{
    struct nk_user_font* userFont;

    // Skip unloading if it's not set.
    if (ctx == NULL) {
        return;
    }

    // Unload the font.
    userFont = (struct nk_user_font*)ctx->style.font;
    if (userFont != NULL) {
        // Clear the raylib Font object.
        void* fontPtr = userFont->userdata.ptr;
        if (fontPtr != NULL) {
            MemFree(fontPtr);
        }

        // Clear the user font.
        MemFree(userFont);
        ctx->style.font = NULL;
    }

    // Unload the custom user data.
    if (ctx->userdata.ptr != NULL) {
        MemFree(ctx->userdata.ptr);
    }

    // Unload the nuklear context.
    nk_free(ctx);
    MemFree(ctx);
    TraceLog(LOG_INFO, "NUKLEAR: Unloaded GUI");
}

/**
 * Convert the given Nuklear rectangle to a raylib Rectangle.
 */
NK_API Rectangle
NuklearRectToRectangle(struct nk_context* ctx, struct nk_rect rect)
{
    float scaling = GetNuklearScaling(ctx);
    Rectangle output;
    output.x = rect.x * scaling;
    output.y = rect.y * scaling;
    output.width = rect.w * scaling;
    output.height = rect.h * scaling;
    return output;
}

/**
 * Convert the given raylib Rectangle to a Nuklear rectangle.
 */
NK_API struct
nk_rect RectangleToNuklearRect(struct nk_context* ctx, Rectangle rect)
{
    float scaling = GetNuklearScaling(ctx);
    return nk_rect(rect.x / scaling, rect.y / scaling, rect.width / scaling, rect.height / scaling);
}

/**
 * Convert the given raylib texture to a Nuklear image
 */
NK_API struct nk_image
TextureToNuklearImageEx(Texture texture, Rectangle region)
{
	struct nk_image img = {0};
	img.handle.id = (int)texture.id;
	img.w = (nk_ushort)texture.width;
	img.h = (nk_ushort)texture.height;
	img.region[0] = (nk_ushort)region.x;
	img.region[1] = (nk_ushort)region.y;
	img.region[2] = (nk_ushort)region.width;
	img.region[3] = (nk_ushort)region.height;
	return img;
}

NK_API struct nk_vec2
Vector2ToNuklearVec2(Vector2 vec)
{
    return nk_vec2(vec.x, vec.y);
}

NK_API Vector2
NuklearVec2ToVector2(struct nk_vec2 vec)
{
    Vector2 v = { vec.x, vec.y };
    return v;
}

NK_API struct nk_image
TextureToNuklearImage(Texture texture)
{
	Rectangle region = { 0, 0, (float)texture.width, (float)texture.height };
	return TextureToNuklearImageEx(texture, region);
}

/**
 * Sets the scaling of the given Nuklear context.
 *
 * @param ctx The nuklear context.
 * @param scaling How much scale to apply to the graphical user interface.
 */
NK_API void
SetNuklearScaling(struct nk_context * ctx, float scaling)
{
    if (ctx == NULL) {
        return;
    }

    if (scaling <= 0.0f) {
        TraceLog(LOG_WARNING, "NUKLEAR: Cannot set scaling to be less than 0");
        return;
    }

    struct NuklearUserData* userData = (struct NuklearUserData*)ctx->userdata.ptr;
    if (userData != NULL) {
        userData->scaling = scaling;
    }
}

/**
 * Retrieves the scale value of the given Nuklear context.
 *
 * @return The scale value that had been set for the Nuklear context. 1.0f is the default scale value.
 */
NK_API float
GetNuklearScaling(struct nk_context * ctx)
{
    if (ctx == NULL) {
        return 1.0f;
    }

    struct NuklearUserData* userData = (struct NuklearUserData*)ctx->userdata.ptr;
    if (userData != NULL) {
        return userData->scaling;
    }

    return 1.0f;
}

/**
 * Convert an nk_rune key binding to a raylib KeyboardKey.
 */
NK_API KeyboardKey
NuklearKeyToKeyboardKey(nk_rune key)
{
    if (key == 0 || key == (nk_rune)NK_KEY_MAX) return KEY_NULL;
    if (key < (nk_rune)NK_KEY_MAX) {
        switch ((enum nk_keys)key) {
            case NK_KEY_ENTER:           return KEY_ENTER;
            case NK_KEY_TAB:             return KEY_TAB;
            case NK_KEY_SHIFT:           return KEY_LEFT_SHIFT; // KEY_RIGHT_SHIFT also maps to NK_KEY_SHIFT but is not reversible
            case NK_KEY_BACKSPACE:       return KEY_BACKSPACE;
            case NK_KEY_TEXT_RESET_MODE: return KEY_ESCAPE;
            case NK_KEY_DEL:             return KEY_DELETE;
            case NK_KEY_UP:              return KEY_UP;
            case NK_KEY_DOWN:            return KEY_DOWN;
            case NK_KEY_LEFT:            return KEY_LEFT;
            case NK_KEY_RIGHT:           return KEY_RIGHT;
            case NK_KEY_F1:              return KEY_F1;
            case NK_KEY_F2:              return KEY_F2;
            case NK_KEY_F3:              return KEY_F3;
            case NK_KEY_F4:              return KEY_F4;
            case NK_KEY_F5:              return KEY_F5;
            case NK_KEY_F6:              return KEY_F6;
            case NK_KEY_F7:              return KEY_F7;
            case NK_KEY_F8:              return KEY_F8;
            case NK_KEY_F9:              return KEY_F9;
            case NK_KEY_F10:             return KEY_F10;
            case NK_KEY_F11:             return KEY_F11;
            case NK_KEY_F12:             return KEY_F12;
            default:                     return KEY_NULL;
        }
    }
    if (key >= 'a' && key <= 'z') key -= 32;
    switch (key) {
        case '+': return KEY_EQUAL;
        case '_': return KEY_MINUS;
        case '?': return KEY_SLASH;
        case ':': return KEY_SEMICOLON;
        case '"': return KEY_APOSTROPHE;
        case '<': return KEY_COMMA;
        case '>': return KEY_PERIOD;
        case '{': return KEY_LEFT_BRACKET;
        case '}': return KEY_RIGHT_BRACKET;
        case '|': return KEY_BACKSLASH;
        case '~': return KEY_GRAVE;
        case '!': return KEY_ONE;
        case '@': return KEY_TWO;
        case '#': return KEY_THREE;
        case '$': return KEY_FOUR;
        case '%': return KEY_FIVE;
        case '^': return KEY_SIX;
        case '&': return KEY_SEVEN;
        case '*': return KEY_EIGHT;
        case '(': return KEY_NINE;
        case ')': return KEY_ZERO;
    }
    return (KeyboardKey)(int)key;
}

/**
 * Convert a raylib KeyboardKey to an nk_rune key binding.
 */
NK_API nk_rune
KeyboardKeyToNuklearKey(KeyboardKey key)
{
    switch (key) {
        case KEY_ENTER:         return (nk_rune)NK_KEY_ENTER;
        case KEY_TAB:           return (nk_rune)NK_KEY_TAB;
        case KEY_LEFT_SHIFT:
        case KEY_RIGHT_SHIFT:   return (nk_rune)NK_KEY_SHIFT; // Both map to NK_KEY_SHIFT; NuklearKeyToKeyboardKey returns only KEY_LEFT_SHIFT
        case KEY_BACKSPACE:     return (nk_rune)NK_KEY_BACKSPACE;
        case KEY_ESCAPE:        return (nk_rune)NK_KEY_TEXT_RESET_MODE;
        case KEY_DELETE:        return (nk_rune)NK_KEY_DEL;
        case KEY_UP:            return (nk_rune)NK_KEY_UP;
        case KEY_DOWN:          return (nk_rune)NK_KEY_DOWN;
        case KEY_LEFT:          return (nk_rune)NK_KEY_LEFT;
        case KEY_RIGHT:         return (nk_rune)NK_KEY_RIGHT;
        case KEY_F1:            return (nk_rune)NK_KEY_F1;
        case KEY_F2:            return (nk_rune)NK_KEY_F2;
        case KEY_F3:            return (nk_rune)NK_KEY_F3;
        case KEY_F4:            return (nk_rune)NK_KEY_F4;
        case KEY_F5:            return (nk_rune)NK_KEY_F5;
        case KEY_F6:            return (nk_rune)NK_KEY_F6;
        case KEY_F7:            return (nk_rune)NK_KEY_F7;
        case KEY_F8:            return (nk_rune)NK_KEY_F8;
        case KEY_F9:            return (nk_rune)NK_KEY_F9;
        case KEY_F10:           return (nk_rune)NK_KEY_F10;
        case KEY_F11:           return (nk_rune)NK_KEY_F11;
        case KEY_F12:           return (nk_rune)NK_KEY_F12;
        default:                return (nk_rune)key;
    }
}

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_NUKLEAR_IMPLEMENTATION_ONCE
#endif  // RAYLIB_NUKLEAR_IMPLEMENTATION
