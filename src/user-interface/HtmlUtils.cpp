#include "HtmlUtils.h"

constexpr const char* TABLE(const std::string& content)
{
    return ("<table>" + content + "</table>").c_str();
}

constexpr char*  TH(const std::string& content)
{
    return "<th>" + content + "</th>";
}

constexpr char*  TR(const std::string& content)
{
    return "<tr>" + content + "</tr>";
}

constexpr char*  TD(const std::string& content)
{
    return "<td>" + content + "</td>";
}

constexpr char*  B(const std::string& content)
{
    return "<b>" + content + "</b>";
}
