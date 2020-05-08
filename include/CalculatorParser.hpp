#ifndef EQUITATION_PARSER_HPP
#define EQUITATION_PARSER_HPP

#include <string>
#include <iostream>
#include <vector>
#include <boost/spirit/include/qi.hpp>
// #include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/phoenix/bind/bind_function.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/optional/optional_io.hpp>

namespace qi     = boost::spirit::qi;
namespace spirit = boost::spirit;
namespace phx    = boost::phoenix;

using calc_rule = qi::rule<std::string::iterator, double(std::vector<double> &), spirit::locals<unsigned int>, qi::space_type>;
using grammar_rule = qi::grammar<std::string::iterator, double(std::vector<double> &), spirit::locals<unsigned int>, qi::space_type>;

struct CalculatorParser : grammar_rule
{
    calc_rule expression, term, factor, variable;

    CalculatorParser() : CalculatorParser::base_type(expression)
    {
        using namespace boost::spirit::qi;

        expression =
           -(term(_r1)[_val = _1]
                >> *(
                    (char_('+') >> term(_r1)[_val += _1]) |
                        (char_('-') >> term(_r1)[_val -= _1])
                    )
            );

        term =
                factor(_r1)[_val = _1]
                >> *(
                        (char_('*') >> factor(_r1)[_val *= _1]) |
                        (char_('/') >> factor(_r1)[_val /= _1])
                    );

        factor =
                double_[_val = _1]
                | (char_('(') >> expression(_r1)[_val = _1] >> char_(')'))
                | (char_('-') >> factor(_r1)[_val = -_1])
                | (char_('+') >> factor(_r1)[_val = _1])
                | variable(_r1)[_val = _1];

        variable = lexeme[ char_('$') >> uint_[_pass = _1 < 10, _a = _1] ]
                    [_val = _r1[_a]];

    }
};

#endif // EQUITATION_PARSER_HPP
