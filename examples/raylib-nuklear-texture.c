/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
/*
	This example shows how to use the image API from raylib nuklear.
	And then display it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

int main()
{
	InitWindow(1280, 720, "[raylib-nuklear] - Texture/Image");

	// Initialize the context
	struct nk_context* ctx = InitNuklear(20);

    // Scale up the Nuklear GUI
    SetNuklearScaling(ctx, 1.2f);

	// Load the nk_image
	struct nk_image img = LoadNuklearImage("resources/test-image.png");

	while (!WindowShouldClose())
	{
		// Input
		UpdateNuklear(ctx);

		// The window called "Image example" is opend
		if(nk_begin(ctx, "Image example", nk_rect(300, 100, img.w, img.h + 50), NK_WINDOW_MINIMIZABLE|NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
		{
			// Setup the layout
			nk_layout_row_static(ctx, img.h, img.w, 1);

			// Draw the image
			nk_image(ctx, img);
		}
		nk_end(ctx);

		// Draw the GUI
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawNuklear(ctx);
		EndDrawing();
	}

	// Unload the Nuklear image
	UnloadNuklearImage(img);

	CloseWindow();
	return 0;
}
