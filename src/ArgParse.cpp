#include "../include/ArgParse.hpp"


#include <iostream>
#include <sstream>

#include <algorithm>
#include <string>
#include <set>

#include <stdexcept>

#include <stdio.h>



ArgParse::ArgParse(const std::string description,
                   const std::string epilog,
                   const bool addHelp)
  : m_description(description)
  , m_epilog(epilog)
  , m_programName()
  , m_params()
{
  if (addHelp)
    addArgument("-h,--help", "Prints this help message");
}


void ArgParse::addArgument(const std::string arg,
                           const std::string help,
                           const ValueType type,
                           const ValueRequired valueRequired,
                           const std::string valueName,
                           const std::string choices)
{

  Argument argument(help, type, valueRequired, typeToString(type, valueName), choices, "");
  m_params.insert(std::pair<std::string, Argument>(arg, argument));
}


bool ArgParse::parseArgs(const int argc,
                         const char* argv[])
{
  std::list<std::string> argList;
  for (int i = 0; i < argc; ++i )
    argList.push_back(argv[i]);

  return parseArgs(argList);
}


void ArgParse::parseArgs(const std::list<std::string> argList)
{
  m_programName = argList.front();

  // the wrok.
  std::list<std::string>::const_iterator it = argList.begin();
  for (++it; it != argList.end(); ++it ) {

    if ( (*it).at(0) != '-' )
      throw std::runtime_error(std::string(*it).
                               append(" shall start with a dash."));

    // inspect each arument
    ArgMap::iterator it2 = findElement(*it);
    if ( it2 == m_params.end() )
      throw std::runtime_error(std::string(*it).append(" is not known."));

    if ( (*it2).second.m_found )
      throw std::runtime_error(std::string(*it).
                               append(" has been given before."));

    (*it2).second.m_found = true;

    if ( (*it2).second.m_type == NONE )
      continue;

    std::list<std::string>::const_iterator next = it;
    next++;

    if ( next == argList.end() ) {
      if ( (*it2).second.m_valueRequired == REQUIRED )
        throw std::runtime_error(std::string(*it).
                                 append(" requires a parameter."));

      if ( (*it2).second.m_valueRequired == OPTIONAL )
        continue;
    }

    if ( (*it2).second.m_valueRequired == OPTIONAL &&
          findElement( *next ) != m_params.end() )
      continue;

      switch ( (*it2).second.m_type ) {
        case INT : {
          int temp;
          if ( sscanf( next->c_str(), "%d", &temp ) == 0 )
            throw std::runtime_error(std::string( *next ).
                    append(" is not an integer, required by ").append( *it ));

          if ( !(*it2).second.m_choices.empty() ) {
            int lowerBound;
            int upperBound;
            if ( sscanf( (*it2).second.m_choices.c_str(),
                 "%d..%d", &lowerBound, &upperBound ) != 2 )
              throw std::logic_error(std::string( *it ).
                              append(" has syntax error. ").
                              append("Range expected in a INT..INT format" ));

            if ( temp < lowerBound || temp > upperBound )
              throw std::runtime_error(std::string( *it ).
                            append( " expects an integer in the range of {" ).
                            append( (*it2).second.m_choices).
                            append("}") );
          }

          break;
        }
        case DOUBLE : {
          double temp;
          if ( sscanf( next->c_str(), "%f", &temp ) == 0 )
            throw std::runtime_error(std::string( *next ).
                    append(" is not a double, required by ").append(*it));

          if ( !(*it2).second.m_choices.empty() ) {
            double lowerBound;
            double upperBound;
            if ( sscanf( (*it2).second.m_choices.c_str(),
                 "%f..%f", &lowerBound, &upperBound ) != 2 )
              throw std::logic_error(std::string( *it ).
                      append(" has syntax error. ").
                      append("Range expected in a DOUBLE..DOUBLE format" ));

            if ( temp < lowerBound || temp > upperBound )
              throw std::runtime_error(std::string( *it ).
                             append( " expects a double in the range of [" ).
                             append( (*it2).second.m_choices).
                             append("}") );
          }

          break;
        }
        case BOOL : {
          std::string temp = *next;
          std::transform(temp.begin(), temp.end(),temp.begin(), ::toupper);
          if ( temp != "TRUE" && temp != "FALSE" )
            throw std::runtime_error(std::string( *next ).
                    append(" is not a boolean, required by ").append(*it));

          if ( !(*it2).second.m_choices.empty() )
            throw std::logic_error(std::string( *next ).
                    append(" expects a boolean not choices."));

          break;
        }
        case STRING : {
          if ( !(*it2).second.m_choices.empty() ) {

            std::set<std::string> choices =
                          parseCommaSepStringToSet( (*it2).second.m_choices );

            if ( choices.find( *next ) == choices.end() )
              throw std::runtime_error(std::string( *next ).
                    append(" is not in the expected list of choices: {").
                    append( (*it2).second.m_choices ).
                    append("}"));
          }

          break;
        }

        default:
          break;
      }


    (*it2).second.m_value = *next;
    (*it2).second.m_valueHasBeenSet = true;
    ++it;
  }
}


void ArgParse::isArg(const std::string arg) const
{
  ArgMap::const_iterator it = m_params.find(arg);
  it != m_params.end();
}


