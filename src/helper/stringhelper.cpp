
#include <stringhelper.h>

namespace helper
{
std::vector<std::string> StringHelper::split_command(std::string command)
{
    // Create an input string stream
    std::istringstream iss(command);

    // Use a vector to store the split words
    std::vector<std::string> tokens;

    // Read each word from the stream and store it in the vector
    std::string word;
    while (std::getline(iss, word, ' '))
    {
        tokens.push_back(word);
    }

    return tokens;
}

} // namespace helper
