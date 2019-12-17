#include "cpu/cpu.h"
#include "cpu/mocks/register_mock.h"
#include "memory/mocks/memory_mock.h"

#include <gtest/gtest.h>

using namespace testing;
using namespace cpu;
using namespace memory;
using namespace std;

class CPUArithmeticOpcodeTest : public Test {
  protected:
	unique_ptr<CPU> cpu;

	unique_ptr<MemoryMock> memory;

	unique_ptr<RegisterMock> a;
	unique_ptr<RegisterMock> b;
	unique_ptr<RegisterMock> c;
	unique_ptr<RegisterMock> d;
	unique_ptr<RegisterMock> e;
	unique_ptr<RegisterMock> f;
	unique_ptr<RegisterMock> h;
	unique_ptr<RegisterMock> l;

	unique_ptr<DoubleRegisterMock> af;
	unique_ptr<DoubleRegisterMock> bc;
	unique_ptr<DoubleRegisterMock> de;
	unique_ptr<DoubleRegisterMock> hl;

	unique_ptr<DoubleRegisterMock> sp;
	unique_ptr<DoubleRegisterMock> pc;

	CPUArithmeticOpcodeTest() {
		a = make_unique<RegisterMock>();
		b = make_unique<RegisterMock>();
		c = make_unique<RegisterMock>();
		d = make_unique<RegisterMock>();
		e = make_unique<RegisterMock>();
		f = make_unique<RegisterMock>();
		h = make_unique<RegisterMock>();
		l = make_unique<RegisterMock>();
		sp = make_unique<DoubleRegisterMock>();
		pc = make_unique<DoubleRegisterMock>();
		af = make_unique<DoubleRegisterMock>();
		bc = make_unique<DoubleRegisterMock>();
		de = make_unique<DoubleRegisterMock>();
		hl = make_unique<DoubleRegisterMock>();

		memory = make_unique<MemoryMock>();

		cpu = make_unique<CPU>(move(a), move(b), move(c), move(d), move(e),
		                       move(f), move(h), move(l), move(af), move(bc),
		                       move(de), move(hl), move(sp), move(pc),
		                       memory.get());
	}
};

// TEST_F(CPUArithmeticOpcodeTest, t) {
//    // This is abandoned for now, cause it's mindfuck to actually write
//    // Until I find a nicer way to write tests, I'm not touching this
// }
