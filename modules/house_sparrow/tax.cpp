#include <gtest/gtest.h>

#include <iostream>
#include <utility>
#include <vector>

// five insurances
// “五险”指的是五种保险，包括养老保险、医疗保险、失业保险、工伤保险和生育保险;“一金”指的是住房公积金
struct SocialInsurances {
    // 养老保险：一种保险形式，旨在为参保者提供退休后的经济保障。参保者每月或每年缴纳一定的保费，到退休时可以领取一定的养老金
    double endowment_insurance = 0;

    // 医疗保险：如果您生病或受伤，用于支付医疗治疗费用的保险，通常由公司为其员工支付
    double medical_insurance = 0;

    // 失业保险：社会保险，为非自愿失业的工人提供失业补偿金，帮助他们度过失业期
    double unemployment_insurance = 0;

    // 工伤保险：一种由雇主或政府提供的保险，旨在为在工作中受伤或患病的员工提供医疗和经济上的支持
    double employment_injury_insurance = 0;

    // 生育保险(maternity insurance)是生育津贴和产假的一种社会根据生育保险政策规定，生育津贴必须由用人单位进行申领
    double maternity_insurance = 0;
};

struct ExemptIncome : SocialInsurances {
    double housing_provident_fund = 0;  // 住房公积金：一种由雇主和雇员共同缴纳的基金，旨在帮助员工购买住房

    double additional_fund = 0;
    double pretax_income_sum;
    double exempt_fund_sum = 0;
};

template <typename Payment>
class MonthlyTaxPayer : public Payment {  // subject to a tax
  public:
    MonthlyTaxPayer(double pretax_income_, double additional_fund_)
      : Payment(pretax_income_), pretax_income(pretax_income_), additional_fund(additional_fund_) {}

    double pretax_income;
    double additional_fund;
};

static std::tuple<double, int> get_tax_proportion_and_quick_deduction(double accumulated_pretax_income) {
    // 速算扣除数（quick calculation deduction）：采用逾额累进税率计税时，简化计较应征税额的一个数据
    if (accumulated_pretax_income <= 36'000) {
        return {0.03, 0};
    } else if (accumulated_pretax_income < 144'000) {
        return {0.10, 2520};
    } else if (accumulated_pretax_income < 300'000) {
        return {0.20, 16920};
    } else if (accumulated_pretax_income < 420'000) {
        return {0.25, 31920};
    } else if (accumulated_pretax_income < 660'000) {
        return {0.30, 52920};
    } else if (accumulated_pretax_income < 960'000) {
        return {0.35, 85920};
    } else {
        return {0.45, 181920};
    }
}

struct MonthlyIncome {
    double pretax_income = 0;
    double aftertax_income = 0;
    double tax = 0;
    double tax_proportion = 0;
    double quick_deduction = 0;
    double accumulated_pretax_income = 0;
    double accumulated_aftax_income = 0;
};

template <typename Payment>
class YearlyTaxPayer {
  public:
    using Payer = MonthlyTaxPayer<Payment>;

    YearlyTaxPayer(double income, double additional_fund) {
        monthly_tax_payers.emplace_back(Payer(income, additional_fund));
    }

