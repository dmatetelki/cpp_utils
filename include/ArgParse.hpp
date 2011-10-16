#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

#include <string>
#include <list>
#include <map>
#include <set>

// Aims for the functionality of Python argparse
// http://docs.python.org/library/argparse.html#module-argparse
class ArgParse
{

public:

  enum ValueType {
    NONE,
    STRING,
    INT,
    DOUBLE,
    BOOL
  };

  enum ValueRequired {
    OPTIONAL,
    REQUIRED
  };

  ArgParse(const std::string description,
           const std::string epilog = std::string(""),
           const bool add_Help = true);

  void addArgument(const std::string name,  // "-f,--foo"
                   const std::string help,
                   const enum ValueType type = NONE,
                   const enum ValueRequired valueRequired = REQUIRED,
                   const std::string valueName = std::string(""),
                   const std::string choices = std::string("")); // {"yes,no"} or range: {1..100}

  // throw(std::runtime_error)
  bool parseArgs(const int argc,
                 const char* argv[]);
  bool parseArgs(const std::list<std::string> argList);

  // is this arg in the map of understood arguments?
  bool isArg(const std::string arg) const;
  // is this argument passed as a command line parameter?
  bool foundArg(const std::string arg) const;
  // argument is passed as a command line parameter, but has a value?
  bool argHasValue(const std::string arg) const;

  // return true is arg exists and the arg in &value
  // arg need to be the same string as in addArgument ( "-h,--help" )
  bool argAsString(const std::string arg, std::string &value) const;
  bool argAsInt(const std::string arg, int &value) const;
  bool argAsDouble(const std::string arg, double &value) const;
  bool argAsBool(const std::string arg, bool &value) const;

  std::string usage() const;


private:

  struct Argument {
    const std::string m_help;
    const enum ValueType m_type;
    const enum ValueRequired m_valueRequired;
    const std::string m_valueName;
    const std::string m_choices;
    std::string m_value;
    bool m_found;
    bool m_valueHasBeenSet;

    Argument (const std::string help,
              const enum ValueType type = NONE,
              const enum ValueRequired valueRequired = REQUIRED,
              const std::string valueName = std::string(""),
              const std::string choices = std::string(""),
              const std::string value = std::string(""));
    };

  class argCompare {
   public:
     // short and long arg shall be compared with same amount of dashes
      bool operator()(const std::string a,const std::string b) const;
  };

  typedef std::map<std::string, Argument, argCompare> ArgMap;

  // arg is just the shor or long form: "-h" or "--help"
  ArgMap::iterator findElement(const std::string param);
  std::set<std::string> parseCommaSepStringToSet(const std::string s) const;
  std::string typeToString(const ValueType type, const std::string valueName) const;


  std::string m_description;
  std::string m_epilog;
  std::string m_programName;

  ArgMap m_params;


}; // class ArgParse


#endif // ARGPARSE_HPP
