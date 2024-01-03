/**
 * @file consoleui.h
 *
 * @brief Implementation of ConsoleUI class
 *
 * Provides a generic class to implement vi-like text interfaces
 *
 * @see LICENSE for licensing details.
 */

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include <ncurses.h>
#include <string>
#include <vector>

//-------------------------------------------------------------------------
// Class definition
//-------------------------------------------------------------------------

typedef std::vector<std::string> command_tokens;

class ConsoleUI
{
public:
    ConsoleUI();
    ~ConsoleUI() = default;

    virtual int execute();

protected:
    virtual void initialize_layouts();
    virtual void process_normal_mode(int ch);
    virtual void process_command_mode(int ch, unsigned int* cursor_command);
    virtual void process_command(const command_tokens& tokens);
    void clear_command_window();
    void print_error(const std::string& message);

protected:
    WINDOW* commands_window = nullptr;
    int x = 0, y = 0;
    uint8_t mode = 0;
    bool end = false;
    std::string command;
    std::vector<std::string> commands;
};
