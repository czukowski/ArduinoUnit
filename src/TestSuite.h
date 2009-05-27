/*
Copyright (c) 2009 Matthew Murdoch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*/
#ifndef TEST_SUITE_H
#define TEST_SUITE_H

struct Test;
struct TestLink;
class Reporter;

/**
 * A test suite consisting of a number of tests.
 *
 * @author Matthew Murdoch
 */
class TestSuite {
public:
    /**
     * Creates a test suite and sets it as the active suite if there is no
     * currently active suite.
     *
     * @param name the name of this test suite (optional, defaults to empty string)
     */
    TestSuite(const char* name = "");

    /**
     * Destroys this test suite.
     */
    ~TestSuite();

    /**
     * Returns whether or not there is an active suite.
     *
     * @return true if there is an active suite, false otherwise
     */
    static bool isActiveSuite();

    /**
     * Returns the active test suite. If called when isActiveSuite() is false
     * the results are undefined.
     *
     * @return the active test suite
     */
    static TestSuite& getActiveSuite();

    /**
     * Sets the active test suite.
     *
     * @param suite active test suite to set
     */
    static void setActiveSuite(TestSuite& suite);

    /**
     * Returns the name of this test suite.
     *
     * @return test suite name
     */
    const char* getName() const;

    /**
     * Returns the reporter which reports on the outcome of this suite.
     *
     * @return current reporter
     */
    Reporter& getReporter() const;

    /**
     * Sets the reporter to report on the outcome of this suite.
     *
     * @param reporter reporter to set
     */
    void setReporter(Reporter& reporter);

    /**
     * Adds a test to this suite.
     *
     * @param name name of test to add
     * @param testFunction test function to add
     */
    void add(const char* name, void (*testFunction)(Test&));

    /**
     * Runs this test suite. If this suite has already run calling
     * this function has no effect.
     */
    void run();

    /**
     * Returns whether or not this suite has finished running.
     *
     * @return true if this suite has finished running, false otherwise
     */
    bool hasCompleted() const;

    /**
     * Returns the number of test functions in this suite.
     *
     * @return the number of test functions in this suite
     */
    int getTestCount() const;

    /**
     * Returns the number of successful test functions in this suite.
     *
     * @return the number of successful test functions in this suite
     */
    int getSuccessCount() const;

    /**
     * Returns the number of failed test functions in this suite.
     *
     * @return the number of failed test functions in this suite
     */
    int getFailureCount() const;

    /**
     * Asserts that a condition is true.
     *
     * @param test test in which assertion occurred
     * @param condition condition to check
     * @param lineNumber line number on which assertion lives
     */
    void suiteAssertTrue(Test& test, bool condition, int lineNumber);

    /**
     * Asserts that two values are equal.
     *
     * @param test test in which assertion occurred
     * @param first first value to test for equality
     * @param second second value to test for equality
     * @param lineNumber line number on which assertion lives
     */
    void suiteAssertEquals(Test& test, int first, int second, int lineNumber);

private:
    /**
     * Reports on a test failure within this suite via the attached Reporter.
     *
     * @param test failing test
     * @param lineNumber line number on which test failed
     */
    void reportFailure(const Test& test, int lineNumber) const;

    /**
     * Reports on an equality test failure within this suite via the attached Reporter.
     *
     * @param test failing test
     * @param lineNumber line number on which test failed
     * @param expected expected value
     * @param actual actual value
     */
    void reportEqualityFailure(const Test& test, int lineNumber, int expected, int actual) const;

    /**
     * Reports on the outcome of this suite via the attached Reporter.
     */
    void reportComplete() const;

    static TestSuite* activeSuite;

    char* name;
    TestLink* head;
    bool completed;
    Reporter* reporter;
};

#endif // TEST_SUITE_H
