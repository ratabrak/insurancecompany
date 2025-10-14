#pragma once
#include "GameState.h"

class InsuranceSystem {
public:
    static int CalculateMonthlyFee(const InsuranceType& insurance, float risk);
    static bool ShouldApproveClient(float risk, float company_risk_tolerance = 6.0f);
    static float GetRiskColor(float risk);
};