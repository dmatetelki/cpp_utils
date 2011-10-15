#include "../include/ArgParse.hpp"


#include <iostream>
#include <sstream>

#include <algorithm>
#include <string>
#include <stdio.h>


ArgParse::ArgParse(const std::string description,
                   const std::string epilog,
                   const bool add_Help)
  : m_description(description)
  , m_epilog(epilog)
  , m_programName()
  , m_params()
{
  if (add_Help)
    addArgument("-h,--help", "Prints this help message");
}


void ArgParse::addArgument(const std::string arg,
                           const std::string help,
                           const ValueType type,
                           const ValueRequired valueRequired,
                           const std::string valueName,
                           const std::string choices)
{
  Argument argument(help, type, valueRequired, valueName, choices, "");
  m_params.insert(std::pair<std::string, Argument>(arg, argument));
}


bool ArgParse::parseArgs(const int argc,
                         const char* argv[]) throw(std::runtime_error)
{
  std::list<std::string> argList;
  for (int i = 0; i < argc; ++i )
    argList.push_back(argv[i]);

  return parseArgs(argList);
}


bool ArgParse::parseArgs(const std::list<std::string> argList) throw(std::runtime_error)
{
  m_programName = argList.front();

  // the wrok.
  std::list<std::string>::const_iterator it = argList.begin();
  for (++it; it != argList.end(); ++it ) {

    if ( (*it).at(0) != '-' )
      throw std::runtime_error(std::string(*it).append(" shall start with a dash."));

    // inspect each arument
    std::map<std::string, Argument>::iterator it2 = findElement(*it);
    if ( it2 == m_params.end() )
      throw std::runtime_error(std::string(*it).append(" is not known."));

    if ( (*it2).second.m_found )
      throw std::runtime_error(std::string(*it).append(" has been given before."));

    (*it2).second.m_found = true;

    if ( (*it2).second.m_type == TYPE_NONE )
      continue;

    std::list<std::string>::const_iterator next = it;
    next++;

    if ( next == argList.end() ) {
      if ( (*it2).second.m_valueRequired == REQUIRED )
        throw std::runtime_error(std::string(*it).append(" requires a parameter."));

      if ( (*it2).second.m_valueRequired == OPTIONAL )
        continue;
    }

    if ( (*it2).second.m_valueRequired == OPTIONAL && findElement( *next ) != m_params.end() )
      continue;

    switch ( (*it2).second.m_type ) {
      case TYPE_INT : {
        int temp;
        if ( sscanf( next->c_str(), "%d", &temp ) == 0 )
          throw std::runtime_error(std::string( *next ).
                  append(" is not an integer, required by ").append(*it));
        break;
      }
      case TYPE_DOUBLE : {
        double temp;
        if ( sscanf( next->c_str(), "%f", &temp ) == 0 )
          throw std::runtime_error(std::string( *next ).
                  append(" is not a double, required by ").append(*it));
        break;
      }
      case TYPE_BOOL : {
        std::string temp = *next;
        std::transform(temp.begin(), temp.end(),temp.begin(), ::toupper);
        if ( temp != "TRUE" && temp != "FALSE" )
          throw std::runtime_error(std::string( *next ).
                  append(" is not a boolean, required by ").append(*it));
        break;
      }
      default:
        break;
    }

    (*it2).second.m_value = *next;
    (*it2).second.m_valueHasBeenSet = true;
    ++it;
  }
  return true;
}


bool ArgParse::isArg(const std::string arg) const
{
  std::map<std::string, Argument>::const_iterator it = m_params.find(arg);
  return it != m_params.end();
}


bool ArgParse::foundArg(const std::string arg) const
{
  std::map<std::string, Argument>::const_iterator it = m_params.find(arg);
  return it != m_params.end() && (*it).second.m_found == true;
}

bool ArgParse::argHasValue(const std::string arg) const
{
  std::map<std::string, Argument>::const_iterator it = m_params.find(arg);
  return it != m_params.end() &&
         (*it).second.m_found == true &&
         (*it).second.m_valueHasBeenSet;
}


bool ArgParse::argAsString(const std::string arg, std::string &value) const
{
  if ( !argHasValue(arg) )
    return false;

  std::map<std::string, Argument>::const_iterator it = m_params.find(arg);
  value = (*it).second.m_value;
  return true;
}


bool ArgParse::argAsInt(const std::string arg, int &value) const
{
  if ( !argHasValue(arg) )
    return false;

  std::map<std::string, Argument>::const_iterator it = m_params.find(arg);
  value = atoi((*it).second.m_value.c_str());
  return true;
}


bool ArgParse::argAsDouble(const std::string arg, double &value) const
{
  if ( !argHasValue(arg) )
    return false;

  std::map<std::string, Argument>::const_iterator it = m_params.find(arg);
  value = atof((*it).second.m_value.c_str());
  return true;
}


bool ArgParse::argAsBool(const std::string arg, bool &value) const
{
  if ( !argHasValue(arg) )
    return false;

  std::map<std::string, Argument>::const_iterator it = m_params.find(arg);

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
    std::map<std::string, Argument>::const_iterator it;
    for ( it = m_params.begin(); it != m_params.end(); it++ ) {
      ss << (*it).first;
      length = (*it).first.length();
      if ( (*it).second.m_type != TYPE_NONE ) {
        if ((*it).second.m_valueRequired == REQUIRED ) {
          ss << "=" << (*it).second.m_valueName;
          length += (*it).second.m_valueName.length()+1;
        } else {
          ss << "[=" << (*it).second.m_valueName << "]";
          length += (*it).second.m_valueName.length() + 3;
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

std::map<std::string, ArgParse::Argument>::iterator
ArgParse::findElement(const std::string param)
{
  std::map<std::string, Argument>::iterator it;
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