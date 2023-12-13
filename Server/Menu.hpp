#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "StressTests.hpp"
#include "../Common/Logger.hpp"
#include "string.h"

/**
 * @brief Displays the menu options to the user.
 */
void DisplayMenu();

/**
 * @brief Runs the desired option chosen by the user.
 *
 * @param serverFunction A function pointer to the server function that needs to be executed.
 *                      This function is invoked based on StartServer function
 */
void RunDesiredOption(void (*serverFunction)());

#endif // MENU_FUNCTIONS_H
