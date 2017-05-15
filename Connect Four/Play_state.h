/*
 * Connect Four
 * MIT Licence, Copyright 2017 Chris Kempson (chriskempson.com)
 */

#pragma once
#include <SDL.h>
#include "State.h"
#include "Grid.h"

/**
 * State implementation for handling the game play state
 *
 * See State class for documentation on init(), handle_events(), update(),
 * render() and clean_up().
 */
class Play_state : public State
{
public:
	void init();
	void handle_events(SDL_Event& event);
	void update();
	void render();
	void clean_up();

private:
	/// Instance of the Grid class
	Grid grid;

	/// The sprite colour to be played, this will be toggled throughout moves 
	/// and is set to sprite_red here so that red players goes first
	Grid::sprite_sheet sprite_to_play = grid.sprite_red;

	/// Holds Grid.player_red (1), Grid.player_yellow (2) or Grid.player_red + 
	/// Grid.player_yellow (3) for a draw
	int win_type;

	/**
	 * Called by handle_events() when a mouse button is pressed and advances 
	 * the game by trying to play a move or clearing the grid and returning
	 * to the Title_state if there is a winner
	 */
	void advance_game();

	/**
	 * Try to play a move (place a red or yellow piece) in the given column 
	 * and row
	 *
	 * @param col Grid column number to play move in
	 * @param row Grid row number to play move in
	 *
	 * @return True if a move could be made
	 */
	bool play_a_move(int col, int row);

	/**
	 * See if the grid contains a win or if all moves have been made
	 *
	 * @param col Grid column number to run check from
	 * @param row Grid row number to run check from
	 *
	 * @return True if a win or draw was found
	 */
	bool check_for_win_or_draw(int col, int row);

private:
	/// X, Y and to Y values for red piece drop animation
	int red_piece_x, red_piece_y, red_piece_to_y;

	/// X, Y and to Y values for yellow piece drop animation
	int yellow_piece_x, yellow_piece_y, yellow_piece_to_y;

	/// Store the column and row of the last move
	int previous_play_col, previous_play_row;

	/// Store the previous sprite to play (red or yellow) of the last move
	Grid::sprite_sheet previous_play_sprite;

	/// Speed of animation for dropping (gravity) of a piece
	int drop_speed;

	/**
	 * Animate the dropping (gravity) of a piece when played
	 */
	void animate_drop_on_play();

	/**
	 * Check to see if the drop animation is playing
	 *
	 * @return true if the animation is still playing
	 */
	bool drop_animation_is_playing();
};