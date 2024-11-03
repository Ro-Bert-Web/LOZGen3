#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestKarnaugh {
	TEST_CLASS(TestKBox) {
	public:

		TEST_METHOD(Get_Out_Of_Bounds) {
			KBox box(3);

			try {
				box.get(3);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}

			try {
				box.get(-1);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}
		}

		TEST_METHOD(Set_Out_Of_Bounds) {
			KBox box(3);

			try {
				box.set(3, T);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}

			try {
				box.set(-1, T);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}
		}

		TEST_METHOD(Contains) {
			KBox box(3);
			box.set(0, T);
			box.set(1, F);
			box.set(2, X);
			
			Assert::IsTrue(box.contains(KState(3, 0b001)));
			Assert::IsTrue(box.contains(KState(3, 0b101)));
			Assert::IsFalse(box.contains(KState(3, 0b011)));
			Assert::IsFalse(box.contains(KState(3, 0b000)));
		}

		TEST_METHOD(Contains_Size_Error) {
			KBox box(3);
			KState state(2, 0);

			try {
				box.contains(state);
				Assert::Fail(L"Expected length error exception");
			}
			catch (const std::length_error& e) {}
		}

		TEST_METHOD(Specificity) {
			KBox box(3);
			box.set(0, T);
			box.set(1, F);
			box.set(2, X);
			Assert::AreEqual(2, box.specificity());
		}
	};
}
