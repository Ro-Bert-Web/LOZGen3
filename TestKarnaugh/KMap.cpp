#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestKarnaugh {
	TEST_CLASS(TestKMap) {
	public:

		TEST_METHOD(Not) {
			for (int defacto = 0; defacto < 3; defacto++) {
				KMap a(2, kVals[defacto]);

				for (int i = 0; i < 3; i++) {
					KState state(2, i);
					a.set(state, kVals[i]);
				}

				KMap Not = ~a;

				KVal expected[] = { F, T, X };
				for (int i = 0; i < 3; i++) {
					KState state(2, i);
					Assert::AreEqual(kValChars[expected[i]], kValChars[Not.get(state)]);
				}
			}
		}

		void testAnd(KVal aDefacto, KVal bDefacto) {
			KMap a(4, aDefacto), b(4, bDefacto);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					KState state(4, i * 3 + j);
					a.set(state, kVals[i]);
					b.set(state, kVals[j]);
				}
			}

			KMap And = a & b;

			KVal expected[] = { T, F, X, F, F, F, X, F, X };
			for (int i = 0; i < 9; i++) {
				KState state(4, i);
				Assert::AreEqual(kValChars[expected[i]], kValChars[And.get(state)]);
			}
		}

		TEST_METHOD(And) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					testAnd(kVals[i], kVals[j]);
				}
			}
		}

		void testOr(KVal aDefacto, KVal bDefacto) {
			KMap a(4, aDefacto), b(4, bDefacto);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					KState state(4, i * 3 + j);
					a.set(state, kVals[i]);
					b.set(state, kVals[j]);
				}
			}

			KMap Or = a | b;

			KVal expected[] = { T, T, T, T, F, X, T, X, X };
			for (int i = 0; i < 9; i++) {
				KState state(4, i);
				Assert::AreEqual(kValChars[expected[i]], kValChars[Or.get(state)]);
			}
		}

		TEST_METHOD(Or) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					testOr(kVals[i], kVals[j]);
				}
			}
		}

		void testXor(KVal aDefacto, KVal bDefacto) {
			KMap a(4, aDefacto), b(4, bDefacto);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					KState state(4, i * 3 + j);
					a.set(state, kVals[i]);
					b.set(state, kVals[j]);
				}
			}

			KMap Xor = a ^ b;

			KVal expected[] = { F, T, X, T, F, X, X, X, X };
			for (int i = 0; i < 9; i++) {
				KState state(4, i);
				Assert::AreEqual(kValChars[expected[i]], kValChars[Xor.get(state)]);
			}
		}

		TEST_METHOD(Xor) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					testXor(kVals[i], kVals[j]);
				}
			}
		}

		TEST_METHOD(Get_By_Value) {
			for (int defacto = 0; defacto < 3; defacto++) {
				KMap a(2, kVals[defacto]);

				for (int i = 0; i < 3; i++) {
					KState state(2, i);
					a.set(state, kVals[i]);
				}

				for (int i = 0; i < 3; i++) {
					Assert::IsTrue(a.get(kVals[i]).count(KState(2, i)) > 0);

					if (i == defacto) {
						Assert::AreEqual((size_t)2, a.get(kVals[i]).size());
					}
					else {
						Assert::AreEqual((size_t)1, a.get(kVals[i]).size());
					}
				}

				Assert::IsTrue(a.get(kVals[defacto]).count(KState(2, 3)) > 0);
			}
		}

		TEST_METHOD(Defact) {
			KMap a(2, F);
			for (int i = 0; i < 3; i++) {
				KState state(2, i);
				a.set(state, kVals[i]);
			}

			KMap b = a;
			for (int i = 0; i < 3; i++) {
				b.defact(kVals[i]);
				Assert::IsTrue(a == b);
			}
		}

		TEST_METHOD(Exact) {
			for (int defacto = 0; defacto < 3; defacto++) {
				KMap a(2, kVals[defacto]);

				for (int i = 0; i < 3; i++) {
					KState state(2, i);
					a.set(state, kVals[i]);
				}

				Assert::AreEqual(1 + (defacto == 0 ? 1 : 0), a.exact().numberOfStates(T));
				Assert::AreEqual(0, a.exact().numberOfStates(X));
				Assert::AreEqual(2 + (defacto != 0 ? 1 : 0), a.exact().numberOfStates(F));
			}
		}

		TEST_METHOD(Maybe) {
			for (int defacto = 0; defacto < 3; defacto++) {
				KMap a(2, kVals[defacto]);

				for (int i = 0; i < 3; i++) {
					KState state(2, i);
					a.set(state, kVals[i]);
				}

				Assert::AreEqual(2 + (defacto != 1 ? 1 : 0), a.maybe().numberOfStates(T));
				Assert::AreEqual(0, a.maybe().numberOfStates(X));
				Assert::AreEqual(1 + (defacto == 1 ? 1 : 0), a.maybe().numberOfStates(F));
			}
		}

		TEST_METHOD(Given) {
		}

		TEST_METHOD(Invert) {
		}
	};
}
