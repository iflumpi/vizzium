/**
 * @file consoleui.cpp
 *
 * @brief Implementation of ConsoleUI class
 *
 * Provides a generic class to implement vi-like text interfaces
 * It uses ncurses library to implement the tui
 *
 * @see LICENSE for licensing details.
 */

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include "consoleui.h"
#include "stringhelper.h"

//-------------------------------------------------------------------------
// Class implementation
//-------------------------------------------------------------------------

/**
 * Constructor
 */
ConsoleUI::ConsoleUI()
{
}

/**
 * Initializes layouts and set ncurses settings
 */
void ConsoleUI::initialize_layouts()
{
    initscr();
    noecho();
    curs_set(0);

    start_color();
    use_default_colors();

    clear();

    commands_window = newwin(1, COLS, LINES - 1, 0);
    keypad(commands_window, TRUE);
    wrefresh(commands_window);
}

/**
 * Main entry point of tui interface
 * - supports normal and command modes
 */
int ConsoleUI::execute()
{
    initialize_layouts();

    int ch;
    int cursor_command = -1;

    while (!end)
    {
        ch = wgetch(commands_window);

        switch (mode)
        {
            case 0:
                process_normal_mode(ch);
                break;

            case 1:
                process_command_mode(ch, &cursor_command);
                break;

            default:
                break;
        }
    }

    endwin();

    return 0;
}

/**
 * Processes input in normal mode for the tui
 */
void ConsoleUI::process_normal_mode(int ch)
{
    switch (ch)
    {
        case ':':
            mode = 1;
            curs_set(1);
            clear_command_window();
            wprintw(commands_window, ":");
            command.erase();
            break;
    }
}

/**
 * Processes input in command mode for the tui
 */
void ConsoleUI::process_command_mode(int ch, int* cursor_command)
{
    switch (ch)
    {
        case 10: // ENTER key
            process_command(helper::StringHelper::split_command(command));
            commands.insert(commands.begin(), command);
            command.erase();
            break;

        case 27: // ESC Key
            mode = 0;
            curs_set(0);
            clear_command_window();

            break;

        case 8: // DEL key
            getyx(commands_window, y, x);
            mvwdelch(commands_window, y, x - 1);
            command = command.substr(0, command.length() - 1);
            break;

        case KEY_UP:
            if (commands.size() < *cursor_command + 2)
                break;

            clear_command_window();
            command = commands.at(++(*cursor_command));
            wprintw(commands_window, ":%s", command.c_str());
            break;

        case KEY_DOWN:
            if (*(cursor_command) <= 0)
                break;

            clear_command_window();
            command = commands.at(--*(cursor_command));
            wprintw(commands_window, ":%s", command.c_str());
            break;

        case '\t':
            // autocomplete features
            break;

        default:
            if (!isprint(ch))
                break;

            command += ch;
            wprintw(commands_window, "%c", ch);
            break;
    }
}

/**
 * Process tui command entered by the user
 * - this method shall be overrided by derived classes according to its functionalities
 */
void ConsoleUI::process_command(const command_tokens& tokens)
{
    if (tokens[0] == "q" || tokens[0] == "quit")
    {
        end = true;
    }
}

/**
 * Clean and restore command window
 */
void ConsoleUI::clear_command_window()
{
    wbkgd(commands_window, COLOR_PAIR(0));
    getyx(commands_window, y, x);
    wmove(commands_window, y, 0);
    wclrtoeol(commands_window);
}

/**
 * Print an error message in command window
 */
void ConsoleUI::print_error(const std::string& message)
{
    init_pair(1, COLOR_WHITE, COLOR_RED);
    wbkgd(commands_window, COLOR_PAIR(1));
    mvwprintw(commands_window, y, 0, message.c_str());
}
