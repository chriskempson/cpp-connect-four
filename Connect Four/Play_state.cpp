/*
 * Connect Four
 * MIT Licence, Copyright 2017 Chris Kempson (chriskempson.com)
 */

#include "Play_state.h"
#include "Game.h"
#include "Setting.h"
#include "State_manager.h"
#include "Resource_manager.h"
#include "Title_state.h"

void Play_state::init()
{
	Resource_manager::load_image("cell_sprite");
	Resource_manager::load_image("grid_overlay");

	Resource_manager::load_image("red_piece")->set_position_hidden();
	Resource_manager::load_image("yellow_piece")->set_position_hidden();

	Resource_manager::load_image("red_wins")->set_position_centered();
	Resource_manager::load_image("yellow_wins")->set_position_centered();
	Resource_manager::load_image("draw")->set_position_centered();

	Resource_manager::load_sound("valid_move");
	Resource_manager::load_sound("invalid_move");
	Resource_manager::load_sound("win");
	Resource_manager::load_sound("draw");

	grid.init();

	// Get the current position of the red piece for the drop animation
	red_piece_x = Resource_manager::load_image("red_piece")->get_position_x();
	red_piece_y = Resource_manager::load_image("red_piece")->get_position_y();

	// Get the current position of the yellow piece for the drop animation
	yellow_piece_x = Resource_manager::load_image("red_piece")->get_position_x();
	yellow_piece_y = Resource_manager::load_image("red_piece")->get_position_y();

	// Store the last played move as this is not acted upon until the next 
	// move so that the played piece does not appear before the drop 
	// animation has finished
	previous_play_col = -1;
	previous_play_row = -1;
	previous_play_sprite = grid.sprite_red;

	// Must divide nicely into 16 or it wont fall on the right parts of the y 
	// axis during animations causing glitches
	drop_speed = 8;
}


void Play_state::clean_up()
{
	Resource_manager::unload_image("cell_sprite");
	Resource_manager::unload_image("grid_overlay");

	Resource_manager::unload_image("red_piece");
	Resource_manager::unload_image("yellow_piece");

	Resource_manager::unload_image("red_wins");
	Resource_manager::unload_image("yellow_wins");
	Resource_manager::unload_image("draw");

	Resource_manager::unload_sound("valid_move");
	Resource_manager::unload_sound("invalid_move");
	Resource_manager::unload_sound("win");
	Resource_manager::unload_sound("draw");
}

void Play_state::handle_events(SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		advance_game();
	}
}

void Play_state::update()
{
	animate_drop_on_play();
}

void Play_state::render()
{
	// Render yellow and red pieces now so that they will appear under the
	// grid overlay that is rendered next
	Resource_manager::get_image("red_piece")->render();
	Resource_manager::get_image("yellow_piece")->render();

	grid.render();

	// Return if drop animation hasn't finished otherwise the win or draw 
	// message will appear too early 
	if (drop_animation_is_playing()) {
		return;
	}

	// Depending on the win or draw conditions, display a win or draw message
	// overlay image
	if (win_type == grid.player_red) {
		Resource_manager::get_image("red_wins")->render();
		Resource_manager::get_sound("win")->play_once_only();

	}
	else if (win_type == grid.player_yellow) {
		Resource_manager::get_image("yellow_wins")->render();
		Resource_manager::get_sound("win")->play_once_only();
	}
	else if (win_type == grid.player_red + grid.player_yellow) {
		Resource_manager::get_image("draw")->render();
		Resource_manager::get_sound("draw")->play_once_only();
	}
}

void Play_state::advance_game() {

	// Don't allow a player to advance the game before the drop animation has
	// finished
	if (drop_animation_is_playing()) {
		return;
	}

	// If we have no winner
	if (!win_type) {

		// Get mouse coordinates and normalise to row and columns so we know 
		// in which column and row we have clicked
		int mouse_x, mouse_y;
		Game::get_mouse_position(&mouse_x, &mouse_y);
		int col = mouse_x / (Setting::window_width  / Setting::grid_columns);
		int row = mouse_y / (Setting::window_height / Setting::grid_rows);

		// Get the row that the play can be dropped to
		row = grid.get_bottommost_available_row_in_column(col);

		// Try to play a move...
		if (play_a_move(col, row)) {

			// See if there was a win or a draw
			check_for_win_or_draw(col, row);
		}
		// Otherwise play a sound for an invalid move
		else {
			Resource_manager::get_sound("invalid_move")->play();
		}

	}

	// Else if there was a win or draw, reset the winner, make next sprite 
	// red and clear the grid
	else {
		win_type = false;
		sprite_to_play = grid.sprite_red;
		grid.clear();
		State_manager::set_state(new Title_state());
	}
}