    YearlyTaxPayer(size_t n, double income, double additional_fund) {
        monthly_tax_payers.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            monthly_tax_payers.emplace_back(Payer(income, additional_fund));
        }
    }

    YearlyTaxPayer(std::vector<double> incomes, double additional_fund) {
        for (double income : incomes) {
            monthly_tax_payers.emplace_back(Payer(income, additional_fund));
        }
    }

    YearlyTaxPayer(std::vector<std::pair<double, double>> ins) {
        for (auto in : ins) {
            monthly_tax_payers.emplace_back(Payer(in.first, in.second));
        }
    }

    void run() {
        /*
        个人所得税税计算公式
        * 基本减除费用 = 5000
        * 累计基本减除费用 = 员工当年在职月数 * 基本减除费用
        * 累计应纳税所得额 = 累计税前收入 - 累计五险一金（个人缴纳部分）- 累计基本减除费用 - 累计专项附加扣除
        * 当月个税 = (累计应纳税所得额  * 预扣税率 - 速算扣除数) - 累计已缴纳税额
        */
        static constexpr double BASIC_FEE = 5000;
        double accumulated_pretax_income = 0;      // 累计税前收入
        double accumulated_except_income = 0;      // 累计五险一金（个人缴纳部分)
        double accumulated_basic_fee = 0;          // 累计基本减除费用
        double accumulated_additional_income = 0;  // 累计专项附加扣除
        double accumulated_tax = 0;                // 累计已缴纳税额

        for (size_t i = 0; i < monthly_tax_payers.size(); ++i) {
            const auto payer = monthly_tax_payers[i];
            accumulated_pretax_income += payer.pretax_income;
            accumulated_except_income += payer.sum();
            accumulated_additional_income += payer.additional_fund;
            // error: conversion from 'size_t' {aka 'long unsigned int'} to 'double' may change value
            // [-Werror=conversion] Turn off it.
            accumulated_basic_fee = i * BASIC_FEE;

            const double accumulated_subject_income = accumulated_pretax_income - accumulated_except_income -
                                                      accumulated_basic_fee - accumulated_additional_income;
            // std::cout << "accumulated_subject_income=" << accumulated_subject_income
            //           << ", accumulated_pretax_income=" << accumulated_pretax_income
            //           << ", accumulated_except_income=" << accumulated_except_income
            //           << ", accumulated_basic_fee=" << accumulated_basic_fee
            //           << ", accumulated_additional_income=" << accumulated_additional_income << std::endl;

            double tax_raio, quick_deduction;
            std::tie(tax_raio, quick_deduction) = get_tax_proportion_and_quick_deduction(accumulated_subject_income);

            double accumulated_tax_last_month = accumulated_tax;
            accumulated_tax = accumulated_subject_income * tax_raio - quick_deduction;
            double current_month_tax = accumulated_tax - accumulated_tax_last_month;  // 当月个税

            MonthlyIncome actual;
            actual.pretax_income = payer.pretax_income;
            actual.tax = current_month_tax;
            actual.aftertax_income = actual.pretax_income - payer.sum() - actual.tax;  // 税后收入
            actual.tax_proportion = tax_raio;
            actual.quick_deduction = quick_deduction;
            actual.accumulated_pretax_income = accumulated_pretax_income;
            actual_incomes.emplace_back(std::move(actual));
        }
    }

    void print() {
        double total_pretax_income = 0;
        double total_aftertax_income = 0;
        double total_tax = 0;
        std::cout << std::fixed << std::setprecision(3);

        for (size_t idx = 0; idx < actual_incomes.size(); ++idx) {
            auto m = actual_incomes[idx];
            total_pretax_income += m.pretax_income;
            total_aftertax_income += m.aftertax_income;
            total_tax += m.tax;
            std::cout << idx << "] pretax income: " << m.pretax_income << ", aftertax income: " << m.aftertax_income
                      << ", tax: " << m.tax << ", ratio: " << m.tax_proportion << ", deduction:" << m.quick_deduction
                      << std::endl;
        }
        std::cout << "Total pretax income: " << total_pretax_income << ", aftertax income: " << total_aftertax_income
                  << ", total tax:" << total_tax << std::endl;
    }

    std::vector<Payer> monthly_tax_payers;
    std::vector<MonthlyIncome> actual_incomes;
};

struct ProportionOfPayment {
    virtual double endowment_insurance() const = 0;

    virtual double medical_insurance() const = 0;

    virtual double unemployment_insurance() const = 0;

    virtual double employment_injury_insurance() const = 0;

    virtual double maternity_insurance() const = 0;

    virtual double housing_provident_fund() const = 0;

