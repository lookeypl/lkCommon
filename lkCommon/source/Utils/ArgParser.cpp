#include "lkCommon/Utils/ArgParser.hpp"

#include "lkCommon/Utils/Logger.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>


namespace lkCommon {
namespace Utils {

ArgParser::NameShortnamePair ArgParser::SplitNameString(const std::string& name) const
{
    NameShortnamePair result;
    result.first = 0;
    result.second.clear();

    size_t comma = name.find(',');
    if (comma == std::string::npos)
    {
        // no comma found, now we have two options - it's either a short arg, or a long arg
        // short arg will be one-char only
        if (name.length() == 1)
            result.first = name[0];
        else
            result.second = name;
    }
    else
    {
        // name string shorter than 4 chars is most probably malformed and looks like "a," or "a,a"
        // both of these options are meh for us, so discard them if they come up
        if (name.size() <= 3)
            return result;

        // comma is here - check where is the short arg
        // it should be at the beginning, or at an end, otherwise it is an error
        if (comma == name.length() - 2)
        {
            result.first = name.c_str()[comma+1];
            result.second = name.substr(0, comma);
        }
        else if (comma == 1)
        {
            result.first = name.c_str()[0];
            result.second = name.substr(comma + 1);
        }
    }

    return result;
}

ArgParser::ConstArgIt ArgParser::FindShortName(const char shortName) const
{
    ConstArgIt it = mArgs.begin();
    for (it; it != mArgs.end(); ++it)
        if (it->shortName == shortName)
            break;

    return it;
}

ArgParser::ConstArgIt ArgParser::FindName(const std::string& name) const
{
    ConstArgIt it = mArgs.begin();
    for (it; it != mArgs.end(); ++it)
        if (it->name == name)
            break;

    return it;
}

ArgParser::ArgIt ArgParser::FindShortName(const char shortName)
{
    ArgIt it = mArgs.begin();
    for (it; it != mArgs.end(); ++it)
        if (it->shortName == shortName)
            break;

    return it;
}

ArgParser::ArgIt ArgParser::FindName(const std::string& name)
{
    ArgIt it = mArgs.begin();
    for (it; it != mArgs.end(); ++it)
        if (it->name == name)
            break;

    return it;
}

bool ArgParser::NameShortnameExists(const NameShortnamePair& nsn) const
{
    ConstArgIt snIt = FindShortName(nsn.first);
    ConstArgIt nIt = FindName(nsn.second);

    return (nIt != mArgs.end()) || (snIt != mArgs.end());
}

bool ArgParser::GetHelper(const char shortName, const ArgType& type, ConstArgIt& it) const
{
    it = FindShortName(shortName);
    if (it == mArgs.end())
        return false;

    if (it->type != type)
        return false;

    return it->acquired;
}

bool ArgParser::GetHelper(const std::string& name, const ArgType& type, ConstArgIt& it) const
{
    it = FindName(name);
    if (it == mArgs.end())
        return false;

    if (it->type != type)
        return false;

    return it->acquired;
}

bool ArgParser::IsLetter(const char c) const
{
    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}

std::string ArgParser::ParseArgName(const std::string& arg, size_t& equalsPos) const
{
    // less than 2 chars is not a name - name should have at least '-' and a letter
    if (arg.size() < 2)
        return std::string();

    // first char must be a dash
    if (arg[0] != '-')
        return std::string();

    // second char is not a dash
    if (arg[1] != '-')
    {
        // if is a letter, return shortname, else return error
        if (IsLetter(arg[1]))
            return arg.substr(1, 1);
        else
            return std::string();
    }

    // second char is a dash, parse contents until '='
    equalsPos = arg.rfind('=');
    if (equalsPos != std::string::npos)
    {
        return arg.substr(2, equalsPos - 2);
    }

    // no '=' meaning entire arg is argname
    return arg.substr(2);
}

uint32_t ArgParser::GetSpaceUsed(const Arg& arg) const
{
    uint32_t result = (arg.shortName != 0) ? 2 : 0; // short arg name + "-"

    if (!arg.name.empty())
        result += arg.name.size() + 2; // full arg name + "--"
    if (arg.shortName != 0 && !arg.name.empty())
        result++; // for ',' between names
    if (arg.optional)
        result++; // for closing ']'

    return result;
}

bool ArgParser::Add(const std::string& name, ArgType type, bool optional, const std::string& desc)
{
    NameShortnamePair nsn = SplitNameString(name);
    if (nsn.first == 0 && nsn.second.empty())
    {
        LOGE("Failed to add argument \"" << name << "\"");
        return false;
    }

    if (NameShortnameExists(nsn))
    {
        LOGE("Argument name/shortname \"" << name << "\" already exists in parser's collection. Skipping.");
        return false;
    }

    mArgs.emplace_back(nsn, desc, type, optional);
    return true;
}

bool ArgParser::Add(const std::string& name, int32_t defaultValue, bool optional, const std::string& desc)
{
    NameShortnamePair nsn = SplitNameString(name);
    if (nsn.first == 0 && nsn.second.empty())
    {
        LOGE("Failed to add argument \"" << name << "\"");
        return false;
    }

    if (NameShortnameExists(nsn))
    {
        LOGE("Argument name/shortname \"" << name << "\" already exists in parser's collection. Skipping.");
        return false;
    }

    mArgs.emplace_back(nsn, desc, ArgType::VALUE, optional, defaultValue);
    return true;
}

bool ArgParser::Add(const std::string& name, const std::string& defaultString, bool optional, const std::string& desc)
{
    NameShortnamePair nsn = SplitNameString(name);
    if (nsn.first == 0 && nsn.second.empty())
    {
        LOGE("Failed to add argument \"" << name << "\"");
        return false;
    }

    if (NameShortnameExists(nsn))
    {
        LOGE("Argument name/shortname \"" << name << "\" already exists in parser's collection. Skipping.");
        return false;
    }

    mArgs.emplace_back(nsn, desc, ArgType::STRING, optional, defaultString);
    return true;
}

bool ArgParser::Parse(int argc, const char* const argv[])
{
    bool success = true;
    for (int i = 1; i < argc; ++i)
    {
        // parse name
        size_t equalsPos = 0;
        std::string argStr(argv[i]);
        std::string argName = ParseArgName(argStr, equalsPos);

        if (argName.empty())
        {
            LOGE("Error while parsing argument " << argStr);
            continue;
        }

        // find name in collection
        ArgIt it;
        if (argName.size() == 1)
            it = FindShortName(argName[0]);
        else
            it = FindName(argName);

        if (it == mArgs.end())
        {
            LOGE("Unrecognised argument: " << argv[i]);
            continue;
        }

        // get argument's value
        std::string argValue;
        if (it->type != ArgType::FLAG)
        {
            if (equalsPos != std::string::npos)
            {
                argValue = argStr.substr(equalsPos + 1);
            }
            else
            {
                i++;
                if (i == argc)
                {
                    LOGE("Not enough arguments provided.");
                    success = false;
                    continue;
                }

                argValue = argv[i];
            }
        }

        // assign value to arg
        if (it->type == ArgType::VALUE)
        {
            try
            {
                it->value = std::stoi(argValue);
            }
            catch (...)
            {
                LOGE("Invalid value for argument " << it->name << " provided: " << argv[i]);
                success = false;
                continue;
            }
        }
        else if (it->type == ArgType::STRING)
        {
            it->string = argv[i];
        }

        it->acquired = true;
    }

    ConstArgIt it = std::find_if(mArgs.cbegin(), mArgs.cend(), [](const Arg& a) {
        return !(a.optional || a.acquired);
    });

    if (it != mArgs.end())
    {
        LOGE("Not all required arguments were provided");
        return false;
    }

    return success;
}

std::string ArgParser::GetUsage() const
{
    // calculate common spacing amount between args and desc
    uint32_t maxSpacing = 0;
    uint32_t curSpacing = 0;
    for (const auto& arg: mArgs)
    {
        curSpacing = GetSpaceUsed(arg);
        if (maxSpacing < curSpacing)
            maxSpacing = curSpacing;
    }


    // form our string
    std::stringstream result;
    result << "Arguments are: " << std::endl;

    for (const auto& arg: mArgs)
    {
        result << "  ";

        if (arg.optional)
            result << '[';
        else
            result << ' ';

        if (arg.shortName != '\0')
            result << '-' << arg.shortName;

        if (arg.shortName != '\0' && !arg.name.empty())
            result << ',';

        if (!arg.name.empty())
            result << "--" << arg.name;
        if (arg.optional)
            result << ']';

        if (!arg.desc.empty())
        {
            curSpacing = GetSpaceUsed(arg);

            // add extra 4 spaces to separate args from descs
            for (uint32_t i = curSpacing; i < maxSpacing + 4; ++i)
                result << ' ';

            result << arg.desc;
        }

        result << std::endl;
    }

    return result.str();
}

void ArgParser::PrintUsage() const
{
    std::cout << GetUsage() << std::endl;
}

bool ArgParser::GetFlag(const std::string& name) const
{
    ConstArgIt it;
    return GetHelper(name, ArgType::FLAG, it);
}

bool ArgParser::GetFlag(const char shortName) const
{
    ConstArgIt it;
    return GetHelper(shortName, ArgType::FLAG, it);
}


bool ArgParser::GetValue(const std::string& name, uint32_t& value) const
{
    ConstArgIt it;
    if (!GetHelper(name, ArgType::VALUE, it))
        return false;

    value = it->value;
    return true;
}

bool ArgParser::GetValue(const char shortName, uint32_t& value) const
{
    ConstArgIt it;
    if (!GetHelper(shortName, ArgType::VALUE, it))
        return false;

    value = it->value;
    return true;
}

bool ArgParser::GetString(const std::string& name, std::string& result) const
{
    ConstArgIt it;
    if (!GetHelper(name, ArgType::STRING, it))
        return false;

    result = it->string;
    return true;
}

bool ArgParser::GetString(const char shortName, std::string& result) const
{
    ConstArgIt it;
    if (!GetHelper(shortName, ArgType::STRING, it))
        return false;

    result = it->string;
    return true;
}

} // namespace lkCommon
} // namespace Utils
