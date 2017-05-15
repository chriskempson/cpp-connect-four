/*
 * Connect Four
 * MIT Licence, Copyright 2017 Chris Kempson (chriskempson.com)
 */

#pragma once
#include <SDL.h>
#include "State.h"

/**
 * State implementation for handling the title screen state
 *
 * See State class for documentation on init(), handle_events(), update(),
 * render() and clean_up().
 */
class Title_state : public State
{
public:
	void init();
	void handle_events(SDL_Event& event);
	void update();
	void render();
	void clean_up();

private:
	int title_f_y, title_o_y, title_u_y, title_r_y, drop_speed;

	/**
	 * Animate the "Four" part of the logo
	 */
	void animate_logo();

	/**
	 * A Checks whether or not given coords are within supplied box coords
	 *
	 * @parm position_x X position on axis coordinate to check against box
	 * @parm position_y Y position on axis coordinate to check against box
	 * @parm box_position_x X position on axis of box
	 * @parm box_position_y Y position on axis of box
	 * @parm box_width Width of box
	 * @parm box_height Height of box
	 * @return true if coords are within box
	 */
	bool within_box(int position_x, int position_y, int box_position_x, int box_position_y, int box_width, int box_height);
};
