#include "Test_FilterManager.h"

#include <cassert>

#include "ImGuiFileDialog/ImGuiFileDialog.h"

// ".*,.cpp,.h,.hpp" => simple filters
// "Source files{.cpp,.h,.hpp},Image files{.png,.gif,.jpg,.jpeg},.md" => collection filters
// "([.][0-9]{3}),.cpp,.h,.hpp" => simple filters with regex
// "frames files{([.][0-9]{3}),.frames}" => collection filters with regex

#pragma region Helper

IGFD::FilterManager Test_IGFD_ParseFilters(const char* vFilters) {
    IGFD::FilterManager mgr;

    std::vector<IGFD::FilterInfos> prParsedFilters;
    IGFD::FilterInfos prSelectedFilter;
    std::string puDLGFilters;

    mgr.ParseFilters(vFilters);

    return mgr;
}

#pragma endregion

#pragma region Filters Simple

bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_0() {
    auto mgr = Test_IGFD_ParseFilters(".*");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != ".*") return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.].*))") return false;


    return true;
}

bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_1() {
    auto mgr = Test_IGFD_ParseFilters(".*,.cpp,.h,.hpp");

    if (mgr.prParsedFilters.size() != 4U) return false;
    if (mgr.prParsedFilters[0].title != ".*") return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.].*))") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".cpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".h") return false;
    if (mgr.prParsedFilters[3].getFirstFilter() != ".hpp") return false;

    return true;
}

// missing '.'
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_2() {
    auto mgr = Test_IGFD_ParseFilters("hpp");

    if (mgr.prParsedFilters.size() != 0U) return false;

    return true;
}

// empty filter after ','
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_3() {
    auto mgr = Test_IGFD_ParseFilters(".hpp,");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != ".hpp") return false;

    return true;
}

// good filter
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_4() {
    auto mgr = Test_IGFD_ParseFilters("..hpp");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "..hpp") return false;

    return true;
}

// good filter, but the second is empty
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_5() {
    auto mgr = Test_IGFD_ParseFilters("..hpp,");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "..hpp") return false;

    return true;
}

// good filters
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_6() {
    auto mgr = Test_IGFD_ParseFilters("..hpp,.hpp");

    if (mgr.prParsedFilters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "..hpp") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".hpp") return false;

    return true;
}

// good filter, but the third not
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_7() {
    auto mgr = Test_IGFD_ParseFilters("..hpp,.hpp,.");

    if (mgr.prParsedFilters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "..hpp") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".hpp") return false;

    return true;
}

// good filters
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_8() {
    auto mgr = Test_IGFD_ParseFilters("..hpp,.hpp,. .");

    if (mgr.prParsedFilters.size() != 3U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "..hpp") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".hpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != "..") return false;

    return true;
}

#pragma endregion

#pragma region Filters Collection

// must be good {filter0, filter1}
// 2022/06/21 => ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_0() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// missing end }
// 2022/06/21 => give an infinite loop
bool Test_IGFD_FilterManager_ParseFilters_Filters_1() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c");

    if (mgr.prParsedFilters.size() != 0U) return false;

    return true;
}

// missing start }
// 2022/06/21 => ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_2() {
    auto mgr = Test_IGFD_ParseFilters("Source Files.cpp,.c}");

    if (mgr.prParsedFilters.size() != 0U) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_3() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c},Header Files{.hpp,.h,.hxx}");

    if (mgr.prParsedFilters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != "Header Files") return false;
    if (mgr.prParsedFilters[1].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[1].filters.find(".hpp") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".hxx") == mgr.prParsedFilters[1].filters.end()) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_4() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c},Header Files{.hpp,.h,.hxx},");

    if (mgr.prParsedFilters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != "Header Files") return false;
    if (mgr.prParsedFilters[1].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[1].filters.find(".hpp") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".hxx") == mgr.prParsedFilters[1].filters.end()) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_5() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}, Header Files{.hpp,.h,.hxx} , ");

    if (mgr.prParsedFilters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != " Header Files") return false;
    if (mgr.prParsedFilters[1].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[1].filters.find(".hpp") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".hxx") == mgr.prParsedFilters[1].filters.end()) return false;

    return true;
}

