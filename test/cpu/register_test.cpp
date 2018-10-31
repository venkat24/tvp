#include "cpu/register/register.h"

#include <gtest/gtest.h>

using namespace testing;
using namespace cpu;
using namespace std;

class RegisterTest : public Test {
  protected:
	unique_ptr<Register> r;
	unique_ptr<DoubleRegister> dr;

	unique_ptr<Register> l_;
	unique_ptr<Register> r_;
	unique_ptr<PairRegister> pr;

	RegisterTest() {
		r = make_unique<Register>();
		dr = make_unique<DoubleRegister>();

		l_ = make_unique<Register>();
		r_ = make_unique<Register>();
		pr = make_unique<PairRegister>(l_.get(), r_.get());
	}
};

TEST_F(RegisterTest, BasicSetterTest) {
	r->set(0x45);
	EXPECT_EQ(r->get(), 0x45);

	dr->set(0x2445);
	EXPECT_EQ(dr->get(), 0x2445);

	pr->set(0x2445);
	EXPECT_EQ(pr->get(), 0x2445);
	EXPECT_EQ(l_->get(), 0x24);
	EXPECT_EQ(r_->get(), 0x45);
}

TEST_F(RegisterTest, DoubleRegisterHighAndLowBitsTest) {
	dr->set(0x2445);
	EXPECT_EQ(dr->get_high(), 0x24);
	EXPECT_EQ(dr->get_low(), 0x45);

	pr->set(0x3669);
	EXPECT_EQ(pr->get_high(), 0x36);
	EXPECT_EQ(pr->get_low(), 0x69);
}

TEST_F(RegisterTest, GetBitTest) {
	r->set(0x45); // 01000101
	EXPECT_EQ(r->get_bit(7), 0);
	EXPECT_EQ(r->get_bit(6), 1);
	EXPECT_EQ(r->get_bit(5), 0);
	EXPECT_EQ(r->get_bit(4), 0);
	EXPECT_EQ(r->get_bit(3), 0);
	EXPECT_EQ(r->get_bit(2), 1);
	EXPECT_EQ(r->get_bit(1), 0);
	EXPECT_EQ(r->get_bit(0), 1);

	pr->set(0x2445); // 0010010001000101
	EXPECT_EQ(pr->get_bit(15), 0);
	EXPECT_EQ(pr->get_bit(14), 0);
	EXPECT_EQ(pr->get_bit(13), 1);
	EXPECT_EQ(pr->get_bit(12), 0);
	EXPECT_EQ(pr->get_bit(11), 0);
	EXPECT_EQ(pr->get_bit(10), 1);
	EXPECT_EQ(pr->get_bit(9), 0);
	EXPECT_EQ(pr->get_bit(8), 0);
	EXPECT_EQ(pr->get_bit(7), 0);
	EXPECT_EQ(pr->get_bit(6), 1);
	EXPECT_EQ(pr->get_bit(5), 0);
	EXPECT_EQ(pr->get_bit(4), 0);
	EXPECT_EQ(pr->get_bit(3), 0);
	EXPECT_EQ(pr->get_bit(2), 1);
	EXPECT_EQ(pr->get_bit(1), 0);
	EXPECT_EQ(pr->get_bit(0), 1);

	dr->set(0x2445); // 0010010001000101
	EXPECT_EQ(dr->get_bit(15), 0);
	EXPECT_EQ(dr->get_bit(14), 0);
	EXPECT_EQ(dr->get_bit(13), 1);
	EXPECT_EQ(dr->get_bit(12), 0);
	EXPECT_EQ(dr->get_bit(11), 0);
	EXPECT_EQ(dr->get_bit(10), 1);
	EXPECT_EQ(dr->get_bit(9), 0);
	EXPECT_EQ(dr->get_bit(8), 0);
	EXPECT_EQ(dr->get_bit(7), 0);
	EXPECT_EQ(dr->get_bit(6), 1);
	EXPECT_EQ(dr->get_bit(5), 0);
	EXPECT_EQ(dr->get_bit(4), 0);
	EXPECT_EQ(dr->get_bit(3), 0);
	EXPECT_EQ(dr->get_bit(2), 1);
	EXPECT_EQ(dr->get_bit(1), 0);
	EXPECT_EQ(dr->get_bit(0), 1);
}

TEST_F(RegisterTest, SetBitTest) {
	r->set_bit(7, false);
	r->set_bit(6, true);
	r->set_bit(5, false);
	r->set_bit(4, false);
	r->set_bit(3, false);
	r->set_bit(2, true);
	r->set_bit(1, false);
	r->set_bit(0, true);
	EXPECT_EQ(r->get(), 0x45); // 01000101

	pr->set_bit(15, false);
	pr->set_bit(14, false);
	pr->set_bit(13, true);
	pr->set_bit(12, false);
	pr->set_bit(11, false);
	pr->set_bit(10, true);
	pr->set_bit(9, false);
	pr->set_bit(8, false);
	pr->set_bit(7, false);
	pr->set_bit(6, true);
	pr->set_bit(5, false);
	pr->set_bit(4, false);
	pr->set_bit(3, false);
	pr->set_bit(2, true);
	pr->set_bit(1, false);
	pr->set_bit(0, true);
	EXPECT_EQ(pr->get(), 0x2445); // 0010010001000101

	dr->set_bit(15, false);
	dr->set_bit(14, false);
	dr->set_bit(13, true);
	dr->set_bit(12, false);
	dr->set_bit(11, false);
	dr->set_bit(10, true);
	dr->set_bit(9, false);
	dr->set_bit(8, false);
	dr->set_bit(7, false);
	dr->set_bit(6, true);
	dr->set_bit(5, false);
	dr->set_bit(4, false);
	dr->set_bit(3, false);
	dr->set_bit(2, true);
	dr->set_bit(1, false);
	dr->set_bit(0, true);
	EXPECT_EQ(pr->get(), 0x2445); // 0010010001000101
}

TEST_F(RegisterTest, IncrementAndDecrementOperatorTest) {
	r->set(0x45);
	++(*r);
	EXPECT_EQ(r->get(), 0x46);

	dr->set(0x2445);
	++(*dr);
	EXPECT_EQ(dr->get(), 0x2446);

	pr->set(0x2445);
	++(*pr);
	EXPECT_EQ(pr->get(), 0x2446);

	r->set(0x45);
	(*r)++;
	EXPECT_EQ(r->get(), 0x46);

	dr->set(0x2445);
	(*dr)++;
	EXPECT_EQ(dr->get(), 0x2446);

	pr->set(0x2445);
	(*pr)++;
	EXPECT_EQ(pr->get(), 0x2446);

	r->set(0x45);
	--(*r);
	EXPECT_EQ(r->get(), 0x44);

	dr->set(0x2445);
	--(*dr);
	EXPECT_EQ(dr->get(), 0x2444);

	pr->set(0x2445);
	--(*pr);
	EXPECT_EQ(pr->get(), 0x2444);

	r->set(0x45);
	(*r)--;
	EXPECT_EQ(r->get(), 0x44);

	dr->set(0x2445);
	(*dr)--;
	EXPECT_EQ(dr->get(), 0x2444);

	pr->set(0x2445);
	(*pr)--;
	EXPECT_EQ(pr->get(), 0x2444);
}
