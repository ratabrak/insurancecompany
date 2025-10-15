#pragma once
#include <string>
#include <vector>
#include <random>

struct InsuranceContract {
    int id;
    int type;
    int monthly_fee;
    int max_payout;
    int duration;
    int remaining_months;
    float client_risk;
    int franchise;
};

struct InsuranceType {
    std::string name;
    int insured_count = 0;
    int max_payout = 0;
    int monthly_fee = 0;

    int new_monthly_fee = 0;
    int new_max_payout = 0;
    int contract_duration = 12;
    int franchise = 0;
    int base_demand = 100;
};

struct Client {
    int insurance_type = -1;
    bool approved = false;
    float risk_factor = 0;
};

struct GameState {
    int balance = 3000000;
    int total_insured = 0;
    int total_covered = 0;
    int monthly_income = 0;
    int monthly_payouts = 0;
    int monthly_tax = 0;
    int curr_month = 1;
    bool event_window_show = true;
    bool clients_window_show = false;
    bool settings_window_show = false;
    bool all_insurances_show = false;
    bool bankrupt = false;
    bool exit_program = false;
    std::vector<std::string> monthly_events;
    std::vector<Client> current_clients;
    std::vector<InsuranceContract> active_contracts;

    float income_tax_rate = 0.09f;

    InsuranceType insurances[3] = {
        {"дома", 0, 1000000, 15000, 15000, 1000000, 12, 0, 40},
        {"машины", 0, 1000000, 15000, 15000, 1000000, 12, 0, 35},
        {"здоровья", 0, 1000000, 15000, 15000, 1000000, 12, 0, 25}
    };

    std::mt19937 rng;
    int next_contract_id = 1;

    GameState();
    void UpdateStats();
    void ProcessMonth();
    void GenerateClients();
    void CalculateMonthlyResults();
    void ApplySettings();
    void CalculateTax();
    int CalculateWeightedInsuranceType();
    void AddInsuranceContract(int type, int fee, int payout, int duration, float risk, int franchise);
    void CheckBankruptcy();
};