// first { missing
bool Test_IGFD_FilterManager_ParseFilters_Filters_6() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}, Header Files.hpp,.h,.hxx} , ");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// last { missing
bool Test_IGFD_FilterManager_ParseFilters_Filters_7() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}, Header Files{.hpp,.h,.hxx , .md");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// last { missing
bool Test_IGFD_FilterManager_ParseFilters_Filters_8() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}, Header Files{.hpp,.h,.hxx},.md");

    if (mgr.prParsedFilters.size() != 3U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != " Header Files") return false;
    if (mgr.prParsedFilters[1].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[1].filters.find(".hpp") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".hxx") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".md") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_9() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}, Header Files{.hpp,.h,.hxx},.md,Other Files{.txt,.doc}");

    if (mgr.prParsedFilters.size() != 4U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != " Header Files") return false;
    if (mgr.prParsedFilters[1].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[1].filters.find(".hpp") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".hxx") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".md") return false;
    if (mgr.prParsedFilters[3].title != "Other Files") return false;
    if (mgr.prParsedFilters[3].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[3].filters.find(".txt") == mgr.prParsedFilters[3].filters.end()) return false;
    if (mgr.prParsedFilters[3].filters.find(".doc") == mgr.prParsedFilters[3].filters.end()) return false;

    return true;
}

// first { missing
bool Test_IGFD_FilterManager_ParseFilters_Filters_10() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}, Header Files{.hpp,.h,.hxx},.md, Other Files.txt,.doc}");

    if (mgr.prParsedFilters.size() != 3U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != " Header Files") return false;
    if (mgr.prParsedFilters[1].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[1].filters.find(".hpp") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".hxx") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".md") return false;

    return true;
}

// last { missing
bool Test_IGFD_FilterManager_ParseFilters_Filters_11() {
    auto mgr = Test_IGFD_ParseFilters("Source Files{.cpp,.c}, Header Files{.hpp,.h,.hxx},.md, Other Files{.txt,.doc");

    if (mgr.prParsedFilters.size() != 3U) return false;
    if (mgr.prParsedFilters[0].title != "Source Files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != " Header Files") return false;
    if (mgr.prParsedFilters[1].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[1].filters.find(".hpp") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[1].filters.find(".hxx") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".md") return false;

    return true;
}

#pragma endregion

#pragma region Filters Simple Regex

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_0() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3}))");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.][0-9]{3}))") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_1() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3}");

    if (mgr.prParsedFilters.size() != 0U) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_2() {
    auto mgr = Test_IGFD_ParseFilters("[.][0-9]{3}))");

    if (mgr.prParsedFilters.size() != 0U) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_3() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3})),.cpp,.h,.hpp");

    if (mgr.prParsedFilters.size() != 4U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".cpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".h") return false;
    if (mgr.prParsedFilters[3].getFirstFilter() != ".hpp") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_4() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3})),.cpp,(([.][0-9]{3})),.h,.hpp");

    if (mgr.prParsedFilters.size() != 5U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".cpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[3].getFirstFilter() != ".h") return false;
    if (mgr.prParsedFilters[4].getFirstFilter() != ".hpp") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_5() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3})),.cpp,(([.][0-9]{3})),.h,.hpp");

    if (mgr.prParsedFilters.size() != 5U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".cpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[3].getFirstFilter() != ".h") return false;
    if (mgr.prParsedFilters[4].getFirstFilter() != ".hpp") return false;

    return true;
}

