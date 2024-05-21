#include <gtest/gtest.h>
#include "InvertIndexLib/directoryInverter.hpp"
#include "SearchEngineLib/SearchProcessor.hpp"
#include "InputReader/reader.hpp"

const fs::path kTestDir1 = fs::absolute("testdir1");
const fs::path kTestDir2 = fs::absolute("testdir2");

namespace inv = invindex;
namespace srch = searcheng;

TEST(MakeIndex, Test1) {
    Reader reader;
    std::vector<std::string> input = {"MakeIndex.exe",
                                      "--target-dir=testdir1",
                                      "--override"};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    bool ovr = reader.FindFlag("override");
    std::vector<std::string> ignore = reader.FindMultivalue("ignore");
    inv::DirectoryInverter inverter{targetDir, ovr, ignore};
    ASSERT_NO_THROW(inverter.MakeInvertedIndex());
}

TEST(MakeIndex, Test2) {
    Reader reader;
    std::vector<std::string> input = {"MakeIndex.exe",
                                      "--target-dir=testdir2",
                                      "-ov",
                                      "--ignore",
                                      "README.md",
                                      ".git",
                                      ".gitignore",
                                      ".vscode",
                                      "build"};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    bool ovr = reader.FindShortFlag("ov");
    std::vector<std::string> ignore = reader.FindMultivalue("ignore");
    inv::DirectoryInverter inverter{targetDir, ovr, ignore};
    ASSERT_NO_THROW(inverter.MakeInvertedIndex());
}

TEST(Search, Test1) {
    Reader reader;
    std::vector<std::string> input = {"Search.exe",
                                      "--target-dir=testdir1",
                                      "aaa"};
    std::vector<srch::LineMatch> ans = {{"test1.txt", 1}, {"test2.txt", 3}};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    std::string expression = reader.FindPositional();
    srch::SearchProcessor searcher{targetDir / srch::kInvertIndexFileName};
    auto result = searcher.Search(expression);
    for (int i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].document_, ans[i].document_);
        ASSERT_EQ(result[i].line_, ans[i].line_);
    }
}

TEST(Search, Test2) {
    Reader reader;
    std::vector<std::string> input = {"Search.exe",
                                      "--target-dir=testdir1",
                                      "aaa OR (abra AND cadabra)"};
    std::vector<srch::LineMatch> ans = {{"test1.txt", 1}, {"test1.txt", 2}, {"test2.txt", 3}};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    std::string expression = reader.FindPositional();
    srch::SearchProcessor searcher{targetDir / srch::kInvertIndexFileName};
    auto result = searcher.Search(expression);
    for (int i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].document_, ans[i].document_);
        ASSERT_EQ(result[i].line_, ans[i].line_);
    }
}

TEST(Search, Test3) {
    Reader reader;
    std::vector<std::string> input = {"Search.exe",
                                      "--target-dir=testdir2",
                                      "iterator"};
    std::vector<srch::LineMatch> ans = {{"labwork8-Abracadabra000/lib/hmbst.hpp", 887}, {"labwork8-Abracadabra000/lib/hmbst.hpp", 890},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 893}, {"labwork8-Abracadabra000/lib/hmbst.hpp", 896},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 899}};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    std::string expression = reader.FindPositional();
    srch::SearchProcessor searcher{targetDir / srch::kInvertIndexFileName};
    auto result = searcher.Search(expression);
    for (int i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].document_, ans[i].document_);
        ASSERT_EQ(result[i].line_, ans[i].line_);
    }
}

TEST(Search, Test4) {
    Reader reader;
    std::vector<std::string> input = {"Search.exe",
                                      "--target-dir=testdir2",
                                      "auto"};
    std::vector<srch::LineMatch> ans = {{"labwork8-Abracadabra000/lib/hmbst.hpp", 154},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 171}};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    std::string expression = reader.FindPositional();
    srch::SearchProcessor searcher{targetDir / srch::kInvertIndexFileName};
    auto result = searcher.Search(expression);
    for (int i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].document_, ans[i].document_);
        ASSERT_EQ(result[i].line_, ans[i].line_);
    }
}

TEST(Search, Test5) {
    Reader reader;
    std::vector<std::string> input = {"Search.exe",
                                      "--target-dir=testdir2",
                                      "while OR vector"};
    std::vector<srch::LineMatch> ans = {{"labwork8-Abracadabra000/lib/hmbst.hpp", 241},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 274},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 334},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 357},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 374},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 385},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 399},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 447},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 462},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 713},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 722},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 838},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 845},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 862},
                                        {"labwork8-Abracadabra000/lib/hmbst.hpp", 1028}};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    std::string expression = reader.FindPositional();
    srch::SearchProcessor searcher{targetDir / srch::kInvertIndexFileName};
    auto result = searcher.Search(expression);
    for (int i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].document_, ans[i].document_);
        ASSERT_EQ(result[i].line_, ans[i].line_);
    }
}

TEST(SearchException, Test1) {
    Reader reader;
    std::vector<std::string> input = {"Search.exe",
                                      "--target-dir=testdir",
                                      "auto"};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    std::string expression = reader.FindPositional();
    ASSERT_ANY_THROW(srch::SearchProcessor searcher{targetDir / srch::kInvertIndexFileName});
}

TEST(SearchException, Test2) {
    Reader reader;
    std::vector<std::string> input = {"Search.exe",
                                      "--target-dir=testdir2",
                                      "auto OR"};
    reader.Read(input);
    fs::path targetDir = reader.FindValue("target-dir");
    std::string expression = reader.FindPositional();
    srch::SearchProcessor searcher{targetDir / srch::kInvertIndexFileName};
    ASSERT_ANY_THROW(auto result = searcher.Search(expression));
}