#pragma once
#include <string>
#include <vector>

struct InsuranceType {
    std::string name;
    int insured_count = 0;
    float max_payout = 0;
};

struct GameState {
    float balance = 3000000.0f;
    int total_insured = 0;
    float total_covered = 0.0f;
    int curr_month = 1;
    bool event_window_show = true;
    bool clients_window_show = false;
    std::vector<std::string> monthly_events;

    InsuranceType insurances[3] = {
        {"Home Insurance", 0, 150000},
        {"Car Insurance", 0, 50000},
        {"Health Insurance", 0, 40000}
    };

    void UpdateStats() {
        total_insured = 0;
        total_covered = 0;
        for (auto& ins : insurances) {
            total_insured += ins.insured_count;
            total_covered += ins.insured_count * ins.max_payout;
        }
    }
};