// last ) missing
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_6() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3})),.cpp,(([.][0-9]{3},.h,.hpp");

    if (mgr.prParsedFilters.size() != 4U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".cpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".h") return false;
    if (mgr.prParsedFilters[3].getFirstFilter() != ".hpp") return false;

    return true;
}

// first ( missing
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_7() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3})),.cpp,[.][0-9]{3})),.h,.hpp");

    if (mgr.prParsedFilters.size() != 4U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".cpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != ".h") return false;
    if (mgr.prParsedFilters[3].getFirstFilter() != ".hpp") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_8() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3})),.cpp,(([.][0-9]{3})) ,.h,.hpp");

    if (mgr.prParsedFilters.size() != 5U) return false;
    if (mgr.prParsedFilters[0].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[1].getFirstFilter() != ".cpp") return false;
    if (mgr.prParsedFilters[2].getFirstFilter() != "(([.][0-9]{3}))") return false;
    if (mgr.prParsedFilters[3].getFirstFilter() != ".h") return false;
    if (mgr.prParsedFilters[4].getFirstFilter() != ".hpp") return false;

    return true;
}

// must fail
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_9() {
    auto mgr = Test_IGFD_ParseFilters("(([.][0-9]{3})");

    if (mgr.prParsedFilters.size() != 0U) return false;

    return true;
}

// must fail
bool Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_10() {
    auto mgr = Test_IGFD_ParseFilters("([.][0-9]{3}))");

    if (mgr.prParsedFilters.size() != 0U) return false;

    return true;
}

#pragma endregion

#pragma region Filters Collection with Regex

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Regex_0() {
    auto mgr = Test_IGFD_ParseFilters("frames files{(([.][0-9]{3})),.frames}");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "frames files") return false;
    if (mgr.prParsedFilters[0].filters.find("(([.][0-9]{3}))") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".frames") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// invalid regex
bool Test_IGFD_FilterManager_ParseFilters_Filters_Regex_1() {
    auto mgr = Test_IGFD_ParseFilters("frames files{((.001,.NNN)),.frames}");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "frames files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].filters.find(".frames") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Regex_2() {
    auto mgr = Test_IGFD_ParseFilters("frames files(.frames){(([.][0-9]{3})),.frames}");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "frames files(.frames)") return false;
    if (mgr.prParsedFilters[0].filters.find("(([.][0-9]{3}))") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".frames") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Regex_3() {
    auto mgr = Test_IGFD_ParseFilters("frames files(.cpp,.hpp){(([.][0-9]{3})),.cpp,.hpp}");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "frames files(.cpp,.hpp)") return false;
    if (mgr.prParsedFilters[0].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[0].filters.find("(([.][0-9]{3}))") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".hpp") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

#pragma endregion

#pragma region Filters Divers Tests

