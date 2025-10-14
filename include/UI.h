#pragma once
#include "GameState.h"
#include "InsuranceSystem.h"
#include <SFML/Graphics.hpp>
#include "imgui.h"

class UI {
private:
    const ImVec4 BLUE_TITLE = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);
    const ImVec4 BLUE_BORDER = ImVec4(0.1f, 0.3f, 0.7f, 1.0f);
    const ImVec4 YELLOW_TITLE = ImVec4(0.8f, 0.7f, 0.1f, 1.0f);
    const ImVec4 YELLOW_BORDER = ImVec4(0.7f, 0.6f, 0.0f, 1.0f);
    const ImVec4 GREEN_COLOR = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
    const ImVec4 GREEN_TITLE = ImVec4(0.1f, 0.6f, 0.1f, 1.0f);
    const ImVec4 GREEN_BORDER = ImVec4(0.1f, 0.5f, 0.1f, 1.0f);
    const ImVec4 RED_COLOR = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    const ImVec4 GRAY_TITLE = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    const ImVec4 GRAY_BORDER = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

public:
    void Update(GameState& state) {
        if (state.bankrupt || state.curr_month > 24) {
            DrawGameOverWindow(state);
        }
        else {
            DrawMainUI(state);
            if (state.event_window_show) {
                DrawEventsWindow(state);
            }
            if (state.clients_window_show) {
                DrawClientsWindow(state);
            }
            if (state.settings_window_show) {
                DrawSettingsWindow(state);
            }
            if (state.all_insurances_show) {
                DrawAllInsurancesWindow(state);
            }
        }
    }

private:
    void DrawMainUI(GameState& state);
    void DrawEventsWindow(GameState& state);
    void DrawClientsWindow(GameState& state);
    void DrawInsuranceStats(GameState& state);
    void DrawSettingsWindow(GameState& state);
    void DrawAllInsurancesWindow(GameState& state);
    void DrawGameOverWindow(GameState& state);

    void ApplyBlueStyle();
    void ApplyYellowStyle();
    void ApplyGreenStyle();
    void ApplyGrayStyle();
    void ResetStyle();
};

inline void UI::ApplyBlueStyle() {
    ImGui::PushStyleColor(ImGuiCol_TitleBg, BLUE_TITLE);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, BLUE_TITLE);
    ImGui::PushStyleColor(ImGuiCol_Border, BLUE_BORDER);
}

inline void UI::ApplyYellowStyle() {
    ImGui::PushStyleColor(ImGuiCol_TitleBg, YELLOW_TITLE);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, YELLOW_TITLE);
    ImGui::PushStyleColor(ImGuiCol_Border, YELLOW_BORDER);
}

inline void UI::ApplyGreenStyle() {
    ImGui::PushStyleColor(ImGuiCol_TitleBg, GREEN_TITLE);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, GREEN_TITLE);
    ImGui::PushStyleColor(ImGuiCol_Border, GREEN_BORDER);
}

inline void UI::ApplyGrayStyle() {
    ImGui::PushStyleColor(ImGuiCol_TitleBg, GRAY_TITLE);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, GRAY_TITLE);
    ImGui::PushStyleColor(ImGuiCol_Border, GRAY_BORDER);
}

inline void UI::ResetStyle() {
    ImGui::PopStyleColor(3);
}

inline void UI::DrawMainUI(GameState& state) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(1200, 800));
    ImGui::Begin("Main", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse);

    ImGui::SetCursorPos(ImVec2(20, 20));
    if (ImGui::Button("Settings", ImVec2(100, 30))) {
        state.settings_window_show = true;
    }

    ImGui::SetCursorPos(ImVec2(20, 550));
    if (ImGui::Button("All current insurances", ImVec2(180, 30))) {
        state.all_insurances_show = true;
    }

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(20, 60));
    ImGui::BeginChild("Stats", ImVec2(300, 150), true);
    ImGui::TextColored(BLUE_TITLE, "Company Statistics");
    ImGui::Separator();

    if (state.balance >= 0) {
        ImGui::TextColored(GREEN_COLOR, "Balance: %d", state.balance);
    }
    else {
        ImGui::TextColored(RED_COLOR, "Balance: %d", state.balance);
    }

    if (state.monthly_income > 0) {
        ImGui::TextColored(GREEN_COLOR, "Monthly Income: %d", state.monthly_income);
    }
    else {
        ImGui::Text("Monthly Income: %d", state.monthly_income);
    }

    if (state.monthly_payouts > 0) {
        ImGui::TextColored(RED_COLOR, "Monthly Payouts: %d", state.monthly_payouts);
    }
    else {
        ImGui::Text("Monthly Payouts: %d", state.monthly_payouts);
    }

    int net_result = state.monthly_income - state.monthly_payouts;
    if (net_result >= 0) {
        ImGui::TextColored(GREEN_COLOR, "Profit This Month: %d", net_result);
    }
    else {
        ImGui::TextColored(RED_COLOR, "Profit This Month: %d", net_result);
    }

    ImGui::Text("Total Insured: %d", state.total_insured);
    ImGui::Text("Total Coverage: %d", state.total_covered);
    ImGui::EndChild();
    ResetStyle();

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(20, 230));
    DrawInsuranceStats(state);
    ResetStyle();

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(880, 20));
    ImGui::BeginChild("Month", ImVec2(300, 60), true);
    ImGui::TextColored(BLUE_TITLE, "Month: %d/24", state.curr_month);
    ImGui::EndChild();
    ResetStyle();

    ImGui::End();
}

