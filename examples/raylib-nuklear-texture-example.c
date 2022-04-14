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
	struct nk_context* ctx = InitNuklear(16);
	// Load the nk_image
	struct nk_image img = LoadNuklearImage("resources/test-image.png");
	
	while(!WindowShouldClose())
	{
		// Input
		UpdateNuklear(ctx);
		
		// The window called "Image example" is opend
		if(nk_begin(ctx, "Image example", nk_rect(200, 200, 420, 320), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
		{
			// Setup the layout
			nk_layout_row_static(ctx, 256, 256, 1);
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
