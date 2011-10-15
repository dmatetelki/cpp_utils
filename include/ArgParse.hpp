#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

#include <string>
#include <list>
#include <map>
#include <stdexcept>

// Aims for the functionality of Python argparse
// http://docs.python.org/library/argparse.html#module-argparse
class ArgParse
{

public:

  enum ValueType {
    TYPE_NONE,
    TYPE_STRING,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOL
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
                   const enum ValueType type = TYPE_NONE,
                   const enum ValueRequired valueRequired = REQUIRED,
                   const std::string valueName = std::string(""),
                   const std::string choices = std::string("")); // "yes,no" or range: [1..100]


  bool parseArgs(const int argc,
                 const char* argv[]) throw(std::runtime_error);

  bool parseArgs(const std::list<std::string> argList) throw(std::runtime_error);

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
              const enum ValueType type = TYPE_NONE,
              const enum ValueRequired valueRequired = REQUIRED,
              const std::string valueName = std::string(""),
              const std::string choices = std::string(""),
              const std::string value = std::string(""))
    : m_help(help)
    , m_type(type)
    , m_valueRequired(valueRequired)
    , m_valueName(valueName)
    , m_choices(choices)
    , m_value(value)
    , m_found(false)
    , m_valueHasBeenSet(false) {};
  };

  // arg is just the shor or long form: "-h" or "--help"
  std::map<std::string, Argument>::iterator findElement(const std::string param);


  std::string m_description;
  std::string m_epilog;
  std::string m_programName;

  std::map<std::string, Argument> m_params;


}; // class ArgParse


#endif // ARGPARSE_HPP
