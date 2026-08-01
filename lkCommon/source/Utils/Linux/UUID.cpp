#include "lkCommon/Utils/UUID.hpp"

#include <lkCommon/Utils/Logger.hpp>


namespace lkCommon {
namespace Utils {

UUID::UUID()
    : mUUID()
{
    uuid_clear(mUUID);
}

void UUID::Generate()
{
    uuid_generate(mUUID);
}

bool UUID::FromString(const std::string& str)
{
    int res = uuid_parse(str.c_str(), mUUID);

    if (res < 0)
    {
        LOGE("Failed to parse " << str << " into UUID");
        return false;
    }

    return true;
}

std::string UUID::ToString() const
{
    char str[37] = {0,};
    uuid_unparse(mUUID, str);
    return std::string(str);
}

bool UUID::operator==(const UUID& other) const
{
    return (uuid_compare(mUUID, other.mUUID) == 0);
}

bool UUID::operator!=(const UUID& other) const
{
    return (uuid_compare(mUUID, other.mUUID) != 0);
}

bool UUID::operator<(const UUID& other) const
{
    return (uuid_compare(mUUID, other.mUUID) < 0);
}

bool UUID::operator>(const UUID& other) const
{
    return (uuid_compare(mUUID, other.mUUID) > 0);
}

std::ostream& operator<<(std::ostream& s, const UUID& u)
{
    return (s << u.ToString());
}

} // namespace Utils
} // namespace lkCommon
