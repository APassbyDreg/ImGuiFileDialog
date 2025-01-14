#include "Test_FileManager.h"

#include <cassert>

#include "ImGuiFileDialog/ImGuiFileDialog.h"

#pragma region Helper

class FileManagerTestHelper {
private:
    IGFD::FileDialogInternal fd;
    std::set<std::string> arr;

private:
    void compute_name_array() {
        for (const auto& file_ptr : fd.puFileManager.prFileList) {
            if (file_ptr) {
                arr.emplace(file_ptr->fileNameExt);
            }
        }
    }

public:
    void addFilter(const std::string& vFilter) {
        fd.puFilterManager.ParseFilters(vFilter.c_str());
        fd.puFilterManager.SetSelectedFilterWithExt(vFilter);
    }

    void addFile(const std::string& vFile) {
        fd.puFileManager.AddFile(fd, ".", vFile, IGFD::FileType(IGFD::FileType::ContentType::File, false));
    };

    void addFiles(const std::vector<std::string>& vFiles) {
        for (const auto& file : vFiles) {
            addFile(file);
        }
    };


    bool isFileExist(const std::string& vFile) {
        if (arr.empty()) {
            compute_name_array();
        }
        return (arr.find(vFile) != arr.end());
    }
};  

#pragma endregion

#pragma region Filtering for .* patterns

bool Test_IGFD_FileManager_Filtering_asterisk_0() {
    FileManagerTestHelper mgr;
    mgr.addFilter(".*");
    mgr.addFiles({"toto.a", "toto.b","titi.c"});

    if (!mgr.isFileExist("toto.a")) return false;
    if (!mgr.isFileExist("toto.b")) return false;
    if (!mgr.isFileExist("titi.c")) return false;

    return true;
}

bool Test_IGFD_FileManager_Filtering_asterisk_1() {
    FileManagerTestHelper mgr;
    mgr.addFilter(".*.*");
    mgr.addFiles({"toto.a.b", "toto.b.c", "titi.c"});

    if (!mgr.isFileExist("toto.a.b")) return false;
    if (!mgr.isFileExist("toto.b.c")) return false;
    if (mgr.isFileExist("titi.c")) return false;

    return true;
}

bool Test_IGFD_FileManager_Filtering_asterisk_2() {
    FileManagerTestHelper mgr;
    mgr.addFilter(".a.*");
    mgr.addFiles({"toto.a.b", "toto.b.c", "titi.a.c"});

    if (!mgr.isFileExist("toto.a.b")) return false;
    if (mgr.isFileExist("toto.b.c")) return false;
    if (!mgr.isFileExist("titi.a.c")) return false;

    return true;
}

bool Test_IGFD_FileManager_Filtering_asterisk_3() {
    FileManagerTestHelper mgr;
    mgr.addFilter(".*.b");
    mgr.addFiles({"toto.a.b", "toto.c.b", "titi.a.bb"});

    if (!mgr.isFileExist("toto.a.b")) return false;
    if (!mgr.isFileExist("toto.c.b")) return false;
    if (mgr.isFileExist("titi.a.bb")) return false;

    return true;
}

#pragma endregion

#pragma region Filtering Divers

bool Test_IGFD_FileManager_Filtering_divers_0() {
    FileManagerTestHelper mgr;
    mgr.addFilter(".a.b");
    mgr.addFiles({"toto.a.b", "toto.b.c", "titi.a.c"});

    if (!mgr.isFileExist("toto.a.b")) return false;
    if (mgr.isFileExist("toto.b.c")) return false;
    if (mgr.isFileExist("titi.a.c")) return false;

    return true;
}

bool Test_IGFD_FileManager_Filtering_divers_1() {
    FileManagerTestHelper mgr;
    mgr.addFilter("{.filters, .a.b}");
    mgr.addFiles({"toto.a.filters", "titi.a.b", "toto.c.a.b", "tata.t.o", "toto.filters"});

    if (!mgr.isFileExist("toto.filters")) return false;
    if (!mgr.isFileExist("titi.a.b")) return false;
    if (!mgr.isFileExist("toto.c.a.b")) return false;
    if (mgr.isFileExist("tata.t.o")) return false;
    if (!mgr.isFileExist("toto.a.filters")) return false;

    return true;
}

#pragma endregion

#pragma region Entry Point

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

bool Test_FileManager(const std::string& vTest) {
    IfTestExist(Test_IGFD_FileManager_Filtering_asterisk_0);
    IfTestExist(Test_IGFD_FileManager_Filtering_asterisk_1);
    IfTestExist(Test_IGFD_FileManager_Filtering_asterisk_2);
    IfTestExist(Test_IGFD_FileManager_Filtering_asterisk_3);
    IfTestExist(Test_IGFD_FileManager_Filtering_divers_0);
    IfTestExist(Test_IGFD_FileManager_Filtering_divers_1);

    assert(0);

    return false;
}

#pragma endregion
