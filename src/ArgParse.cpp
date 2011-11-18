#include "ArgParse.hpp"

#include "Common.hpp"

#include <iostream>
#include <sstream>

#include <algorithm>
#include <string>
#include <set>

#include <stdexcept>

#include <cstdio> // sscan
#include <cctype>


ArgParse::ArgParse(const std::string description,
                   const std::string epilog,
                   const bool addHelp)
  : m_description(description)
  , m_epilog(epilog)
  , m_programName()
  , m_params()
{
  if (addHelp)
    addArgument("-h, --help", "Prints this help message");
}


void ArgParse::addArgument(const std::string arg,
                           const std::string help,
                           const ValueType valueType,
                           const Required valueRequired,
                           const Required argRequired,
                           const std::string valueName,
                           const std::string choices)
{
  if ( arg.at(0) != '-' )
    throw std::logic_error(std::string(arg).
                               append(" shall start with a dash."));

  if ( findKeyinArgMap(arg) != m_params.end() )
    throw std::logic_error(std::string(arg).
                               append(" has been given before."));

  int i;
  if ( valueType == INT &&
       !choices.empty() &&
       sscanf( choices.c_str(), "%d..%d", &i, &i ) != 2 )
           throw std::logic_error(std::string( arg ).
                           append(" has syntax error. ").
                           append("Range expected in a INT..INT format" ));

  float f;
  if ( valueType == FLOAT &&
       !choices.empty() &&
       sscanf( choices.c_str(), "%f..%f", &f, &f ) != 2 )
           throw std::logic_error(std::string( arg ).
                           append(" has syntax error. ").
                           append("Range expected in a FLOAT..FLOAT format" ));

  Argument argument(help,
                    valueType,
                    valueRequired,
                    argRequired,
                    typeToString(valueType, valueName),
                    choices,
                    "");
  m_params.insert(std::pair<std::string, Argument>(arg, argument));
}


void ArgParse::parseArgs(int argc,
                         char* argv[])
{
  std::list<std::string> argList;
  for (int i = 0; i < argc; ++i )
    argList.push_back(argv[i]);

  parseArgs(argList);
}


void ArgParse::parseArgs(const std::list<std::string> argList)
{
  m_programName = argList.front();

  std::list<std::string>::const_iterator it = argList.begin();
  for (++it; it != argList.end(); ++it ) {

    ArgMap::iterator argMapIt = findKeyinArgMap(*it);
    if ( argMapIt == m_params.end() )
      throw std::runtime_error(std::string(*it).append(" is not known."));

    (*argMapIt).second.m_found = true;

    if ( (*argMapIt).second.m_type == NONE )
      continue;

    std::list<std::string>::const_iterator next = it;
    next++;

    if ( next == argList.end() ) {
      if ( (*argMapIt).second.m_valueRequired == REQUIRED )
        throw std::runtime_error(std::string(*it).
                                 append(" requires a parameter."));

      if ( (*argMapIt).second.m_valueRequired == OPTIONAL )
        continue;
    }

    if ( (*argMapIt).second.m_valueRequired == OPTIONAL &&
          findKeyinArgMap( *next ) != m_params.end() )
      continue;

    validateValue( (*argMapIt).second.m_type,
                   (*argMapIt).first,
                   (*argMapIt).second.m_choices,
                   *next );

    (*argMapIt).second.m_value = *next;
    (*argMapIt).second.m_valueHasBeenSet = true;
    ++it;
  }

  checkRequiredArgsFound();
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


bool ArgParse::argAsFloat(const std::string arg, float &value) const
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

  bool oprionals = thereAreOptionalArgs();
  bool required = thereAreRequiredArgs();
  ss << "usage: " << m_programName
     << (required ? " <ARGS>" : "")
     << (oprionals ? " [OPTIONS]" : "")
     << std::endl;

  if ( required ) {
    ss << std::endl << "Required arguments:" << std::endl;
    ss << printArgs(REQUIRED);
  }
  if ( oprionals ) {
    ss << std::endl << "Options:" << std::endl;
    ss << printArgs(OPTIONAL);
  }

  ss << std::endl;
  ss << m_epilog;

  return ss.str();;
}


bool ArgParse::thereAreOptionalArgs() const
{
  for (ArgMap::const_iterator it = m_params.begin(); it != m_params.end(); ++it )
    if ( (*it).second.m_argRequired == OPTIONAL )
      return true;

  return false;
}


bool ArgParse::thereAreRequiredArgs() const
{
  for (ArgMap::const_iterator it = m_params.begin(); it != m_params.end(); ++it )
    if ( (*it).second.m_argRequired == REQUIRED )
      return true;

  return false;
}


