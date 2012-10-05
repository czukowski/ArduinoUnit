ArduinoUnit 
===========

Fork of Matthew Murdoch's Unit Testing module for Arduino. In this fork I'll try to migrate this library
to use it with the Atmel Studio 6. This fork is experimental.

Install for Atmel Studio 6
--------------------------

To import the library - this only needs to be done once. Navigate to your Atmel Studio 6 solution directory,
usually in `..\Documents\Atmel Studio`. If you're using git to version control your solution, open a git console
and type:

    git clone git://github.com/czukowski/ArduinoUnit.git ArduinoUnit

Open a newly created solution (`ArduinoUnit.atsln`) in Atmel Studio and build it. Now you have a file named
`libArduinoUnit.a` you will be linking to your projects.

To add unit tests to your solution, create a new Executable C++ project in your solution. Set up include and library
paths as necessary. The include path to the ArduinoUnit project may look like this:

	../../../ArduinoUnit/ArduinoUnit

The library search path for the `libArduinoUnit.a` file you've built may look like this:

	../../../ArduinoUnit/ArduinoUnit/Debug

Don't forget to add ArduinoUnit to the libraries list. You're done with the set up now, it's time to write some tests!

Note, that you may run the tests on the AVR Simulator, but it won't output anything and it seems that the only way (and
yes, it sucks) to know if any test has failed, is to debug the program and set breakpoints in some points of interest,
for example in `TestSuite::addFailure()` method.

Install for Arduino IDE
-----------------------

Create a directory called `ArduinoUnit` under `<arduino installation directory>\libraries`.

Copy `ArduinoUnit.h`, `keywords.txt` and `utility` dir from the `ArduinoUnit` directory to this new directory.

Open a sketch in arduino and click **Sketch | Import Library | ArduinoUnit** to start unit testing.

Writing tests
-------------

This works for both Arduino IDE and Atmel Studio setups. Try this simple unit testing sketch:

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

Build and deploy the tests and expect to see the following on the serial monitor:

    Running test suite...
    Tests run: 1 Successful: 1 Failed: 0

FAQ
---

1. **Q.** I'm getting an error message like:

		1: error: pasting "test_" and ""something"" does not give a valid preprocessing token

   when I build or deploy my tests. How can I fix this?

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

   when I build or deploy my tests. How can I fix this?

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

   when I build or deploy my tests. How can I fix this?

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

   when I build or deploy my tests. How can I fix this?

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