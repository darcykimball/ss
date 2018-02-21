#ifndef PARSER_HPP
#define PARSER_HPP


#include <iostream>

#include <boost/spirit/include/qi.hpp>

#include "http.hpp"


namespace http {


// Parse an HTTP/1.0 request
//template <typename Iterator>
//struct request_grammar
//  : qi::grammar<Iterator, mini_xml(), qi::locals<std::string>, ascii::space_type>
//{
//    mini_xml_grammar()
//      : mini_xml_grammar::base_type(xml)
//    {
//        using qi::lit;
//        using qi::lexeme;
//        using ascii::char_;
//        using ascii::string;
//        using namespace qi::labels;
//
//        text %= lexeme[+(char_ - '<')];
//        node %= xml | text;
//
//        start_tag %=
//                '<'
//            >>  !lit('/')
//            >>  lexeme[+(char_ - '>')]
//            >>  '>'
//        ;
//
//        end_tag =
//                "</"
//            >>  string(_r1)
//            >>  '>'
//        ;
//
//        xml %=
//                start_tag[_a = _1]
//            >>  *node
//            >>  end_tag(_a)
//        ;
//    }
//
//    qi::rule<Iterator, mini_xml(), qi::locals<std::string>, ascii::space_type> xml;
//    qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
//    qi::rule<Iterator, std::string(), ascii::space_type> text;
//    qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
//    qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
//};


} // namespace http


#endif // PARSER_HPP