std::string
ArgParse::printArgs(const Required argRequired) const
{
  int length;
  std::string ret("");

  ArgMap::const_iterator it;
  for ( it = m_params.begin(); it != m_params.end(); it++ ) {
    if ( (*it).second.m_argRequired != argRequired )
      continue;

    ret.append( (*it).first );
    length = (*it).first.length();
    if ( (*it).second.m_type != NONE ) {

      ret.append( " " );
      length++;

      if ( (*it).second.m_valueRequired == OPTIONAL ) {
        ret.append( "[" );
        length ++;
      }

      if ( !(*it).second.m_choices.empty() ) {
        ret.append( "{" ).append( (*it).second.m_choices).append( "}" );
        length += (*it).second.m_choices.length() + 2;
      } else {
        ret.append( (*it).second.m_valueName );
        length += (*it).second.m_valueName.length();
      }

      if ( (*it).second.m_valueRequired == OPTIONAL ) {
        ret.append( "]" );
        length++;
      }
    }

    ret.append( std::string( length < 30 ? 30-length : 2, ' ') );
    ret.append( (*it).second.m_help );
    ret.append("\n");
  }

    return ret;
}


void ArgParse::checkRequiredArgsFound() const
{
  ArgMap::const_iterator it;

  std::string missed;
  for ( it = m_params.begin(); it != m_params.end(); ++it )
    if ( (*it).second.m_argRequired && !(*it).second.m_found )
      missed.append((*it).first).append("\n");

  if ( !missed.empty() )
    throw std::runtime_error(
      std::string("Required argument(s) missing: \n").append(missed));
}


void ArgParse::validateValue(const ArgParse::ValueType type,
                             const std::string name,
                             const std::string choices,
                             const std::string value) const
{
  switch ( type ) {
     case INT :
       validateInt(name, choices, value);
       break;
     case FLOAT :
       validateFloat(name, choices, value);
       break;
     case BOOL :
       validateBool(name, choices, value);
       break;
     case STRING :
       validateString(name, choices, value);
       break;
     default:
       break;
   }
}


void ArgParse::validateString( const std::string name,
                               const std::string choices,
                               const std::string value) const
{
  if ( !choices.empty() ) {

    std::set<std::string> choicesSet = choicesStringToSet( choices );

    if ( choicesSet.find( value ) == choicesSet.end() )
      throw std::runtime_error(std::string( value ).
            append(" is not in the expected list of choices: {").
            append( choices ).
            append("}, required by ").
            append( name ));
  }
}


void ArgParse::validateInt( const std::string name,
                            const std::string choices,
                            const std::string value) const
{
  int temp;
  if ( sscanf( value.c_str(), "%d", &temp ) == 0 )
    throw std::runtime_error(std::string( value ).
            append(" is not an integer, required by ").append( name ));

  if ( !choices.empty() ) {
    int lowerBound;
    int upperBound;
    sscanf( choices.c_str(), "%d..%d", &lowerBound, &upperBound );

    if ( temp < lowerBound || temp > upperBound )
      throw std::runtime_error(std::string( name ).
                    append( " expects an integer in the range of {" ).
                    append( choices).
                    append("}") );
  }
}


void ArgParse::validateFloat( const std::string name,
                              const std::string choices,
                              const std::string value) const
{
  float temp;
  if ( sscanf( value.c_str(), "%f", &temp ) == 0 )
    throw std::runtime_error(std::string( value ).
            append(" is not a float, required by ").append( name ));

  if ( !choices.empty() ) {
    float lowerBound;
    float upperBound;
    sscanf( choices.c_str(), "%f..%f", &lowerBound, &upperBound );

    if ( temp < lowerBound || temp > upperBound )
      throw std::runtime_error(std::string( name ).
                     append( " expects a float in the range of [" ).
                     append( choices).
                     append("}") );
  }
}


void ArgParse::validateBool( const std::string name,
                             const std::string choices,
                             const std::string value) const
{
  std::string temp = value;
  std::transform(temp.begin(), temp.end(),temp.begin(), ::toupper);
  if ( temp != "TRUE" && temp != "FALSE" )
    throw std::runtime_error(std::string( value ).
            append(" is not a boolean, required by ").append( name ));

  if ( !choices.empty() )
    throw std::logic_error(std::string( value ).
            append(" expects a boolean not choices."));
}


ArgParse::Argument::Argument (const std::string help,
                              const ValueType type,
                              const Required valueRequired,
                              const Required argRequired,
                              const std::string valueName,
                              const std::string choices,
                              const std::string value)
  : m_help(help)
  , m_type(type)
  , m_valueRequired(valueRequired)
  , m_argRequired(argRequired)
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
ArgParse::findKeyinArgMap(const std::string param)
{
  for( ArgMap::iterator it = m_params.begin(); it != m_params.end(); ++it) {

    std::string first;
    std::string second;

    size_t commaPos = (*it).first.find(",");
    if ( commaPos == std::string::npos ) {
      first = (*it).first;
    } else {
      first = (*it).first.substr(0, commaPos);
      second = (*it).first.substr(commaPos+1);
    }

    first.erase(std::remove_if(first.begin(), first.end(), isspace), first.end());
    second.erase(std::remove_if(second.begin(), second.end(), isspace), second.end());

    if ( param == first || param == second )
      return it;
  }

  return m_params.end();
}

std::set<std::string>
ArgParse::choicesStringToSet(const std::string s) const
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
  if ( !valueName.empty() )
    return valueName;

  switch ( type ) {
    case NONE :
      return "NONE";
    case STRING :
      return "STRING";
    case INT :
      return "INT";
    case FLOAT :
      return "DOUBLE";
    case BOOL :
       return "BOOL";
    default:
      return "";
  }
}