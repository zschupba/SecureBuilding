This program only runs on linux

Build Instructinos:

make local

./local

This program is entirely controlled by the CLI so my security approach was validating userInput very thouruoghly to only what i want and then structuring the commands to be from a list of options

The main security features i had to add was whitelist userValidation, protecting arrays and buffer boundaries, preventing brute force and ddos attacks from users and 
practicing principle of least privilege for user priviliges.

I didn't use many built in libraries for security, i handle most of it myself but termios.h helped hide cli input