bool Play_state::play_a_move(int col, int row) {

	// As we are about to reuse an animation piece now is the time to fill
	// the previous play before moving the animation piece, otherwise a hole
    // would be left on the board. Before running this code variables 
    // "previous_play_col" and "previous_play_row" are checked to prevent the
    // code from running on first play when no previous play has been made.
	if (previous_play_col >= 0 && previous_play_row >= 0) {
		grid.cell[previous_play_col][previous_play_row].current_sprite = previous_play_sprite;
	}

	// If there is a row space in the column for the play to be dropped to
	if (row >= 0) {

		// Store this play so we can update the sprite cell next play
		previous_play_col = col;
		previous_play_row = row;
		previous_play_sprite = sprite_to_play;

		// Determine who is playing by checking sprite to use for play and toggle 
		// play by toggling sprite
		if (sprite_to_play == grid.sprite_red) {
			// Set the colour of the next sprite to be played
			sprite_to_play = grid.sprite_yellow;
			
			// Set the X axis value of the red piece so the animation will 
			// start and define how far the piece should drop
			red_piece_x = col * Setting::grid_sprite_width;
			red_piece_to_y = row * Setting::grid_sprite_height;

			// Mark the cell as played by the red player
			grid.cell[col][row].played_by = grid.player_red;
		}
		else {
			// Set the colour of the next sprite to be played
			sprite_to_play = grid.sprite_red;

			// Set the X axis value of the red piece so the animation will 
			// start and define how far the piece should drop
			yellow_piece_x = col * Setting::grid_sprite_width;
			yellow_piece_to_y = row * Setting::grid_sprite_height;

			// Mark the cell as played by the yellow player
			grid.cell[col][row].played_by = grid.player_yellow;
		}

		return true;
	}

	return false;
}

bool Play_state::check_for_win_or_draw(int col, int row) {

	// Check to see if the last move caused a win for the red player
	if (grid.check_for_win(col, row, grid.player_red)) {
		win_type = grid.player_red;
		return true;
	}

	// Check to see if the last move caused a win for the yellow player
	else if (grid.check_for_win(col, row, grid.player_yellow)) {
		win_type = grid.player_yellow;
		return true;
	}

	// Check to see if the last move caused a draw
	else if (grid.check_for_draw()) {
		win_type = grid.player_red + grid.player_yellow;
		return true;
	}

	return false;
}

void Play_state::animate_drop_on_play() {

	// A images initial hidden position is -width for x and -height for Y. 
	// play_a_move() will set the x axis value of a red or yellow piece and
	// used to discover if the animation needs to start

	// Drop animation - advance red piece on Y axis
	if (red_piece_x != -Setting::grid_sprite_width) {
		red_piece_y = red_piece_y + drop_speed;
		Resource_manager::get_image("red_piece")->set_position(red_piece_x, red_piece_y);
	}

	// Check if we need to stop red piece drop animation by checking current 
	// Y axis value against red_piece_to_y set by play_a_move()
	if (red_piece_y == red_piece_to_y) {
		red_piece_x = -Setting::grid_sprite_width;
		red_piece_y = -Setting::grid_sprite_height;

		Resource_manager::get_sound("valid_move")->play();
	}

	// Drop animation - advance red piece on Y axis
	if (yellow_piece_x != -Setting::grid_sprite_width) {
		Resource_manager::get_image("yellow_piece")->set_position(yellow_piece_x, yellow_piece_y += drop_speed);
	}

	// Check if we need to stop yellow piece drop animation by checking 
	// current Y axis value against yellow_piece_to_y set by play_a_move()
	if (yellow_piece_y == yellow_piece_to_y) {
		yellow_piece_x = -Setting::grid_sprite_width;
		yellow_piece_y = -Setting::grid_sprite_height;

		Resource_manager::get_sound("valid_move")->play();
	}
}

bool Play_state::drop_animation_is_playing() {
	// If a red or yellow piece is currently animating return true
	if (red_piece_y != -Setting::grid_sprite_height
		|| yellow_piece_y != -Setting::grid_sprite_height) {
		return true;
	}

	return false;
}