# Nuklear Raylib

Use [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear) in [raylib](https://www.raylib.com/).

![nuklear_raylib_example screenshot](example/nuklear_raylib_example.png)

## Example

``` c
#define NK_IMPLEMENTATION
#define NK_RAYLIB_IMPLEMENTATION
#include "nuklear_raylib.h"

int main() {
    // Create the Nuklear Context
    struct nk_context *ctx = nk_raylib_init();

    while (!WindowShouldClose()) {
        // Input
        nk_raylib_input(ctx);

        // Create the Nuklear GUI
        // ...

        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);
            nk_raylib_render(ctx);
        EndDrawing();
    }

    // Finalize
    nk_raylib_free(ctx);
}
```

## Development

### Examples
```
mkdir build
cd build
cmake ..
make
```

### Nuklear Source

The copy of [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear) at [`include/nuklear_raylib_nuklear.h`](include/nuklear_raylib_nuklear.h) has been modified to avoid naming conflicts with raylib. To update it...

1. Grab the latest from https://github.com/Immediate-Mode-UI/Nuklear/blob/master/nuklear.h
2. Replace instances of `stbrp_` with `nuklear_stbrp_`
