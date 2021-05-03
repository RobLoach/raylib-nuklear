# raylib-nuklear

Use the [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear) immediate mode cross-platform GUI library in [raylib](https://www.raylib.com/).

![raylib-nuklear-example screenshot](examples/raylib-nuklear-example.png)

## Example

``` c
#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

int main() {
    InitWindow(640, 480, "raylib-nuklear example");

    // Create the Nuklear Context
    struct nk_context *ctx = InitNuklear();

    while (!WindowShouldClose()) {
        // Update the Nuklear context, along with input
        UpdateNuklear(ctx);

        // Add your own Nuklear GUI
        // See the Nuklear Wiki for examples:
        // https://github.com/Immediate-Mode-UI/Nuklear/wiki/Window
        // ...

        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Render the Nuklear GUI
            DrawNuklear(ctx);

        EndDrawing();
    }

    // De-initialize the Nuklear GUI
    UnloadNuklear(ctx);

    CloseWindow();
    return 0;
}
```

## API

``` c
NK_API struct nk_context* InitNuklear();
NK_API void UpdateNuklear(struct nk_context * ctx);
NK_API void DrawNuklear(struct nk_context * ctx);
NK_API void UnloadNuklear(struct nk_context * ctx);
NK_API Color ColorFromNuklear(struct nk_color color);
NK_API struct nk_color ColorToNuklear(Color color);
NK_API struct nk_colorf ColorToNuklearF(Color color);
NK_API struct Color ColorFromNuklear(struct nk_color color);
NK_API struct Color ColorFromNuklearF(struct nk_colorf color);
NK_API struct Rectangle RectangleFromNuklear(struct nk_rect rect);
NK_API struct nk_rect RectangleToNuklear(Rectangle rect);
```

## Development

### Examples
```
mkdir build
cd build
cmake ..
make
./example/raylib-nuklear-example
```

## License

raylib-nuklear is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
