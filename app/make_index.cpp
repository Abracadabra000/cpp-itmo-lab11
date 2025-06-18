#include "../InvertIndexLib/directoryInverter.hpp"
#include "../InputReader/reader.hpp"

namespace inv = invindex;

int main(int argc, char** argv) {
    std::string targetDir;
    bool overrideFlag;
    try
    {
    
        Reader reader;
        reader.Read(argc, argv);
        targetDir = reader.FindValue("target-dir");
        std::vector<std::string> ignore = reader.FindMultivalue("ignore");
        std::cout << "Ignore List:\n";
        for (const auto& i : ignore) {
            std::cout << i << "\n";
        }
        overrideFlag = reader.FindFlag("override") || reader.FindShortFlag("ov");
        fs::path targetPath{targetDir};
        inv::DirectoryInverter inverter{targetPath, overrideFlag, ignore};
        inverter.MakeInvertedIndex();

    }
    catch (const inv::bad_hash_calculation_error& error)
    {
    
        std::cout << error.what();

    }
    catch (const inv::invert_index_file_overwrite_error& error)
    {

        std::cout << error.what();

    }
    catch (const inv::cannot_find_directory_error& error)
    {
        
        std::cout << error.what();

    }
    catch (const std::exception& error)
    {

        std::cout << error.what();

    }
    catch (...)
    {
        std::cout << "Unknown error occured!";
    }
}