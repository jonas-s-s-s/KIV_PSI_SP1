#pragma once

#include <string>

// Table element
inline std::string TABLE(const std::string &content) {
    return "<table>" + content + "</table>";
}

// Table Heading
inline std::string TH(const std::string &content) {
    return "<th>" + content + "</th>";
}

// Table Row
inline std::string TR(const std::string &content) {
    return "<tr>" + content + "</tr>";
}

// Table Cell
inline std::string TD(const std::string &content) {
    return "<td>" + content + "</td>";
}

// Bold text
inline std::string B(const std::string &content) {
    return "<b>" + content + "</b>";
}

// HTML Paragraph
inline std::string P(const std::string &content) {
    return "<p>" + content + "</p>";
}

// HTML Heading (h1)
inline std::string H1(const std::string &content) {
    return "<h1>" + content + "</h1>";
}

// HTML Heading (h2)
inline std::string H2(const std::string &content) {
    return "<h2>" + content + "</h2>";
}

// HTML Heading (h3)
inline std::string H3(const std::string &content) {
    return "<h3>" + content + "</h3>";
}

// HTML Link
inline std::string A(const std::string &href, const std::string &content) {
    return "<a href=\"" + href + "\">" + content + "</a>";
}

// HTML Unordered List
inline std::string UL(const std::string &content) {
    return "<ul>" + content + "</ul>";
}

// HTML Ordered List
inline std::string OL(const std::string &content) {
    return "<ol>" + content + "</ol>";
}

// HTML List Item
inline std::string LI(const std::string &content) {
    return "<li>" + content + "</li>";
}

// HTML Div
inline std::string DIV(const std::string &content) {
    return "<div>" + content + "</div>";
}

// HTML Span
inline std::string SPAN(const std::string &content) {
    return "<span>" + content + "</span>";
}

// HTML Break Line
inline std::string BR() {
    return "<br />";
}

// HTML Horizontal Line
inline std::string HR() {
    return "<hr />";
}

constexpr std::string_view DEFAULT_STYLE_SHEET = "* {\n"
                                                 "  margin: 0;\n"
                                                 "  padding: 0;\n"
                                                 "  box-sizing: border-box;\n"
                                                 "}\n"
                                                 "\n"
                                                 "html {\n"
                                                 "  font-size: 100%; \n"
                                                 "  line-height: 1.6;\n"
                                                 "  font-family: Arial, sans-serif;\n"
                                                 "  scroll-behavior: smooth;\n"
                                                 "}\n"
                                                 "\n"
                                                 "body {\n"
                                                 "  margin: 0;\n"
                                                 "  padding: 20px;\n"
                                                 "  font-size: 1rem;\n"
                                                 "}\n"
                                                 "\n"
                                                 "h1, h2, h3, h4, h5, h6 {\n"
                                                 "  margin-bottom: 1rem;\n"
                                                 "}\n"
                                                 "\n"
                                                 "p {\n"
                                                 "  margin-bottom: 1.5rem;\n"
                                                 "}\n"
                                                 "\n"
                                                 "a {\n"
                                                 "  text-decoration: none;\n"
                                                 "}\n"
                                                 "\n"
                                                 "a:hover {\n"
                                                 "  text-decoration: underline;\n"
                                                 "}\n"
                                                 "\n"
                                                 "strong {\n"
                                                 "  font-weight: bold;\n"
                                                 "}\n"
                                                 "\n"
                                                 "em {\n"
                                                 "  font-style: italic;\n"
                                                 "}\n"
                                                 "\n"
                                                 "table {\n"
                                                 "  width: auto;\n"
                                                 "  border-collapse: collapse;\n"
                                                 "  margin-bottom: 1.5rem;\n"
                                                 "}\n"
                                                 "\n"
                                                 "table th,\n"
                                                 "table td {\n"
                                                 "  border: 1px solid #ccc;\n"
                                                 "  padding: 0.75rem;\n"
                                                 "  text-align: left;\n"
                                                 "}\n"
                                                 "\n"
                                                 "table th {\n"
                                                 "  background-color: #f2f2f2;\n"
                                                 "  font-weight: bold;\n"
                                                 "}\n"
                                                 "\n"
                                                 "table tbody tr:nth-child(even) {\n"
                                                 "  background-color: #f9f9f9;\n"
                                                 "}\n"
                                                 "\n"
                                                 "ul, ol {\n"
                                                 "  margin: 1rem 0;\n"
                                                 "  padding-left: 2rem;\n"
                                                 "}\n"
                                                 "\n"
                                                 "ul li, ol li {\n"
                                                 "  margin-bottom: 0.5rem;\n"
                                                 "}";

inline std::string DEFAULT_HTML_TEMPLATE(const std::string &content) {
    return "<html>\n"
           "<head>\n"
           "<style>\n"
           + std::string(DEFAULT_STYLE_SHEET) +
           "</style>\n"
           "</head>\n"
           "<body>\n"
           + content +
           "</body>\n"
           "</html>";
}
