#include "pch.h"
#include "CppUnitTest.h"

#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestDungeonGenerator
{
	TEST_CLASS(TestTrigger)
	{
	public:
		
		TEST_METHOD(Trigger_Enforcement_Symmetrical) {
			Trigger trigger = { {false, true, false, true}, 0 };
			KMap map(5);
	
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					int index = i * 3 + j;
					if (index == 1 || index == 3)
						continue;

					KState state(5, index * 2);
					map.set(state, kVals[i]);
					state.toggle(0);
					map.set(state, kVals[j]);
				}
			}

			map = trigger.enforce(map, 0);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					int index = i * 3 + j;
					if (index == 1 || index == 3)
						continue;

					KState state(5, index * 2);
					KVal neg = map.get(state);
					state.toggle(0);
					KVal pos = map.get(state);

					if (kVals[i] != X && kVals[i] != neg ||
						kVals[j] != X && kVals[j] != pos
					) {
						std::wstringstream ss;
						ss << L"Integrity lost :: State:";
						for (int k = 1; k < state.size; k++)
							ss.put((state.get(k) ? L'A' : L'a') + k);
						ss << " :: " << kValChars[kVals[i]] << " " << kValChars[kVals[j]];
						ss << " :: " << kValChars[neg] << " " << kValChars[pos];
						Assert::Fail(ss.str().c_str());
					}

					if (neg != pos) {
						std::wstringstream ss;
						ss << L"Symmetry lost :: State:";
						for (int k = 1; k < state.size; k++)
							ss.put((state.get(k) ? L'A' : L'a') + k);
						ss << " :: " << kValChars[kVals[i]] << " " << kValChars[kVals[j]];
						ss << " :: " << kValChars[neg] << " " << kValChars[pos];
						Assert::Fail(ss.str().c_str());
					}
				}
			}
		}

		TEST_METHOD(Trigger_Enforcement_Non_Closeable) {
			Trigger trigger = { {false, false, false, false}, 0 };
			KMap map(5);
	
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					int index = i * 3 + j;
					if (index == 1)
						continue;

					KState state(5, index * 2);
					map.set(state, kVals[i]);
					state.toggle(0);
					map.set(state, kVals[j]);
				}
			}

			map = trigger.enforce(map, 0);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					int index = i * 3 + j;
					if (index == 1)
						continue;

					KState state(5, index * 2);
					KVal neg = map.get(state);
					state.toggle(0);
					KVal pos = map.get(state);

					if (kVals[i] != X && kVals[i] != neg ||
						kVals[j] != X && kVals[j] != pos
					) {
						std::wstringstream ss;
						ss << L"Integrity lost :: State:";
						for (int k = 1; k < state.size; k++)
							ss.put((state.get(k) ? L'A' : L'a') + k);
						ss << " :: " << kValChars[kVals[i]] << " " << kValChars[kVals[j]];
						ss << " :: " << kValChars[neg] << " " << kValChars[pos];
						Assert::Fail(ss.str().c_str());
					}

					if (neg == T && pos != T || neg == X && pos == F) {
						std::wstringstream ss;
						ss << L"Symmetry lost :: State:";
						for (int k = 1; k < state.size; k++)
							ss.put((state.get(k) ? L'A' : L'a') + k);
						ss << " :: " << kValChars[kVals[i]] << " " << kValChars[kVals[j]];
						ss << " :: " << kValChars[neg] << " " << kValChars[pos];
						Assert::Fail(ss.str().c_str());
					}
				}
			}
		}
	};
}