inline void UI::DrawInsuranceStats(GameState& state) {
    ImGui::BeginChild("InsuranceStats", ImVec2(300, 300), true);
    ImGui::TextColored(BLUE_TITLE, "Insurance Types");
    ImGui::Separator();

    for (int i = 0; i < 3; i++) {
        ImGui::PushID(i);
        ImGui::Text("%s:", state.insurances[i].name.c_str());
        ImGui::Text("  Insured: %d", state.insurances[i].insured_count);
        ImGui::Text("  Coverage: %d", state.insurances[i].max_payout);
        ImGui::Text("  Monthly Fee: %d", state.insurances[i].monthly_fee);
        ImGui::Text("  Base Demand: %d%%", state.insurances[i].base_demand);
        if (i < 2) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::EndChild();
}

inline void UI::DrawEventsWindow(GameState& state) {
    ApplyYellowStyle();
    ImGui::SetNextWindowPos(ImVec2(200, 150), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
    ImGui::Begin("Monthly Report", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored(YELLOW_TITLE, "Month %d Report:", state.curr_month);
    ImGui::Separator();

    for (const auto& event : state.monthly_events) {
        if (event.find("income") != std::string::npos || event.find("profit") != std::string::npos) {
            ImGui::TextColored(GREEN_COLOR, "%s", event.c_str());
        }
        else if (event.find("tax") != std::string::npos || event.find("payouts") != std::string::npos) {
            ImGui::TextColored(RED_COLOR, "%s", event.c_str());
        }
        else if (event.find("Income Today") != std::string::npos) {
            try {
                size_t pos = event.find_last_of(" ");
                if (pos != std::string::npos) {
                    std::string value_str = event.substr(pos + 1);
                    int value = std::stoi(value_str);
                    if (value >= 0) {
                        ImGui::TextColored(GREEN_COLOR, "%s", event.c_str());
                    }
                    else {
                        ImGui::TextColored(RED_COLOR, "%s", event.c_str());
                    }
                }
                else {
                    ImGui::Text("%s", event.c_str());
                }
            }
            catch (...) {
                ImGui::Text("%s", event.c_str());
            }
        }
        else {
            ImGui::Text("%s", event.c_str());
        }
    }

    ImGui::NewLine();
    if (ImGui::Button("Continue to Clients", ImVec2(150, 30))) {
        state.event_window_show = false;
        state.clients_window_show = true;
        state.GenerateClients();
    }
    ImGui::End();
    ResetStyle();
}

inline void UI::DrawClientsWindow(GameState& state) {
    ApplyYellowStyle();
    ImGui::SetNextWindowPos(ImVec2(200, 150), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
    ImGui::Begin("Client Management", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored(YELLOW_TITLE, "New clients waiting for insurance decisions:");
    ImGui::Separator();

    for (size_t i = 0; i < state.current_clients.size(); ++i) {
        auto& client = state.current_clients[i];
        ImGui::PushID(static_cast<int>(i));

        ImGui::Text("Client %d", (int)i + 1);
        ImGui::SameLine(100);
        float risk_color = InsuranceSystem::GetRiskColor(client.risk_factor);
        ImVec4 color;
        if (risk_color == 0.3f) {
            color = GREEN_COLOR;
        }
        else if (risk_color == 0.6f) {
            color = YELLOW_TITLE;
        }
        else {
            color = RED_COLOR;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::Text("Risk: %.1f%%", client.risk_factor);
        ImGui::PopStyleColor();

        ImGui::SameLine(250);
        ImGui::Text("Insurance: %s", state.insurances[client.insurance_type].name.c_str());

        ImGui::SameLine(450);
        ImGui::Text("Monthly Fee: %d",
            InsuranceSystem::CalculateMonthlyFee(state.insurances[client.insurance_type], client.risk_factor));

        ImGui::SameLine(600);
        ImGui::Checkbox("Approve", &client.approved);

        if (i < state.current_clients.size() - 1) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::NewLine();
    if (ImGui::Button("Process Month", ImVec2(120, 30))) {
        state.ProcessMonth();
        state.clients_window_show = false;
        state.curr_month++;

        if (state.curr_month <= 24 && !state.bankrupt) {
            state.event_window_show = true;
        }
    }

    ImGui::End();
    ResetStyle();
}

inline void UI::DrawSettingsWindow(GameState& state) {
    ApplyGrayStyle();
    ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("Insurance Settings", &state.settings_window_show);

    ImGui::TextColored(GRAY_TITLE, "Insurance Conditions Settings");
    ImGui::Separator();

    int total_demand = 0;
    for (int i = 0; i < 3; i++) {
        total_demand += state.insurances[i].base_demand;
    }

    if (total_demand != 100) {
        ImGui::TextColored(RED_COLOR, "Total Base Demand: %d%% (should be 100%%)", total_demand);
    }
    else {
        ImGui::Text("Total Base Demand: 100%%");
    }
    ImGui::Separator();

    for (int i = 0; i < 3; i++) {
        ImGui::PushID(i);
        ImGui::Text("%s", state.insurances[i].name.c_str());

        ImGui::InputInt("Monthly Fee", &state.insurances[i].new_monthly_fee);
        ImGui::InputInt("Max Payout", &state.insurances[i].new_max_payout);
        ImGui::InputInt("Contract Duration (months)", &state.insurances[i].contract_duration);
        ImGui::InputInt("Franchise", &state.insurances[i].franchise);
        ImGui::InputInt("Base Demand %", &state.insurances[i].base_demand);

        state.insurances[i].base_demand = std::max(0, std::min(100, state.insurances[i].base_demand));

        if (i < 2) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::NewLine();
    if (ImGui::Button("Apply", ImVec2(100, 30))) {
        state.ApplySettings();
        state.settings_window_show = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(100, 30))) {
        state.settings_window_show = false;
    }

    ImGui::End();
    ResetStyle();
}

inline void UI::DrawAllInsurancesWindow(GameState& state) {
    ApplyGreenStyle();
    ImGui::SetNextWindowPos(ImVec2(350, 150), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("All Current Insurances", &state.all_insurances_show);

    ImGui::TextColored(GREEN_TITLE, "All Active Insurances");
    ImGui::Separator();

    if (state.active_contracts.empty()) {
        ImGui::Text("No active insurances.");
    }
    else {
        ImGui::BeginChild("ContractsTable", ImVec2(0, 400), true);

        ImGui::Columns(5);
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Type"); ImGui::NextColumn();
        ImGui::Text("Monthly Fee"); ImGui::NextColumn();
        ImGui::Text("Max Payout"); ImGui::NextColumn();
        ImGui::Text("Remaining Months"); ImGui::NextColumn();
        ImGui::Separator();

        for (const auto& contract : state.active_contracts) {
            ImGui::Text("%d", contract.id); ImGui::NextColumn();
            ImGui::Text("%s", state.insurances[contract.type].name.c_str()); ImGui::NextColumn();
            ImGui::Text("%d", contract.monthly_fee); ImGui::NextColumn();
            ImGui::Text("%d", contract.max_payout); ImGui::NextColumn();
            ImGui::Text("%d/%d", contract.remaining_months, contract.duration); ImGui::NextColumn();
        }

        ImGui::EndChild();

        ImGui::NewLine();
        ImGui::Text("Total active insurances: %d", (int)state.active_contracts.size());
    }

    ImGui::End();
    ResetStyle();
}

inline void UI::DrawGameOverWindow(GameState& state) {
    ImGui::SetNextWindowPos(ImVec2(400, 300));
    ImGui::SetNextWindowSize(ImVec2(400, 200));
    ImGui::Begin("Game Over", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (state.bankrupt) {
        ImGui::TextColored(RED_COLOR, "Bankruptcy!");
        ImGui::Separator();
        ImGui::TextWrapped("Your company has gone bankrupt and cannot maintain its work.");
        //ImGui::NewLine();
        //ImGui::Text("Final balance: %d", state.balance);
    }
    else {
        ImGui::TextColored(GREEN_COLOR, "Win!");
        ImGui::Separator();
        ImGui::TextWrapped("Your company has been working fine for 24 months! Congrats!");
        ImGui::NewLine();
        ImGui::Text("Final balance: %d", state.balance);
    }

    ImGui::NewLine();
    ImGui::SetCursorPosX(150);
    if (ImGui::Button("Exit Game", ImVec2(100, 30))) {
        state.exit_program = true;
    }

    ImGui::End();
}