bool ArgParse::foundArg(const std::string arg) const
{
  ArgMap::const_iterator it = m_params.find(arg);
  return it != m_params.end() && (*it).second.m_found == true;
}

bool ArgParse::argHasValue(const std::string arg) const
{
  ArgMap::const_iterator it = m_params.find(arg);
  return it != m_params.end() &&
         (*it).second.m_found == true &&
         (*it).second.m_valueHasBeenSet;
}


bool ArgParse::argAsString(const std::string arg, std::string &value) const
{
  if ( !argHasValue(arg) )
    return false;

  ArgMap::const_iterator it = m_params.find(arg);
  value = (*it).second.m_value;
  return true;
}


bool ArgParse::argAsInt(const std::string arg, int &value) const
{
  if ( !argHasValue(arg) )
    return false;

  ArgMap::const_iterator it = m_params.find(arg);
  value = atoi((*it).second.m_value.c_str());
  return true;
}


bool ArgParse::argAsDouble(const std::string arg, double &value) const
{
  if ( !argHasValue(arg) )
    return false;

  ArgMap::const_iterator it = m_params.find(arg);
  value = atof((*it).second.m_value.c_str());
  return true;
}


bool ArgParse::argAsBool(const std::string arg, bool &value) const
{
  if ( !argHasValue(arg) )
    return false;

  ArgMap::const_iterator it = m_params.find(arg);

  std::string temp = (*it).second.m_value;
  std::transform(temp.begin(), temp.end(),temp.begin(), ::toupper);

  value = temp == "TRUE";
  return true;
}


std::string ArgParse::usage() const
{
  std::stringstream ss;

  ss << m_description << std::endl << std::endl;

  ss << "usage: " << m_programName;
  if (!m_params.empty()) {
    ss << " [OPTION]" << std::endl << std::endl;
    ss << "Options:" << std::endl;
    int length;
    ArgMap::const_iterator it;
    for ( it = m_params.begin(); it != m_params.end(); it++ ) {
      ss << (*it).first;
      length = (*it).first.length();
      if ( (*it).second.m_type != NONE ) {

        ss << " ";
        length++;

        if ( (*it).second.m_valueRequired == OPTIONAL ) {
          ss << "[";
          length ++;
        }

        if ( !(*it).second.m_choices.empty() ) {
          ss << "{" << (*it).second.m_choices << "}";
          length += (*it).second.m_choices.length() + 2;
        } else {
          ss << (*it).second.m_valueName;
          length += (*it).second.m_valueName.length();
        }

        if ( (*it).second.m_valueRequired == OPTIONAL ) {
          ss << "]";
          length++;
        }
      }
      ss << std::string(30-length, ' ');
      ss << (*it).second.m_help << std::endl;
    }
  }
  ss << std::endl;
  ss << m_epilog << std::endl;

  return ss.str();;
}


ArgParse::Argument::Argument (const std::string help,
                              const enum ValueType type,
                              const enum ValueRequired valueRequired,
                              const std::string valueName,
                              const std::string choices,
                              const std::string value)
  : m_help(help)
  , m_type(type)
  , m_valueRequired(valueRequired)
  , m_valueName(valueName)
  , m_choices(choices)
  , m_value(value)
  , m_found(false)
  , m_valueHasBeenSet(false)
{

}


bool
ArgParse::argCompare::operator()(const std::string a,const std::string b) const
{
  if ( a.at(1) == '-' && b.at(1) != '-' )
    return a.substr(1) < b;

  if ( b.at(1) == '-' && a.at(1) != '-' )
    return a < b.substr(1);

  return a<b;
}


std::map<std::string, ArgParse::Argument>::iterator
ArgParse::findElement(const std::string param)
{
  ArgMap::iterator it;
  for( it = m_params.begin(); it != m_params.end(); ++it) {

    // if it's the short param at the beginning
    if ( (*it).first.find(param) == 0 )
      return it;

    // or is it the long after the comma?
    size_t commaPos = (*it).first.find(",");
    if ( commaPos != std::string::npos &&
         (*it).first.find( param, commaPos+1 ) != std::string::npos )
      return it;
  }

  return m_params.end();
}

std::set<std::string>
ArgParse::parseCommaSepStringToSet(const std::string s) const
{
  std::string tmp(s);
  std::set<std::string> stringSet;

  size_t pos;
  std::string element;

  while ( !tmp.empty() ) {
    pos = tmp.find(',');
    if (pos == std::string::npos) {
      element = tmp;
      tmp = "";
    } else {
      element = tmp.substr(0,pos);
      tmp = tmp.substr(pos+1);
    }

    // if ( element.empty() ) ... ?

    if ( stringSet.find(element) != stringSet.end() ) {
      throw std::logic_error( std::string( element ).
                              append(" listed twice in ").append(s) );
    }

    stringSet.insert(element);
  }

  return stringSet;
}

std::string
ArgParse::typeToString(const ValueType type, std::string valueName) const
{
  if ( type != NONE && valueName.empty() ) {
    switch ( type ) {
      case INT :
        return "INT";
      case DOUBLE :
        return "DOUBLE";
        break;
      case BOOL :
         return "BOOL";
        break;
      default:
        return "";
    }
  }
  return valueName;
}