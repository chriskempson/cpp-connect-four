/*
 * Connect Four
 * MIT Licence, Copyright 2017 Chris Kempson (chriskempson.com)
 */

#include "Title_state.h"
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"
#include "Play_state.h"

void Title_state::init()
{
	Resource_manager::load_image("title_background");

	// Load the text images with an offset so the don't appear on the 
	// screen when first rendered
	Resource_manager::load_image("title_f")->set_position_hidden();
	Resource_manager::load_image("title_o")->set_position_hidden();
	Resource_manager::load_image("title_u")->set_position_hidden();
	Resource_manager::load_image("title_r")->set_position_hidden();

	Resource_manager::load_sound("title")->play();
	Resource_manager::load_sound("ck");

	// Y axis values for animation of "Four" text, the initial numbers here
	// determine the order in which they fall. Each character falls at the 
	// same speed but will start at different heights.
	title_f_y = 31;
	title_o_y = 11;
	title_u_y = 21;
	title_r_y = 1;

	// The drop speed of the "Four" text
	drop_speed = 2;
}

void Title_state::clean_up()
{
	Resource_manager::unload_image("title_background");
	Resource_manager::unload_image("title_f");
	Resource_manager::unload_image("title_o");
	Resource_manager::unload_image("title_u");
	Resource_manager::unload_image("title_r");

	Resource_manager::unload_sound("title");
	Resource_manager::unload_sound("ck");
}

void Title_state::handle_events(SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {

		// Get mouse coords
		int mouse_x, mouse_y;
		Game::get_mouse_position(&mouse_x, &mouse_y);
		
		// Check if click falls within box coords
		if (within_box(mouse_x, mouse_y, 24, 87, 64, 5)) {
			Resource_manager::get_sound("ck")->play();

            // Wait for the sound play before moving on
            SDL_Delay(1000);

            State_manager::set_state(new Play_state());
		}
		else {
			State_manager::set_state(new Play_state());
		}
	}
}

void Title_state::update()
{
	animate_logo();
}

void Title_state::render()
{
	Resource_manager::get_image("title_f")->render();
	Resource_manager::get_image("title_o")->render();
	Resource_manager::get_image("title_u")->render();
	Resource_manager::get_image("title_r")->render();

	// Render background last so that it appears on top of text images above
	Resource_manager::get_image("title_background")->render();
}

void Title_state::animate_logo()
{
	// Increase the Y position of the "Four" text until it reaches 49
	if (title_f_y < 49) {
		title_f_y = title_f_y + drop_speed;
		Resource_manager::get_image("title_f")->set_position(20, title_f_y);
	}

	if (title_o_y < 49) {
		title_o_y = title_o_y + drop_speed;
		Resource_manager::get_image("title_o")->set_position(39, title_o_y);
	}

	if (title_u_y < 49) {
		title_u_y = title_u_y + drop_speed;
		Resource_manager::get_image("title_u")->set_position(58, title_u_y);
	}

	if (title_r_y < 49) {
		title_r_y = title_r_y + drop_speed;
		Resource_manager::get_image("title_r")->set_position(77, title_r_y);
	}
}

bool Title_state::within_box(int x, int y, int box_position_x, int box_position_y, int box_width, int box_height)
{
	// Check four conditions to determine if X and Y are inside the box
	if (x >= box_position_x // Not too far left
		&& x < box_position_x + box_width // Not too far right
		&& y >= box_position_y // Not too far up
		&& y < box_position_y + box_height) // Not too far down
	{
		return true;
	}

	return false;
}