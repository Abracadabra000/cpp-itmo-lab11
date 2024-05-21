#include <iostream>
#include <vector>

class Reader {
public:

    Reader():
        input_{}
    {}

    void Read(int argc, char** argv);

    std::string FindValue(const std::string& command);

    std::vector<std::string> FindMultivalue(const std::string& command);

    bool FindFlag(const std::string& command);

    bool FindShortFlag(const std::string& command);

    std::string FindPositional();


private:

    std::vector<std::string> input_;
};

class command_not_found_error : std::exception {
public:
    const char* what() const noexcept override {
        return "Error! Can\'t find required command option!\n";
    }
};