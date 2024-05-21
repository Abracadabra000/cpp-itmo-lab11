#include "reader.hpp"

void Reader::Read(int argc, char**argv) {
    std::string buffer;
    for (int i = 1; i < argc; ++i) {
        int j = 0;
        while (argv[i][j] != '\0') {
            buffer += argv[i][j];
            ++j;
        }
        input_.push_back(buffer);
        buffer = "";
    }
}

void Reader::Read(const std::vector<std::string> inp) {
    std::string buffer;
    for (int i = 1; i < inp.size(); ++i) {
        int j = 0;
        while (j < inp[i].size()) {
            buffer += inp[i][j];
            ++j;
        }
        input_.push_back(buffer);
        buffer = "";
    }
}

std::string Reader::FindValue(const std::string& rawCommand) {
    std::string command = "--" + rawCommand;
    command += '=';
    std::string result;
    bool isCommand;
    for (const auto& s : input_) {
        if (s.size() < command.size()) {
            continue;
        }
        isCommand = true;
        for (int i = 0; i < command.size(); ++i) {
            if (command[i] != s[i]) {
                isCommand = false;
                break;
            }
        }
        if (isCommand) {
            for (int i = command.size(); i < s.size(); ++i) {
                result += s[i];
            }
            break;
        } else {
            isCommand = false;
        }
    }
    if (!isCommand) {
        throw command_not_found_error{};
    }
    return result;
}

std::vector<std::string> Reader::FindMultivalue(const std::string& rawCommand) {
    std::string command = "--" + rawCommand;
    int i;
    std::vector<std::string> result;
    for (i = 0; i < input_.size(); ++i) {
        if (input_[i] == command) {
            ++i;
            break;
        }
    }
    for (i; i < input_.size(); ++i) {
        if (input_[i].size() >= 2 && input_[i][0] == '-' && input_[i][1] == '-') {
            break;
        }
        result.push_back(input_[i]);
    }
    return result;
}

bool Reader::FindFlag(const std::string& rawCommand) {
    std::string command = "--" + rawCommand;
    for (const auto& s : input_) {
        if (s == command) {
            return true;
        }
    }
    return false;
}

bool Reader::FindShortFlag(const std::string& rawCommand) {
    std::string command = "-" + rawCommand;
    for (const auto& s : input_) {
        if (s == command) {
            return true;
        }
    }
    return false;
}

std::string Reader::FindPositional() {
    std::string result;
    for (const auto& s : input_) {
        if (s.size() < 2 || s[0] != '-' || s[1] != '-') {
            if (result != "") result += ' ';
            result += s;
        }
    }
    return result;
}