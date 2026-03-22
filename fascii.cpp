#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// A CLI SAMPLE TO SHOW HOW TO USE CLI's COMMAND AND ARGUMENTS

// THE PROJECT BASED ON ICETOWNE.COM MAN <JAVA PACKAGE LIKE COM.ICETOWNE.FASCII>
namespace ICETOWNE
{

    void printUsage(std::ostream &os)
    {
        os << "Usage: fascii [options] [arguments]...\n"
           << "Options:\n"
           << "  -c CHAR      Show ASCII code for CHAR.\n"
           << "  -s STRING    Show ASCII codes for all characters in STRING.\n"
           << "  -f FILE.txt  Show ASCII codes for every byte inside the text file.\n"
           << "  ARG          Treat ARG as a character (default behavior).\n";
    }

    bool hasTxtExtension(const std::string &path)
    {
        auto dotPos = path.rfind('.');
        if (dotPos == std::string::npos)
        {
            return false;
        }
        std::string ext = path.substr(dotPos + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch)
                       { return static_cast<char>(std::tolower(ch)); });
        return ext == "txt";
    }

    void printAscii(const std::vector<unsigned char> &bytes)
    {
        for (size_t idx = 0; idx < bytes.size(); ++idx)
        {
            std::cout << static_cast<int>(bytes[idx]);
            if (idx + 1 < bytes.size())
            {
                std::cout << ' ';
            }
        }
        if (!bytes.empty())
        {
            std::cout << '\n';
        }
    }

    std::vector<unsigned char> readFileBytes(const std::string &path)
    {
        std::ifstream in(path, std::ios::binary);
        if (!in)
        {
            throw std::runtime_error("cannot open file");
        }
        std::vector<unsigned char> bytes;
        char buffer;
        while (in.get(buffer))
        {
            bytes.push_back(static_cast<unsigned char>(buffer));
        }
        return bytes;
    }

} // namespace ICETOWNE

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        ICETOWNE::printUsage(std::cerr);
        return 1;
    }

    bool hasError = false;

    for (int idx = 1; idx < argc; ++idx)
    {
        std::string arg = argv[idx];
        try
        {
            if (arg == "-s")
            {
                if (++idx >= argc)
                {
                    std::cerr << "Error: -s requires an argument\n";
                    hasError = true;
                    continue;
                }
                std::string payload = argv[idx];
                std::vector<unsigned char> bytes(payload.begin(), payload.end());
                ICETOWNE::printAscii(bytes);
            }
            else if (arg == "-c")
            {
                if (++idx >= argc)
                {
                    std::cerr << "Error: -c requires an argument\n";
                    hasError = true;
                    continue;
                }
                std::string payload = argv[idx];
                if (payload.empty())
                {
                    std::cerr << "Error: Character cannot be empty\n";
                    hasError = true;
                    continue;
                }
                unsigned char ch = static_cast<unsigned char>(payload[0]);
                std::cout << static_cast<int>(ch) << '\n';
            }
            else if (arg == "-f")
            {
                if (++idx >= argc)
                {
                    std::cerr << "Error: -f requires a file path\n";
                    hasError = true;
                    continue;
                }
                std::string payload = argv[idx];
                if (!ICETOWNE::hasTxtExtension(payload))
                {
                    std::cerr << "Error: Only .txt files are supported (" << payload << ")\n";
                    hasError = true;
                    continue;
                }
                auto bytes = ICETOWNE::readFileBytes(payload);
                ICETOWNE::printAscii(bytes);
            }
            else if (!arg.empty() && arg[0] == '-')
            {
                std::cerr << "Error: Unknown option " << arg << "\n";
                hasError = true;
            }
            else
            {
                // Default behavior: treat as single char
                if (arg.empty())
                    continue;
                unsigned char ch = static_cast<unsigned char>(arg[0]);
                std::cout << static_cast<int>(ch) << '\n';
            }
        }
        catch (const std::exception &err)
        {
            std::cerr << "Error processing '" << arg << "': " << err.what() << '\n';
            hasError = true;
        }
    }

    return hasError ? 1 : 0;
}
