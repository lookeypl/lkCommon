#include "lkCommon/Utils/UUID.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/StringConv.hpp>


namespace lkCommon {
namespace Utils {

UUID::UUID()
    : mUUID()
{
    RPC_STATUS ret = ::UuidCreateNil(&mUUID);
    if (ret != RPC_S_OK)
    {
        LOGE("Failed to create Nil UUID");
    }
}

void UUID::Generate()
{
    RPC_STATUS ret = ::UuidCreate(&mUUID);
    if (ret != RPC_S_OK)
    {
        LOGE("Failed to generate new UUID");
    }
}

bool UUID::FromString(const std::string& str)
{
    std::wstring wideStr;
    if (!Utils::StringToWString(str, wideStr))
    {
        LOGE("Failed to convert input String to WString");
        return false;
    }

    RPC_STATUS ret = ::UuidFromString(reinterpret_cast<RPC_WSTR>(const_cast<wchar_t*>(wideStr.data())), &mUUID);
    if (ret != RPC_S_OK)
    {
        LOGE("Failed to parse UUID string: " << str);
        return false;
    }

    return true;
}

std::string UUID::ToString() const
{
    RPC_WSTR strw;
    RPC_STATUS ret = ::UuidToString(&mUUID, &strw);
    if (ret != RPC_S_OK)
    {
        LOGE("Failed to convert UUID to WString");
        return std::string();
    }

    std::string result;
    if (!Utils::WStringToString(std::wstring(reinterpret_cast<wchar_t*>(strw)), result))
    {
        LOGE("Failed to convert WString to String");
        return std::string();
    }

    return result;
}

bool UUID::operator==(const UUID& other) const
{
    RPC_STATUS ret;
    return (::UuidEqual(const_cast<::UUID*>(&mUUID), const_cast<::UUID*>(&other.mUUID), &ret) == TRUE);
}

bool UUID::operator!=(const UUID& other) const
{
    RPC_STATUS ret;
    return (::UuidEqual(const_cast<::UUID*>(&mUUID), const_cast<::UUID*>(&other.mUUID), &ret) == FALSE);
}

bool UUID::operator<(const UUID& other) const
{
    RPC_STATUS ret;
    return (::UuidCompare(const_cast<::UUID*>(&mUUID), const_cast<::UUID*>(&other.mUUID), &ret) < 0);
}

bool UUID::operator>(const UUID& other) const
{
    RPC_STATUS ret;
    return (::UuidCompare(const_cast<::UUID*>(&mUUID), const_cast<::UUID*>(&other.mUUID), &ret) > 0);
}

std::ostream& operator<<(std::ostream& s, const UUID& u)
{
    return (s << u.ToString());
}

} // namespace Utils
} // namespace lkCommon
