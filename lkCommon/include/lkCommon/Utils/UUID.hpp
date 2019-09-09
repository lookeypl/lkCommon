#pragma once

#include <lkCommon/lkCommon.hpp>

#ifdef WIN32
#include <rpc.h>
#elif defined(__linux__) | defined(__LINUX__)
#include <uuid/uuid.h>
#else
#error "Target platform not supported"
#endif

#include <string>


namespace lkCommon {
namespace Utils {

class UUID
{
    #ifdef WIN32
    ::UUID mUUID;
    #elif defined(__linux__) | defined(__LINUX__)
    uuid_t mUUID;
    #endif

public:
    /**
     * Constructs an empty, zeroed UUID
     */
    UUID();
    ~UUID() = default;

    /**
     * Generates a random UUID using platform-dependent calls.
     */
    void Generate();

    /**
     * Converts string into an UUID.
     *
     * String must be in predefined UUID format, ex:
     *    00000000-1111-2222-3333-444444444444
     *
     * @p[in] str Entry string to parse
     * @return True on success, false when parsing failed.
     */
    bool FromString(const std::string& str);

    /**
     * Returns UUID in form of a string.
     *
     * @return String containing UUID in predefined UUID format.
     */
    std::string ToString() const;

    /**
     * @defgroup Comparisons Comparison operators
     * @{
     */
    bool operator==(const UUID& other) const;
    bool operator!=(const UUID& other) const;
    bool operator<(const UUID& other) const;
    bool operator>(const UUID& other) const;
    /**
     * @}
     */

    friend std::ostream& operator<<(std::ostream& s, const UUID& u);
};

/**
 * Stream operator for
 */
std::ostream& operator<<(std::ostream& s, const UUID& u);

} // namespace Utils
} // namespace lkCommon
