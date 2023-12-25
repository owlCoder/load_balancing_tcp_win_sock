/*
    COLORS.hpp

    Description:
    ------------
    This header file defines macros for foreground colors and their combinations for Windows console applications.
    These macros assist in setting various foreground text colors when outputting text to the console.

    Macros Defined:
    ---------------
    - FOREGROUND_BLUE: Blue color for text.
    - FOREGROUND_GREEN: Green color for text.
    - FOREGROUND_RED: Red color for text.
    - FOREGROUND_INTENSITY: Mask for brightness (intensity) of colors.

    Color Combinations:
    -------------------
    - FOREGROUND_WHITE: White color composed of red, green, and blue.
    - FOREGROUND_CYAN: Cyan color composed of green and blue.
    - FOREGROUND_MAGENTA: Magenta color composed of red and blue.
    - FOREGROUND_YELLOW: Yellow color composed of red and green.
    - FOREGROUND_LIGHTBLUE: Light blue color with higher intensity, composed of blue.
    - FOREGROUND_LIGHTGREEN: Light green color with higher intensity, composed of green.
    - FOREGROUND_LIGHTRED: Light red color with higher intensity, composed of red.
    - FOREGROUND_LIGHTWHITE: Light white color with higher intensity, composed of red, green, and blue.
    - FOREGROUND_LIGHTCYAN: Light cyan color with higher intensity, composed of green and blue.
    - FOREGROUND_LIGHTMAGENTA: Light magenta color with higher intensity, composed of red and blue.
    - FOREGROUND_LIGHTYELLOW: Light yellow color with higher intensity, composed of red and green.
*/

#ifndef COLORS_HPP
#define COLORS_HPP

#define FOREGROUND_BLUE      0x0001 // Blue
#define FOREGROUND_GREEN     0x0002 // Green
#define FOREGROUND_RED       0x0004 // Red
#define FOREGROUND_INTENSITY 0x0008 // Brightness (intensity) mask

#define FOREGROUND_WHITE     (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FOREGROUND_CYAN      (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FOREGROUND_MAGENTA   (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_YELLOW    (FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_LIGHTBLUE (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTGREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTRED  (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTWHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTCYAN  (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTMAGENTA (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_LIGHTYELLOW  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#endif // COLORS_H
