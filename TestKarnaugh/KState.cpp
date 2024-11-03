#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestKarnaugh {
	TEST_CLASS(TestKState) {
	public:

		TEST_METHOD(Get_Out_Of_Bounds) {
			KState state(3);

			try {
				state.get(3);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}

			try {
				state.get(-1);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}
		}

		TEST_METHOD(Set_Out_Of_Bounds) {
			KState state(3);

			try {
				state.set(3);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}

			try {
				state.set(-1);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}
		}

		TEST_METHOD(Clear_Out_Of_Bounds) {
			KState state(3);

			try {
				state.clr(3);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}

			try {
				state.clr(-1);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}
		}

		TEST_METHOD(Toggle_Out_Of_Bounds) {
			KState state(3);

			try {
				state.toggle(3);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}

			try {
				state.toggle(-1);
				Assert::Fail(L"Expected out of bounds error");
			}
			catch (const std::out_of_range& e) {}
		}
	};
}
