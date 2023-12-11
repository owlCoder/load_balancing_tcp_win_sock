#ifndef CRITICAL_SECTION_WRAPPER_HPP
#define CRITICAL_SECTION_WRAPPER_HPP

#include <winsock2.h>

/**
 * @brief A wrapper struct for handling Critical Section operations.
 */
typedef struct {
    CRITICAL_SECTION cs; /**< The actual Critical Section object. */
} CriticalSectionWrapper;

/**
 * @brief Initializes the Critical Section wrapper object.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */
void InitializeCriticalSectionWrapper(CriticalSectionWrapper* wrapper) {
    InitializeCriticalSection(&(wrapper->cs));
}

/**
 * @brief Deletes the Critical Section wrapper object.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */void DeleteCriticalSectionWrapper(CriticalSectionWrapper* wrapper) {
    DeleteCriticalSection(&(wrapper->cs));
}

/**
 * @brief Enters the Critical Section.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */
void EnterCriticalSectionWrapper(CriticalSectionWrapper* wrapper) {
    EnterCriticalSection(&(wrapper->cs));
}

/**
 * @brief Leaves the Critical Section.
 *
 * @param wrapper A pointer to the Critical Section wrapper object.
 */void LeaveCriticalSectionWrapper(CriticalSectionWrapper* wrapper) {
    LeaveCriticalSection(&(wrapper->cs));
}

#endif // CRITICAL_SECTION_WRAPPER_HPP