/* Rules
0) a filter must have 2 chars mini and the first must be a .
1) a regex must be in (( and ))
2) a , will separate filters except if between a ( and )
3) name{filter1, filter2} is a spetial form for collection filters
3.1) the name can be composed of what you want except { and }
3.2) the filter can be a regex
4) the filters cannot integrate these chars '(' ')' '{' '}' except for a regex with respect to rule 1)
5) the filters cannot integrate a ','
6) all spaces in filters will be shrinked, the best you can do is avoid them
*/

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Divers_0() {
    auto mgr = Test_IGFD_ParseFilters(
        "\
All files{.*},\
Frames Format 0(.001,.NNN){(([.][0-9]{3}))},\
Frames Format 1(XXX.png){(([0-9]{3}.png))},\
Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},\
Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},\
.md\
");

    if (mgr.prParsedFilters.size() != 6U) return false;
    if (mgr.prParsedFilters[0].title != "All files") return false;
    if (mgr.prParsedFilters[0].filters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].filters.find("(([.].*))") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != "Frames Format 0(.001,.NNN)") return false;
    if (mgr.prParsedFilters[1].filters.size() != 1U) return false;
    if (mgr.prParsedFilters[1].filters.find("(([.][0-9]{3}))") == mgr.prParsedFilters[1].filters.end()) return false;
    if (mgr.prParsedFilters[2].title != "Frames Format 1(XXX.png)") return false;
    if (mgr.prParsedFilters[2].filters.size() != 1U) return false;
    if (mgr.prParsedFilters[2].filters.find("(([0-9]{3}.png))") == mgr.prParsedFilters[2].filters.end()) return false;
    if (mgr.prParsedFilters[3].title != "Source files (*.cpp *.h *.hpp)") return false;
    if (mgr.prParsedFilters[3].filters.size() != 3U) return false;
    if (mgr.prParsedFilters[3].filters.find(".cpp") == mgr.prParsedFilters[3].filters.end()) return false;
    if (mgr.prParsedFilters[3].filters.find(".h") == mgr.prParsedFilters[3].filters.end()) return false;
    if (mgr.prParsedFilters[3].filters.find(".hpp") == mgr.prParsedFilters[3].filters.end()) return false;
    if (mgr.prParsedFilters[4].title != "Image files (*.png *.gif *.jpg *.jpeg)") return false;
    if (mgr.prParsedFilters[4].filters.size() != 4U) return false;
    if (mgr.prParsedFilters[4].filters.find(".png") == mgr.prParsedFilters[4].filters.end()) return false;
    if (mgr.prParsedFilters[4].filters.find(".gif") == mgr.prParsedFilters[4].filters.end()) return false;
    if (mgr.prParsedFilters[4].filters.find(".jpg") == mgr.prParsedFilters[4].filters.end()) return false;
    if (mgr.prParsedFilters[4].filters.find(".jpeg") == mgr.prParsedFilters[4].filters.end()) return false;
    if (mgr.prParsedFilters[5].title != ".md") return false;
    if (mgr.prParsedFilters[5].filters.size() != 1U) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Divers_1() {
    auto mgr = Test_IGFD_ParseFilters("Regex Custom*.h{((Custom.+[.]h))}");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "Regex Custom*.h") return false;
    if (mgr.prParsedFilters[0].filters.find("((Custom.+[.]h))") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Divers_2() {
    auto mgr = Test_IGFD_ParseFilters("C++ File (*.cpp){.cpp}");

    if (mgr.prParsedFilters.size() != 1U) return false;
    if (mgr.prParsedFilters[0].title != "C++ File (*.cpp)") return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ParseFilters_Filters_Divers_3() {
    auto mgr = Test_IGFD_ParseFilters("C/C++ File (*.c *.cpp){.c,.cpp}, Header File (*.h){.h}");

    if (mgr.prParsedFilters.size() != 2U) return false;
    if (mgr.prParsedFilters[0].title != "C/C++ File (*.c *.cpp)") return false;
    if (mgr.prParsedFilters[0].filters.find(".c") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[0].filters.find(".cpp") == mgr.prParsedFilters[0].filters.end()) return false;
    if (mgr.prParsedFilters[1].title != " Header File (*.h)") return false;
    if (mgr.prParsedFilters[1].filters.find(".h") == mgr.prParsedFilters[1].filters.end()) return false;

    return true;
}
#pragma endregion

#pragma region ReplaceExtentionWithCurrentFilter

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_0() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".cpp", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto");
    if (res != "toto.cpp") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_1() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".cpp", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.cpp") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_2() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".code.cpp", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto");
    if (res != "toto.code.cpp") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_3() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".code.cpp", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.code.cpp") return false;

    return true;
}

// if regex, the function have no impact
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_4() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter("((.*\\.a\\.b))", true);
    auto res                          = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.c") return false;

    return true;
}

// many filter in the current collection, no change
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_5() {
    IGFD::FilterManager mgr;
    mgr.ParseFilters("C/C++ File (*.c,*.cpp){.c,.cpp}");
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.c") return false;

    return true;
}

// one filter in the current collection => change
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_6() {
    IGFD::FilterManager mgr;
    mgr.ParseFilters("C/C++ File (*.cpp){.cpp}");
    auto res = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.cpp") return false;

    return true;
}

// .* => no change
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_7() {
    IGFD::FilterManager mgr;
    mgr.ParseFilters(".*");
    auto res = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.c") return false;

    return true;
}

