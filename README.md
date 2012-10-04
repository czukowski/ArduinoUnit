ArduinoUnit 
===========

Fork of Matthew Murdoch's Unit Testing module for Arduino.

Install for Arduino IDE
-----------------------

Create a directory called `ArduinoUnit` under `<arduino installation directory>\libraries`.

Copy everything from the src directory to this new directory.

Open a sketch in arduino and click **Sketch | Import Library | ArduinoUnit** to start unit testing.

For example, try this simple unit testing sketch:

    #include <ArduinoUnit.h>

    // Create test suite
    TestSuite suite;

    void setup() {
    }

    // Create a test called 'addition' in the test suite
    test(addition) {
        assertEquals(3, 1 + 2);
    }

    void loop() {
        // Run test suite, printing results to the serial port
        suite.run();
    }

Upload this sketch to the Arduino (using the 'Upload' button, 
File | Upload to I/O Board or Ctrl+U).

Turn on the Serial Monitor (using the 'Serial Monitor' button, Tools | Serial Monitor or 
Ctrl+Shift+M) and expect to see the following:

    Running test suite...
    Tests run: 1 Successful: 1 Failed: 0

FAQ
---

1. **Q.** I'm getting an error message like:

          1: error: pasting "test_" and ""something"" does not give a valid preprocessing token

      when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   **A.** You have declared a test like this with the name in quotes:

          test("something") {
              // ...
          }

      The name of the test must not be quoted. i.e. it should look like this:

          test(something) {
              // ...
          }

2. **Q.** I'm getting an error message like:

          error: redefinition of 'SuiteAppender test_a_appender' In function 'void test_a(Test&)':

      when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   **A.** You have two or more tests declared with the same name. For example:

          test(a) {
              // ...
          }
          
          test(a) {
              // ...
          }

      The names of all tests in the same sketch must be unique. This is true even if they are in 
         different test suites.

3. **Q.** I'm trying to write a custom assertion function but I keep getting an error message like:

          error: '__test__' was not declared in this scope

      when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   **A.** You have probably defined a custom assertion function such as the following:

          void assertStringsEqual(const char* expected, const char* actual) {
              assertEquals(strlen(expected), strlen(actual));
              for (int i = 0; i < strlen(expected); i++) {
                  assertEquals(expected[i], actual[i]);
              }
          }
          
      and are trying to use it like this from within a test function:
       
          assertStringsEqual("expected string", actualString);
          
      To fix this you need to add a `Test& __test__` parameter to the signature of the custom 
      assertion function, for example:
      
          void assertStringsEqual(Test& __test__, const char* expected, const char* actual) {
              assertEquals(strlen(expected), strlen(actual));
              for (int i = 0; i < strlen(expected); i++) {
                  assertEquals(expected[i], actual[i]);
              }
          }

      and add `__test__` as the relevant argument when you call it, for example:
      
          assertStringsEqual(__test__, "expected string", actualString);

4. **Q.** I'm trying to use a custom assertion function but I keep getting an error message like:

          error: '<custom_assertion_function>' was not declared in this scope

      when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   **A.** You need to define the custom assertion function above where it is being used or 
        forward-declare it, for example:
        
          // Forward declare custom assertion
          void assertStringsEqual(Test& __test__, const char* expected, const char* actual);

          // ...
          
          test(something) {
              // Use custom assertion
              assertStringsEqual(__test__, "expected string", actualString);
          }
          
          // ...

          // Define custom assertion
          void assertStringsEqual(Test& __test__, const char* expected, const char* actual) {
              assertEquals(strlen(expected), strlen(actual));
              for (int i = 0; i < strlen(expected); i++) {
                  assertEquals(expected[i], actual[i]);
              }
          }