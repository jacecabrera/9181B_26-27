b_template file structure and purposes; for bowen, myself, design book, and all

comments are also scattered across individual files

include/
    b_template/
        api.h/ : all the #includes (easy access)
        chassis/ : will store declarations relevant to chassis (gear ratio, track width, sensor setup, etc.)
        control/ : contains the math controllers and other necessary algs (PID, slew rate limiter(?), settling/exit conditions)
        subsystem/ : class definitions for subsystems, helper methods like set_state and such
        util/ : math utils, pose
    config.hpp : stores physical hardware config data such as ports

general notes:
    - always put an ';' after every line otherwise it may not compile correctly
    - include your header file in your source file (or else you can't use it lol)
    - for every source file, there is usually a cooresponding header file
    - a struct is just a data type that stores related variables and functions
    - you only delcare or mention methods or variables in a header file, but you assign real logic in the source file
    - put "#pragma once' at the top of each header file (TRUST ME)
    - typically, any sort of coordinate mapping will use either cartesian or polar coordinates
        -> in cases that involve rotation or uses heading as a reference, polar coordinates (r, theta) will come in handy. Otherwise, use cartesian (x, y)
    - for interal calcuations, the <cmath> consideres everything in radians rather than degrees 
        ex: std::sin(), std::cos()
    - double vs double& as a param: double makes a copy of the input variable that cannot be modified, a double& will apply the modifications made inside the function

good practices:
    1. when declaring numbers using primitve constants, try to mark them with 'inline constexpr'. (don't ask me why it's just better and doesn't throw out errors)
        ex: 'inline constexpr double kP = 0.1', 'inline constexpr randomNum = 0.2'
    2. to be safe, when performing an opreation with, say, a double and you want to return a double also, ensure that you include a decimal and an extra figure to prevent poor truncation (if all numbers in an operation are of an integer type, it will cut down the output figures to that of an integer)
        -> an example of this can be found in math_utils.hpp : '180.0 is used instead of 180'. 
    3. if a function contains compile-time variables (not dynamically changing), you should add the 'constexpr' prefix to make calcuations at compile-time rather than runtime

TO-DO:
    - [ ] setup auton selector system