// one filter .* in one collection => no change
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_8() {
    IGFD::FilterManager mgr;
    mgr.ParseFilters("All files{.*}");
    auto res = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.c") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_9() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".cpp", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto.c.r.x");
    if (res != "toto.c.r.cpp") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_10() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".cpp.tv", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto.c.r.x");
    if (res != "toto.c.cpp.tv") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_11() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter("", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto.c.r.x");
    if (res != "toto.c.r.x") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_12() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".cpp.tv", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto.");
    if (res != "toto.cpp.tv") return false;

    return true;
}

// must be ok
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_13() {
    IGFD::FilterManager mgr;
    mgr.prSelectedFilter.addFilter(".cpp.tv", false);
    auto res                    = mgr.ReplaceExtentionWithCurrentFilter("toto");
    if (res != "toto.cpp.tv") return false;

    return true;
}

// if regex, the function have no impact
bool Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_14() {
    IGFD::FilterManager mgr;
    mgr.ParseFilters("C/C++ File{((.*\\.a\\.b))}");
    auto res                          = mgr.ReplaceExtentionWithCurrentFilter("toto.c");
    if (res != "toto.c") return false;

    return true;
}

#pragma endregion

#pragma region FilterInfos transformAsteriskBasedFilterToRegex

// must be ok
bool Test_IGFD_FilterManager_FilterInfos_transformAsteriskBasedFilterToRegex_0() {
    IGFD::FilterManager mgr;
    const auto& res = mgr.prSelectedFilter.transformAsteriskBasedFilterToRegex(".a.*");
    if (res != "(([.]a[.].*$))") return false;
    return true;
}

#pragma endregion

#pragma region FilterInfos IsCoveredByFilters

// must be ok
bool Test_IGFD_FilterManager_IsCoveredByFilters_0() {
    IGFD::FilterManager mgr;
    mgr.ParseFilters(".*");

    IGFD::FileInfos file;
    file.fileNameExt      = "toto.a.b";
    file.fileExtLevels[0] = ".a.b";

    if (!mgr.IsCoveredByFilters(file, false)) return false;

    return true;
}

bool Test_IGFD_FilterManager_IsCoveredByFilters_1() {
    IGFD::FilterManager mgr;
    mgr.ParseFilters("{.a.b, .b}");

    {
        IGFD::FileInfos file;
        file.fileNameExt      = "toto.a.b";
        file.fileExtLevels[0] = ".a.b";
        if (!mgr.IsCoveredByFilters(file, false)) return false;
    }
    {
        IGFD::FileInfos file;
        file.fileNameExt      = "toto.b";
        file.fileExtLevels[0] = ".b";
        if (!mgr.IsCoveredByFilters(file, false)) return false;
    }
    {
        IGFD::FileInfos file;
        file.fileNameExt      = "toto.c";
        file.fileExtLevels[0] = ".c";
        if (mgr.IsCoveredByFilters(file, false)) return false;
    }

    return true;
}

#pragma endregion

#pragma region Entry Point


#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

bool Test_FilterManager(const std::string& vTest) {
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_0);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_1);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_2);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_3);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_4);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_5);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_6);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_7);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_8);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_0);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_1);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_2);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_3);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_4);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_5);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_6);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_7);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_8);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_9);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_10);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_11);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_0);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_1);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_2);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_3);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_4);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_5);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_6);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_7);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_8);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_9);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Simple_Regex_10);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Regex_0);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Regex_1);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Regex_2);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Regex_3);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Divers_0);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Divers_1);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Divers_2);
    IfTestExist(Test_IGFD_FilterManager_ParseFilters_Filters_Divers_3);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_0);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_1);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_2);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_3);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_4);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_5);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_6);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_7);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_8);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_9);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_10);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_11);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_12);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_13);
    IfTestExist(Test_IGFD_FilterManager_ReplaceExtentionWithCurrentFilter_14);
    IfTestExist(Test_IGFD_FilterManager_FilterInfos_transformAsteriskBasedFilterToRegex_0);
    IfTestExist(Test_IGFD_FilterManager_IsCoveredByFilters_0);
    IfTestExist(Test_IGFD_FilterManager_IsCoveredByFilters_1);
    
    assert(0);

    return false;
}

#pragma endregion
