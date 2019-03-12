#pragma once

#include <string>
#include <vector>


namespace lkCommon {
namespace Utils {

enum class ArgType: unsigned char
{
    UNDEFINED = 0,
    FLAG,
    VALUE,
    STRING,
};

/**
 * Input argument parser object.
 *
 * ArgParser is designed to easily parse input arguments (aka. argv's) and provide a handy interface
 * for acquiring values provided by user (or default values if argv's miss provided arguments).
 *
 * Three types of arguments are available:
 *   - FLAG - Set to true if argument of given name exists. This argument does NOT have a default.
 *   - VALUE - Set to integer value provided by user after argument. Can have default value.
 *   - STRING - Set to string provided by user after argument. Can have default value.
 *
 * Following argument formats are considered as valid:
 * Long args:
 *   - "--argument 42"
 *   - "--argument=42"
 * Short args:
 *   - "-a42"
 *   - "-a=42"
 *   - "-a 42"
 */
class ArgParser
{
    using NameShortnamePair = std::pair<char, std::string>;

    struct Arg
    {
        const char shortName;
        const std::string name;
        const std::string desc;
        const ArgType type;
        const bool optional;

        // TODO Right now possible values are split (without union) to avoid constructor madness with std::string.
        //      Use an union here some day
        bool acquired;
        int32_t value;
        std::string string;

        // Argument without default value
        Arg(const NameShortnamePair& nsn, const std::string& desc, const ArgType type, const bool optional)
            : shortName(nsn.first)
            , name(nsn.second)
            , desc(desc)
            , type(type)
            , optional(optional)
            , acquired(false)
            , value(0)
            , string()
        {
        }

        // Argument with default value
        Arg(const NameShortnamePair& nsn, const std::string& desc, const ArgType type, const bool optional, const int32_t value)
            : shortName(nsn.first)
            , name(nsn.second)
            , desc(desc)
            , type(type)
            , optional(optional)
            , acquired(false)
            , value(value)
            , string()
        {
        }

        // Argument with default string
        Arg(const NameShortnamePair& nsn, const std::string& desc, const ArgType type, const bool optional, const std::string& string)
            : shortName(nsn.first)
            , name(nsn.second)
            , desc(desc)
            , type(type)
            , optional(optional)
            , acquired(false)
            , value(0)
            , string(string)
        {
        }
    };

    using ArgContainer = std::vector<Arg>;
    using ArgIt = ArgContainer::iterator;
    using ConstArgIt = ArgContainer::const_iterator;

    ArgContainer mArgs;


    // Splits name string into name/shortname combo
    NameShortnamePair SplitNameString(const std::string& name) const;

    // Argument searchers returning const iterator
    ConstArgIt FindShortName(const char shortName) const;
    ConstArgIt FindName(const std::string& name) const;

    // Argument searchers returning modifiable iterator
    ArgIt FindShortName(const char shortName);
    ArgIt FindName(const std::string& name);

    // Returns true if given name/shortname string exists, false otherwise
    bool NameShortnameExists(const NameShortnamePair& nsn) const;

    // Helper for acquiring args in getters, to avoid repeating code
    // Returns true when it can be used to get a value/string from. Otherwise, in case of errors, returns false.
    bool GetHelper(const char shortName, const ArgType& type, ConstArgIt& it) const;
    bool GetHelper(const std::string& name, const ArgType& type, ConstArgIt& it) const;

    // true if c is an ASCII letter
    bool IsLetter(const char c) const;

    // Gets argument name. One letter string is considered as a short name, more than that as a long name
    std::string ParseArgName(const std::string& arg, size_t& splitPos) const;

    // Gets amount of characters used by argument in usage text
    uint32_t GetSpaceUsed(const Arg& arg) const;

public:
    ArgParser() = default;
    ~ArgParser() = default;

    /**
     * @defgroup ArgAdders Argument adders
     *
     * Collection of functions adding new arguments to search for.
     *
     * Argument name should consist of either:
     *   - a single char (short arg)
     *   - a string (long arg)
     *   - a combination of both, separated by comma
     *
     * Below are valid combinations of argument "example" with short name "e":
     *   - "e"
     *   - "example"
     *   - "e,example"
     *   - "example,e"
     *
     * Following argument names (and their similar variants) are considered as invalid:
     *   - "e,e"
     *   - ",eeeeee"
     *   - "e,"
     *
     * @warning Argument names should contain only ASCII letters. Numbers and special characters
     *          are considered as error.
     *
     * Arguments can be marked as "optional", which will not fail parsing if argument is not present.
     *
     * @{
     */

