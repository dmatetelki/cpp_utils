#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

#include <string>
#include <list>
#include <map>
#include <set>

/** @brief Aim to achieve the functionality of Python argparse
 *
 * http://docs.python.org/library/argparse.html#module-argparse
 */
class ArgParse
{

public:

  enum ValueType {
    NONE,
    STRING,
    INT,
    FLOAT,
    BOOL
  };

  enum Required {
    OPTIONAL,
    REQUIRED
  };

  /** @param description Exmplanation of the purpose of the program,
   *  before the usage lines.
   *  @param epilog Lines after the usage and options. Usually contact e-mail.
   *  @param addHelp Add a "-h,--help" option.
   */
  ArgParse(const std::string description,
           const std::string epilog = std::string(""),
           const bool addHelp = true);

  /** @brief Adds an argument which the object will accept.
   *
   * @param name short and/or long form: "-f,--foo"
   * The value can be retreived with this string passed to the argAs... functions.
   * @param help Description of the argument, printed when --help is given.
   * @param valueType Type of the paramterer, required by the argument.
   * @param valueRequired Parameter requiered/optional after the argument.
   * @param argRequired Argument is required or optional.
   * It's a bad practice to have a required argument,
   * "options", shall be optional.
   * @param valueName Default is the type. But some short text can be better.
   * @param choices Comma separeted list of strings without whitespaces:
   * "yes,no,maybe"
   * or a range accepted numbers: "INT..INT", "DOUBLE..DOUBLE"
   */
  void addArgument(const std::string name,
                   const std::string help,
                   const ValueType valueType = NONE,
                   const Required valueRequired = REQUIRED,
                   const Required argRequired = OPTIONAL,
                   const std::string valueName = std::string(""),
                   const std::string choices = std::string(""));

  /** @brief Parse command line arguments according to the accepted arguments.
   *
   * Wrapper around the list<string> version of parseArgs.
   *
   * @param argc Argumetn counter of the main function.
   * @param argv Argument vector of the main function.
   * @throw std::runtime_error When the command line args are bad.
   * Shall be cought be the client code!
   * @throw std::logic_error If the addArgument was bad.
   */
  void parseArgs(int argc,
                 const char* argv[]);

  void parseArgs(const std::list<std::string> argList);

  // is this argument passed as a command line parameter?
  bool foundArg(const std::string arg) const;
  // argument is passed as a command line parameter, but has a value?
  bool argHasValue(const std::string arg) const;

  // return true is arg exists and the arg in &value
  // arg need to be the same string as in addArgument ( "-h,--help" )
  bool argAsString(const std::string arg, std::string &value) const;
  bool argAsInt(const std::string arg, int &value) const;
  bool argAsFloat(const std::string arg, float &value) const;
  bool argAsBool(const std::string arg, bool &value) const;

  std::string usage() const;


private:

  bool thereAreOptionalArgs() const;
  bool thereAreRequiredArgs() const;
  std::string printArgs(const Required argRequired) const;

  void checkRequiredArgsFound() const;

  void validateValue(const ArgParse::ValueType type,
                     const std::string name,
                     const std::string choices,
                     const std::string value) const;

  void validateString( const std::string name,
                       const std::string choices,
                       const std::string value) const;
  void validateInt( const std::string name,
                    const std::string choices,
                    const std::string value) const;
  void validateFloat( const std::string name,
                      const std::string choices,
                      const std::string value) const;
  void validateBool( const std::string name,
                     const std::string choices,
                     const std::string value) const;



  struct Argument {
    const std::string m_help;
    const ValueType m_type;
    const Required m_valueRequired;
    const Required m_argRequired;
    const std::string m_valueName;
    const std::string m_choices;
    std::string m_value;
    bool m_found;
    bool m_valueHasBeenSet;

    Argument (const std::string help,
              const ValueType type = NONE,
              const Required valueRequired = REQUIRED,
              const Required argRequired = OPTIONAL,
              const std::string valueName = std::string(""),
              const std::string choices = std::string(""),
              const std::string value = std::string(""));
    };

  class argCompare {
    public:
      // short and long arg shall be compared with same amount of dashes
      // this is needed at the usage, so the order of the params is ok,
      // even when a param has short/long/booth name
      bool operator()(const std::string a,const std::string b) const;
  };

  typedef std::map<std::string, Argument, argCompare> ArgMap;

  // arg is just the shor or long form: "-h" or "--help"
  ArgMap::iterator findKeyinArgMap(const std::string param);
  std::set<std::string> choicesStringToSet(const std::string s) const;

  /** @return with valueName if specified,
  or the string version of the type enum otherwise */
  std::string typeToString(const ValueType type,
                           const std::string valueName) const;


  std::string m_description;
  std::string m_epilog;
  std::string m_programName;

  ArgMap m_params;


}; // class ArgParse


#endif // ARGPARSE_HPP
