/**
 * @file consolez80.cpp
 *
 * @brief Implementation of ConsoleZ80 class
 *
 * The Console Z80 class provides a vi-like text interface to debug a z80 program
 * It uses ncurses library to implement the tui
 *
 * @note Uses emuzeta80 as Z80 emulator
 *
 * @see LICENSE for licensing details.
 */

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include "consolez80.h"

#include <math.h>
#include <sstream>
#include <string>

//-------------------------------------------------------------------------
// Class implementation
//-------------------------------------------------------------------------

/**
 * Constructor
 */
ConsoleZ80::ConsoleZ80()
{
    // Initializes CPU instance with 16k of memory
    cpu = new emuzeta80::CPU(0x1000);

    // Create mapping of tui commands
    initialize_commands();
}

/**
 * Initializes the mapping of tui commands
 * - Each commands is associated to a method with command tokens as parameter
 */
void ConsoleZ80::initialize_commands()
{
    map_commands["q"] = &ConsoleZ80::quit;
    map_commands["quit"] = &ConsoleZ80::quit;
    map_commands["showcpu"] = &ConsoleZ80::show_cpu_panel;
    map_commands["hidecpu"] = &ConsoleZ80::hide_cpu_panel;
    map_commands["load"] = &ConsoleZ80::load_binary;
    map_commands["step"] = &ConsoleZ80::execute_step;
    map_commands["write"] = &ConsoleZ80::write_memory;
    map_commands["pc"] = &ConsoleZ80::set_pc;
    map_commands["update"] = &ConsoleZ80::update_memory;
}

/**
 * Initializes the windows and panels for the Z80 tui
 */
void ConsoleZ80::initialize_layouts()
{
    ConsoleUI::initialize_layouts();

    // Calculate the number of columns for the right side
    int right_cols = round(COLS * 0.2);

    // Configure window with memory status on the left side
    memory_window = newwin(LINES - 2, COLS - right_cols - 4, 1, 2);
    wrefresh(memory_window);

    // Configure window with CPU information on the right side
    cpu_window = newwin(LINES - 2, right_cols, 0, COLS - right_cols);
    cpu_panel = new_panel(cpu_window);

    // Display memory content from address 0x0000
    display_memory(0);

    // Display CPU panel
    update_cpu_panel();
}

/**
 * Main entry point of tui interface for z80
 */
int ConsoleZ80::execute()
{
    return ConsoleUI::execute();
}

/**
 * Update CPU panel with current register values and CPU status
 */
void ConsoleZ80::update_cpu_panel()
{
    mvwprintw(cpu_window, 2, 2, "PC: 0x%04x", cpu->getpc());
    mvwprintw(cpu_window, 3, 2, "SP: 0x%04x", cpu->getsp());
    mvwprintw(cpu_window, 5, 2, "AF: 0x%04x", cpu->getaf());
    mvwprintw(cpu_window, 6, 2, "BC: 0x%04x", cpu->getbc());
    mvwprintw(cpu_window, 7, 2, "DE: 0x%04x", cpu->getde());
    mvwprintw(cpu_window, 8, 2, "HL: 0x%04x", cpu->gethl());
    mvwprintw(cpu_window, 10, 2, "AF': 0x%04x", cpu->getaf(true));
    mvwprintw(cpu_window, 11, 2, "BC': 0x%04x", cpu->getbc(true));
    mvwprintw(cpu_window, 12, 2, "DE': 0x%04x", cpu->getde(true));
    mvwprintw(cpu_window, 13, 2, "HL': 0x%04x", cpu->gethl(true));
    mvwprintw(cpu_window, 15, 2, "iX: 0x0000");
    mvwprintw(cpu_window, 16, 2, "iY: 0x0000");
    mvwprintw(cpu_window, 18, 2, "Cycles: %d", cpu->getClockCycles());

    wrefresh(cpu_window);
}

/**
 * Processes input in normal mode for the Z80 tui
 * - Handles the specified character (ch) in the context of normal mode operations
 * - Performs actions or triggers functions based on the input character
 */
void ConsoleZ80::process_normal_mode(int ch)
{
    ConsoleUI::process_normal_mode(ch);

    switch (ch)
    {
        case 's': // execute step
            execute_step({});
            break;

        default:
            break;
    }
}

/**
 * Displays memory content starting from the specified address in the Z80 tui
 */
void ConsoleZ80::display_memory(uint16_t address)
{
    wmove(memory_window, 0, 1);
    for (ssize_t i = 0; i < LINES - 4; i++)
    {
        auto i_addr = i + address;
        if (i_addr == cpu->getpc())
            wattron(memory_window, A_BOLD);
        mvwprintw(memory_window, i, 0, "0x%04x: 0x%02x", i_addr, cpu->read(i_addr));
        wattroff(memory_window, A_BOLD);
    }

    wrefresh(memory_window);
}

/**
 * Process tui command entered by the user
 */
void ConsoleZ80::process_command(const command_tokens& tokens)
{
    auto it = map_commands.find(tokens[0]);
    if (it != map_commands.end())
    {
        (this->*it->second)(tokens);
    }
    else
    {
        print_error("ERROR: Command not recognized (" + tokens[0] + ")");
    }

    mode = 0;
    curs_set(0);
}

/**
 * Load binary data from a file to CPU memory
 */
bool ConsoleZ80::load_binary(const char* filename)
{

    return true;
}

//-------------------------------------------------------------------------
// TUI commands implementation
//-------------------------------------------------------------------------

void ConsoleZ80::quit(const command_tokens& tokens)
{
    end = true;
}

void ConsoleZ80::show_cpu_panel(const command_tokens& tokens)
{
    show_panel(cpu_panel);
    update_panels();
    doupdate();
}

void ConsoleZ80::hide_cpu_panel(const command_tokens& tokens)
{
    hide_panel(cpu_panel);
    update_panels();
    doupdate();
}

void ConsoleZ80::execute_step(const command_tokens& tokens)
{
    cpu->execute();
    update_cpu_panel();
    display_memory(start_address);
}

void ConsoleZ80::write_memory(const command_tokens& tokens)
{
    if (tokens.size() < 3)
    {
        print_error("ERROR: Missing arguments");
        return;
    }

    auto address = std::stoi(tokens[1]);
    auto value = std::stoi(tokens[2]);
    cpu->write(value, address);
    display_memory(start_address);
}

void ConsoleZ80::set_pc(const command_tokens& tokens)
{
    if (tokens.size() < 2)
    {
        print_error("ERROR: Missing argument (PC)");
        return;
    }

    auto pc = std::stoi(tokens[1]);
    cpu->setpc(pc);
    display_memory(pc);
    update_cpu_panel();
}

void ConsoleZ80::update_memory(const command_tokens& tokens)
{
    start_address = std::stoi(tokens[1]);
    display_memory(start_address);
}

void ConsoleZ80::load_binary(const command_tokens& tokens)
{
    if (tokens.size() < 2)
    {
        print_error("ERROR: Missing argument (filename)");
        return;
    }

    auto filename = tokens[1].c_str();
    auto file = fopen(filename, "rb");
    if (file == NULL)
    {
        print_error("ERROR: Unable to open the file");
        return;
    }

    char buffer[4];
    size_t bytesRead;
    uint16_t pos = 0;
    while (bytesRead = fread(buffer, sizeof(char), 1, file))
    {
        auto value = buffer[0];
        cpu->write(value, pos++);
    }

    cpu->setpc(0);
    display_memory(0);
}