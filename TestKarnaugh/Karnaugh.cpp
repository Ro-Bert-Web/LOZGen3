#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestKarnaugh {
	TEST_CLASS(TestKarnaugh) {
	public:

		TEST_METHOD(No_Truth_All_Unknown) {
			KMap map(4);
			std::unordered_set<KBox> sol = map.solve();

			Assert::AreEqual((size_t)0, sol.size());
		}

		TEST_METHOD(One_Truth_All_Unknown) {
			KMap map(4);
			map.set(KState(4, 0), T);
			std::unordered_set<KBox> sol = map.solve();

			Assert::AreEqual((size_t)1, sol.size());
			Assert::AreEqual(0, sol.begin()->specificity());
		}

		TEST_METHOD(One_Truth_One_False) {
			KMap map(4);
			map.set(KState(4, 0), T);
			map.set(KState(4, 1), F);
			std::unordered_set<KBox> sol = map.solve();

			Assert::AreEqual((size_t)1, sol.size());
			Assert::AreEqual(1, sol.begin()->specificity());
			Assert::IsTrue(sol.begin()->contains(KState(4, 0)));
			Assert::IsFalse(sol.begin()->contains(KState(4, 1)));
		}
	};
}
