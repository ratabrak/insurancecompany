#include "InsuranceSystem.h"

int InsuranceSystem::CalculateMonthlyFee(const InsuranceType& insurance, float risk) {
    return static_cast<int>(insurance.monthly_fee * (1.0f + (risk / 100.0f) * 0.5f));
}

bool InsuranceSystem::ShouldApproveClient(float risk, float company_risk_tolerance) {
    return risk < company_risk_tolerance;
}

float InsuranceSystem::GetRiskColor(float risk) {
    if (risk < 1.0f) {
        return 0.3f;
    }
    if (risk < 5.0f) {
        return 0.6f;
    }
    return 1.0f;
}