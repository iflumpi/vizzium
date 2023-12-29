/**
 * @file consolez80.cpp
 *
 * @brief Definition of ConsoleZ80 class
 *
 * @see LICENSE for licensing details
 */

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include "consoleui.h"
#include "emuzeta80/CPU.h"

#include <functional>
#include <map>
#include <panel.h>
#include <stdint.h>
#include <unordered_map>

//-------------------------------------------------------------------------
// Class definition
//-------------------------------------------------------------------------

class ConsoleZ80 : public ConsoleUI
{

public:
    ConsoleZ80();
    ~ConsoleZ80();
    int execute();

protected:
    void initialize_layouts();
    void initialize_commands();
    void process_normal_mode(int ch);
    void process_command(const command_tokens& tokens);
    void update_cpu_panel();
    bool load_binary(const char* filename);
    void display_memory(uint16_t address);

protected: // tui commands
    void quit(const command_tokens& tokens);
    void show_cpu_panel(const command_tokens& tokens);
    void hide_cpu_panel(const command_tokens& tokens);
    void execute_step(const command_tokens& tokens);
    void write_memory(const command_tokens& tokens);
    void set_pc(const command_tokens& tokens);
    void update_memory(const command_tokens& tokens);
    void load_binary(const command_tokens& tokens);

protected:
    emuzeta80::CPU* cpu = nullptr;

protected:
    WINDOW* memory_window = nullptr;
    WINDOW* cpu_window = nullptr;
    PANEL* cpu_panel = nullptr;

protected:
    std::unordered_map<std::string, void (ConsoleZ80::*)(const command_tokens&)> map_commands;
    int start_address = 0;
    std::string command;
};