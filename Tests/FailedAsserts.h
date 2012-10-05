/*
 * FailedAsserts.h
 *
 * Created: 5.10.2012 16:46:34
 *  Author: czukowski
 */ 


#ifndef FAILEDASSERTS_H_
#define FAILEDASSERTS_H_

test(failingAssertion) {
	assertTrue(false);
}

test(failingEqualityAssertion) {
	assertEquals(0, 1);
}

test(failingAssertionAfterSuccess) {
	assertTrue(true);
	assertTrue(false);
}

test(failingEqualityAssertionAfterSuccess) {
	assertEquals(0, 0);
	assertEquals(0, 1);
}

test(twoFailingAssertions) {
	assertTrue(false);
	assertTrue(false);
}

test(twoFailingEqualityAssertions) {
	assertEquals(0, 1);
	assertEquals(0, 1);
}

test(failingAssertionAfterSuccessfulEqualityAssertion) {
	assertEquals(0, 0);
	assertTrue(false);
}

test(failingEqualityAssertionAfterSuccessfulAssertion) {
	assertTrue(true);
	assertEquals(0, 1);
}

#endif /* FAILEDASSERTS_H_ */