    double sum() const {
        return endowment_insurance() + medical_insurance() + unemployment_insurance() + employment_injury_insurance() +
               maternity_insurance() + housing_provident_fund();
    }
};

class BeijingEmployee : public ProportionOfPayment {
  public:
    BeijingEmployee(double income) : _income(income) {}

    double endowment_insurance() const override { return _income * 0.08; }

    double medical_insurance() const override { return _income * 0.02 + 3; }

    double unemployment_insurance() const override { return _income * 0.002; }

    double employment_injury_insurance() const override { return 0; }

    double maternity_insurance() const override { return 0; }

    double housing_provident_fund() const override { return _income * 0.12; }

  private:
    double _income;
};

class BeijingEmployer : public ProportionOfPayment {
  public:
    BeijingEmployer(double income) : _income(income) {}

    double endowment_insurance() const override { return _income * 0.19; }

    double medical_insurance() const override { return _income * 0.1; }

    double unemployment_insurance() const override { return _income * 0.008; }

    double employment_injury_insurance() const override { return _income * 0.005; }

    double maternity_insurance() const override { return _income * 0.008; }

    double housing_provident_fund() const override { return _income * 0.12; }

  private:
    double _income;
};

TEST(Tax, beigjing_employee) {
    const double income = 10'000;  // 月薪1万
    BeijingEmployee employee(income);
    EXPECT_DOUBLE_EQ(800, employee.endowment_insurance());
    EXPECT_DOUBLE_EQ(203, employee.medical_insurance());
    EXPECT_DOUBLE_EQ(20, employee.unemployment_insurance());
    EXPECT_DOUBLE_EQ(0, employee.employment_injury_insurance());
    EXPECT_DOUBLE_EQ(0, employee.maternity_insurance());
    EXPECT_DOUBLE_EQ(1200, employee.housing_provident_fund());
    EXPECT_DOUBLE_EQ(2223, employee.sum());
}

TEST(Tax, beigjing_employer) {
    const double income = 10'000;  // 月薪1万
    BeijingEmployer employer(income);
    EXPECT_DOUBLE_EQ(1900, employer.endowment_insurance());
    EXPECT_DOUBLE_EQ(1000, employer.medical_insurance());
    EXPECT_DOUBLE_EQ(80, employer.unemployment_insurance());
    EXPECT_DOUBLE_EQ(50, employer.employment_injury_insurance());
    EXPECT_DOUBLE_EQ(80, employer.maternity_insurance());
    EXPECT_DOUBLE_EQ(1200, employer.housing_provident_fund());
    EXPECT_DOUBLE_EQ(4310, employer.sum());
}

TEST(Tax, monthly_tax_payer) {
    const double income = 10'000;  // 月薪1万
    const double additional_fund = 1000;
    MonthlyTaxPayer<BeijingEmployee> employee(income, additional_fund);
    EXPECT_DOUBLE_EQ(800, employee.endowment_insurance());
    EXPECT_DOUBLE_EQ(203, employee.medical_insurance());
    EXPECT_DOUBLE_EQ(20, employee.unemployment_insurance());
    EXPECT_DOUBLE_EQ(0, employee.employment_injury_insurance());
    EXPECT_DOUBLE_EQ(0, employee.maternity_insurance());
    EXPECT_DOUBLE_EQ(1200, employee.housing_provident_fund());
    EXPECT_DOUBLE_EQ(2223, employee.sum());
}

TEST(Tax, year_payer) {
    const double income = 10'000;  // 月薪1万
    const double additional_fund = 1000;
    YearlyTaxPayer<BeijingEmployee> employee(12, income, additional_fund);
    employee.run();
    employee.print();
}

TEST(Tax, year_payer2) {
    const double income = 10'000;  // 月薪1万
    const double additional_fund = 0;
    YearlyTaxPayer<BeijingEmployee> employee(12, income, additional_fund);
    employee.run();
    employee.print();
}