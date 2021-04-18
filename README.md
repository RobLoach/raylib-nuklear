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

## Development

### Examples
```
mkdir build
cd build
cmake ..
make
./example/raylib-nuklear-example
```

### Nuklear Source

The copy of [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear) at [`include/raylib-nuklear-nuklear.h`](include/raylib-nuklear-nuklear.h) has been modified to avoid naming conflicts with raylib. To update it...

1. Grab the latest from https://github.com/Immediate-Mode-UI/Nuklear/blob/master/nuklear.h
2. Replace instances of `stbrp_` with `nuklear_stbrp_`