    /**
     * Add new argument of given type, without default value and optional description.
     *
     * @p[in] name      Name of the argument. See Argument adders group description for more info about argument naming.
     * @p[in] type      Type of added argument.
     * @p[in] optional  True if argument is optional.
     * @p[in] desc      Description of argument for help printing purposes. Can be omitted.
     * @return True on success, false on error.
     */
    bool Add(const std::string& name, const ArgType type, const bool optional, const std::string& desc = std::string());

    /**
     * Add new value type argument, with default value and optional description.
     *
     * @p[in] name          Name of the argument. See Argument adders group description for more info about argument naming.
     * @p[in] defaultValue  Default value of argument.
     * @p[in] optional      True if argument is optional.
     * @p[in] desc          Description of argument for help printing purposes. Can be omitted.
     * @return True on success, false on error.
     */
    bool Add(const std::string& name, const int32_t defaultValue, const bool optional, const std::string& desc = std::string());

    /**
     * Add new value type argument, with default value and optional description.
     *
     * @p[in] name          Name of the argument. See Argument adders group description for more info about argument naming.
     * @p[in] defaultString Default value of argument.
     * @p[in] optional      True if argument is optional.
     * @p[in] desc          Description of argument for help printing purposes. Can be omitted.
     * @return True on success, false on error.
     */
    bool Add(const std::string& name, const std::string& defaultString, const bool optional, const std::string& desc = std::string());

    /**
     * @}
     */

    /**
     * Parses argv's and fills created base of arguments with read values.
     *
     * @return True when parsing succeeds - all required arguments were parsed correctly and expected
     *         values were returned. In case user provides invalid argument (ex. STRING in place of
     *         FLAG/VALUE, or any required argument is missing) function returns false.
     */
    bool Parse(int argc, const char* const argv[]);

    /**
     * Gets currently stored argument list in form of a string, handy to print to user as a help message.
     *
     * @return String with preformatted usage data.
     */
    std::string GetUsage() const;

    /**
     * Gets usage info using GetUsage() and prints
     */
    void PrintUsage() const;

    /**
     * @defgroup ArgGetters Argument getters
     *
     * Collection of functions getting values/results of parsed arguments.
     *
     * Use below functions
     * @{
     */

    /**
     * Check if flag according to long name was set.
     *
     * @p[in] name Long name of argument to query
     *
     * @return True if arguments contained flag given by name
     */
    bool GetFlag(const std::string& name) const;

    /**
     * Check if flag according to short name was set.
     *
     * @p[in] name Short name of argument to query
     *
     * @return True if arguments contained flag given by name
     */
    bool GetFlag(const char shortName) const;

    /**
     * Acquire value added to argument given by long name.
     *
     * @p[in]  name  Long name of argument to query.
     * @p[out] value Value provided by user in arguments.
     *
     * @return True if arguments contained an argument and its value.
     */
    bool GetValue(const std::string& name, int32_t& value) const;

    /**
     * Acquire value added to argument given by short name.
     *
     * @p[in]  name  Short name of argument to query.
     * @p[out] value Value provided by user in arguments.
     *
     * @return True if arguments contained an argument and its value.
     */
    bool GetValue(const char shortName, int32_t& value) const;

    /**
     * Acquire value added to argument given by long name.
     *
     * @p[in]  name   Long name of argument to query.
     * @p[out] string String provided by user in arguments.
     *
     * @return True if arguments contained an argument and its string.
     */
    bool GetString(const std::string& name, std::string& result) const;

    /**
     * Acquire value added to argument given by short name.
     *
     * @p[in]  name   Short name of argument to query.
     * @p[out] string String provided by user in arguments.
     *
     * @return True if arguments contained an argument and its string.
     */
    bool GetString(const char shortName, std::string& result) const;

    /**
     * @}
     */
};

} // namespace lkCommon
} // namespace Utils
