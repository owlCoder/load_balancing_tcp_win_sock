#ifndef CRITICAL_SECTION_WRAPPER_HPP
#define CRITICAL_SECTION_WRAPPER_HPP

#include <winsock2.h>

/**
 * @brief Initializes the Critical Section wrapper object.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */
inline void InitializeCriticalSectionWrapper(CRITICAL_SECTION* cs) {
    InitializeCriticalSection(cs);
}

/**
 * @brief Deletes the Critical Section wrapper object.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */
inline void DeleteCriticalSectionWrapper(CRITICAL_SECTION* cs) {
    DeleteCriticalSection(cs);
}

/**
 * @brief Enters the Critical Section.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */
inline void EnterCriticalSectionWrapper(CRITICAL_SECTION *cs) {
    EnterCriticalSection(cs);
}

/**
 * @brief Leaves the Critical Section.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */
inline void LeaveCriticalSectionWrapper(CRITICAL_SECTION *cs) {
    LeaveCriticalSection(cs);
}

#endif // CRITICAL_SECTION_WRAPPER_HPP
