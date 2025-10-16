#include "GameState.h"
#include "InsuranceSystem.h"
#include <random>
#include <algorithm>

GameState::GameState() : rng(std::random_device{}()) {
    for (int i = 0; i < 3; i++) {
        insurances[i].new_monthly_fee = insurances[i].monthly_fee;
        insurances[i].new_max_payout = insurances[i].max_payout;
    }
    monthly_events.push_back("Приветствуем! Ваша задача - проработать 24 месяца и не обанкротиться!");
}

void GameState::UpdateStats() {
    total_insured = 0;
    total_covered = 0;
    monthly_income = 0;

    for (const auto& contract : active_contracts) {
        total_insured++;
        total_covered += contract.max_payout;
        monthly_income += contract.monthly_fee;
    }

    for (int i = 0; i < 3; i++) {
        insurances[i].insured_count = 0;
    }

    for (const auto& contract : active_contracts) {
        if (contract.type >= 0 && contract.type < 3) {
            insurances[contract.type].insured_count++;
        }
    }
}

void GameState::ProcessMonth() {
    for (const auto& client : current_clients) {
        if (client.approved && client.insurance_type >= 0 && client.insurance_type < 3) {
            AddInsuranceContract(
                client.insurance_type,
                InsuranceSystem::CalculateMonthlyFee(insurances[client.insurance_type], client.risk_factor),
                client.coverage,
                insurances[client.insurance_type].contract_duration,
                client.risk_factor,
                insurances[client.insurance_type].franchise
            );
        }
    }

    monthly_income = 0;
    for (const auto& contract : active_contracts) {
        monthly_income += contract.monthly_fee;
    }

    CalculateTax();
    balance += monthly_income - monthly_tax;

    CalculateMonthlyResults();
    balance -= monthly_payouts;

    for (auto it = active_contracts.begin(); it != active_contracts.end(); ) {
        it->remaining_months--;
        if (it->remaining_months <= 0) {
            it = active_contracts.erase(it);
        }
        else {
            ++it;
        }
    }

    UpdateStats();
    current_clients.clear();

    CheckBankruptcy();
    CheckWinCondition();
}

void GameState::GenerateClients() {
    current_clients.clear();
    std::uniform_int_distribution<int> client_count(2, 5);
    std::uniform_real_distribution<float> risk(0.1f, 10.0f);
    std::uniform_real_distribution<float> coverage_ratio(0.9f, 1.1f);

    int num_clients = client_count(rng);
    for (int i = 0; i < num_clients; ++i) {
        Client client;
        client.insurance_type = CalculateWeightedInsuranceType();
        client.risk_factor = risk(rng);

        float ratio = coverage_ratio(rng);
        client.coverage = static_cast<int>(insurances[client.insurance_type].max_payout * ratio);

        current_clients.push_back(client);
    }
}

int GameState::CalculateWeightedInsuranceType() {
    int total_demand = 0;
    for (int i = 0; i < 3; i++) {
        total_demand += insurances[i].base_demand;
    }

    std::uniform_int_distribution<int> dist(0, total_demand - 1);
    int random_value = dist(rng);

    int cumulative = 0;
    for (int i = 0; i < 3; i++) {
        cumulative += insurances[i].base_demand;
        if (random_value < cumulative) {
            return i;
        }
    }

    return 0;
}

void GameState::CalculateMonthlyResults() {
    monthly_payouts = 0;
    std::uniform_real_distribution<float> random_percent(0.1f, 100.0f);
    std::uniform_real_distribution<float> damage_ratio(0.0f, 1.0f);

    for (const auto& contract : active_contracts) {
        float random_value = random_percent(rng);

        if (random_value <= contract.client_risk) {
            float damage = damage_ratio(rng);
            int payout = static_cast<int>(contract.max_payout * damage);

            if (payout > contract.franchise) {
                payout -= contract.franchise;
            }
            else {
                payout = 0;
            }

            monthly_payouts += payout;
        }
    }

    monthly_events.clear();
    monthly_events.push_back("Месячный доход (руб./мес.): " + std::to_string(monthly_income));
    monthly_events.push_back("Уплаченные налоги (руб.): " + std::to_string(monthly_tax));
    monthly_events.push_back("Страховые случаи (шт.): " + std::to_string(monthly_payouts));
    monthly_events.push_back("Всего (руб.): " + std::to_string(monthly_income - monthly_tax - monthly_payouts));
}

void GameState::ApplySettings() {
    for (int i = 0; i < 3; i++) {
        insurances[i].monthly_fee = insurances[i].new_monthly_fee;
        insurances[i].max_payout = insurances[i].new_max_payout;
    }
}

void GameState::CalculateTax() {
    monthly_tax = static_cast<int>(monthly_income * income_tax_rate);
}

void GameState::AddInsuranceContract(int type, int fee, int payout, int duration, float risk, int franchise) {
    InsuranceContract contract;
    contract.id = next_contract_id++;
    contract.type = type;
    contract.monthly_fee = fee;
    contract.max_payout = payout;
    contract.duration = duration;
    contract.remaining_months = duration;
    contract.client_risk = risk;
    contract.franchise = franchise;
    active_contracts.push_back(contract);
}

void GameState::CheckBankruptcy() {
    if (balance < 0) {
        bankrupt = true;
        monthly_events.push_back("Банкрот.");
    }
}

void GameState::CheckWinCondition() {
    if (curr_month >= 24 && !bankrupt) {
        win = true;
    }
}