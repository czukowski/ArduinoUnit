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
#include "TestSuite.h"

#include "Test.h"
#include "SerialReporter.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct TestLink {
    Test test;
    TestLink* next;
};

TestSuite* TestSuite::activeSuite = NULL;

TestSuite::TestSuite(const char* nameToCopy) :
    head(NULL), successCount(0), failureCount(0), completed(false) {

    name = (char*) malloc(sizeof(char) * (strlen(nameToCopy)+1));
    strcpy(name, nameToCopy);

    TestSuite::setActiveSuite(*this);

    setReporter(serialReporter);
}

TestSuite::~TestSuite() {
    TestLink* current = head;
    while (current != NULL) {
        TestLink* next = current->next;
        free(current);
        current = next;
    }
}

bool TestSuite::isActiveSuite() {
    return TestSuite::activeSuite != NULL;
}

TestSuite& TestSuite::getActiveSuite() {
    return *(TestSuite::activeSuite);
}

void TestSuite::setActiveSuite(TestSuite& suite) {
    TestSuite::activeSuite = &suite;
}

const char* TestSuite::getName() const {
    return name;
}

Reporter& TestSuite::getReporter() const {
    return *reporter;
}

void TestSuite::setReporter(Reporter& reporter_) {
    reporter = &reporter_;
}

void TestSuite::add(const char* name, void (*testFunction)(Test&)) {
    TestLink* newLink = (TestLink*) malloc(sizeof(TestLink));
    newLink->test.suite = this;
    newLink->test.testFunction = testFunction;
    newLink->test.name = name;
    // Default to true so that a test with no assertions doesn't cause failure
    newLink->test.successful = true;
    newLink->next = NULL;

    TestLink** newTail;
    if (head == NULL) {
      newTail = &head;
    } else {
        TestLink* tail = head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        newTail = &(tail->next);
    }

    *newTail = newLink;
    successCount++;
}

int TestSuite::getTestCount() const {
    return successCount + failureCount;
}

int TestSuite::getFailureCount() const {
    return failureCount;
}

int TestSuite::getSuccessCount() const {
    return successCount;
}

void TestSuite::run() {
    if (completed) {
        return;
    }

    reporter->begin(name);

    TestLink* current = head;
    while (current != NULL) {
      current->test.testFunction(current->test);
      current = current->next;
    }

    completed = true;

    reporter->reportComplete(*this);
}

bool TestSuite::hasCompleted() const {
    return completed;
}

int adjustLineNumber(int lineNumber) {
    // Adjust line number to cater for extra lines added to
    // sketch during compilation
    return lineNumber - 3;
}

void TestSuite::suiteAssertTrue(Test& test, bool condition, int lineNumber) {
    test.successful = condition;

    if (!condition) {
        successCount--;
        failureCount++;
        reporter->reportFailure(test, adjustLineNumber(lineNumber));
    }
}

void TestSuite::suiteAssertEquals(Test& test, int expected, int actual, int lineNumber) {
    bool areEqual = (expected == actual);
    test.successful = areEqual;
    if (!areEqual) {
        successCount--;
        failureCount++;
        char expectedBuffer[11];
        char actualBuffer[11];
        sprintf(expectedBuffer, "%d", expected);
        sprintf(actualBuffer, "%d", actual);
        reporter->reportEqualityFailure(test, adjustLineNumber(lineNumber), expectedBuffer, actualBuffer);
    }
}