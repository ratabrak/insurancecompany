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
        if (state.bankrupt || state.win) {
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
    if (ImGui::Button("Настройки", ImVec2(100, 30))) {
        state.settings_window_show = true;
    }

    ImGui::SetCursorPos(ImVec2(130, 20));
    if (ImGui::Button("Текущие страховки", ImVec2(180, 30))) {
        state.all_insurances_show = true;
    }

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(20, 60));
    ImGui::BeginChild("Stats", ImVec2(300, 150), true);
    ImGui::TextColored(BLUE_TITLE, "Статистика компании");
    ImGui::Separator();

    if (state.balance >= 0) {
        ImGui::TextColored(GREEN_COLOR, "Баланс: %d руб.", state.balance);
    }
    else {
        ImGui::TextColored(RED_COLOR, "Баланс: %d руб.", state.balance);
    }

    if (state.monthly_income > 0) {
        ImGui::TextColored(GREEN_COLOR, "Доход: %d руб./мес.", state.monthly_income);
    }
    else {
        ImGui::Text("Доход: %d руб./мес.", state.monthly_income);
    }

    if (state.monthly_payouts > 0) {
        ImGui::TextColored(RED_COLOR, "Выплаты за месяц: %d руб.", state.monthly_payouts);
    }
    else {
        ImGui::Text("Выплаты за месяц: %d руб.", state.monthly_payouts);
    }

    int net_result = state.monthly_income - state.monthly_tax - state.monthly_payouts;
    if (net_result >= 0) {
        ImGui::TextColored(GREEN_COLOR, "Прибыль: %d руб.", net_result);
    }
    else {
        ImGui::TextColored(RED_COLOR, "Убыток: %d руб.", -net_result);
    }

    ImGui::Text("Всего застраховано: %d чел.", state.total_insured);
    ImGui::Text("Суммарное покрытие: %d руб.", state.total_covered);
    ImGui::EndChild();
    ResetStyle();

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(20, 230));
    DrawInsuranceStats(state);
    ResetStyle();

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(1090, 20));
    ImGui::BeginChild("Month", ImVec2(90, 30), true);
    ImGui::TextColored(BLUE_TITLE, "Месяц: %d/24", state.curr_month);
    ImGui::EndChild();
    ResetStyle();

    ImGui::End();
}

inline void UI::DrawInsuranceStats(GameState& state) {
    ImGui::BeginChild("InsuranceStats", ImVec2(300, 320), true);
    ImGui::TextColored(BLUE_TITLE, "Тип страховки");
    ImGui::Separator();

    for (int i = 0; i < 3; i++) {
        ImGui::PushID(i);
        ImGui::Text("%s:", state.insurances[i].name.c_str());
        ImGui::Text("  Застраховано: %d чел.", state.insurances[i].insured_count);
        ImGui::Text("  Покрытие: %d руб.", state.insurances[i].max_payout);
        ImGui::Text("  Стоимость: %d руб./мес.", state.insurances[i].monthly_fee);
        ImGui::Text("  Спрос: %d%%", state.insurances[i].base_demand);
        if (i < 2) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::EndChild();
}

inline void UI::DrawEventsWindow(GameState& state) {
    ApplyYellowStyle();
    ImGui::SetNextWindowPos(ImVec2(350, 60), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
    ImGui::Begin("Месячный отчет", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored(YELLOW_TITLE, "Отчет для месяца %d :", state.curr_month);
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
    if (ImGui::Button("Перейти к клиентам", ImVec2(150, 30))) {
        state.event_window_show = false;
        state.clients_window_show = true;
        state.GenerateClients();
    }
    ImGui::End();
    ResetStyle();
}

inline void UI::DrawClientsWindow(GameState& state) {
    ApplyYellowStyle();
    ImGui::SetNextWindowPos(ImVec2(350, 60), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
    ImGui::Begin("Управление клиентами", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored(YELLOW_TITLE, "У Вас новые клиенты!");
    ImGui::Separator();

    for (size_t i = 0; i < state.current_clients.size(); ++i) {
        auto& client = state.current_clients[i];
        ImGui::PushID(static_cast<int>(i));

        ImGui::Text("Клиент %d", (int)i + 1);
        ImGui::SameLine(75);
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
        ImGui::Text("Риск: %.1f%%", client.risk_factor);
        ImGui::PopStyleColor();

        ImGui::SameLine(150);
        ImGui::Text("Страховка: %s", state.insurances[client.insurance_type].name.c_str());

        ImGui::SameLine(300);
        ImGui::Text("Стоимость: %d руб./мес.",
            InsuranceSystem::CalculateMonthlyFee(state.insurances[client.insurance_type], client.risk_factor));

        ImGui::SameLine(500);
        ImGui::Text("Покрытие: %d руб.", client.coverage);

        ImGui::SameLine(700);
        ImGui::Checkbox("Принять", &client.approved);

        if (i < state.current_clients.size() - 1) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::NewLine();
    if (ImGui::Button("Текущий месяц", ImVec2(120, 30))) {
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
    ImGui::Begin("Настройки страховок", &state.settings_window_show);

    int total_demand = 0;
    for (int i = 0; i < 3; i++) {
        total_demand += state.insurances[i].base_demand;
    }

    if (total_demand != 100) {
        ImGui::TextColored(RED_COLOR, "Итоговый спрос: %d%% (должен быть 100%%)", total_demand);
    }
    else {
        ImGui::Text("Итоговый спрос: 100%%");
    }
    ImGui::Separator();

    for (int i = 0; i < 3; i++) {
        ImGui::PushID(i);
        ImGui::Text("%s", state.insurances[i].name.c_str());

        auto coverage_limits = state.GetCoverageLimits(i);
        auto fee_limits = state.GetFeeLimits(state.insurances[i].new_max_payout);
        int max_duration = state.GetMaxContractDuration();
        int max_franchise = state.insurances[i].new_max_payout / 2;

        ImGui::InputInt("Покрытие (руб.)", &state.insurances[i].new_max_payout);
        if (state.insurances[i].new_max_payout < coverage_limits.first) {
            state.insurances[i].new_max_payout = coverage_limits.first;
        }
        else if (state.insurances[i].new_max_payout > coverage_limits.second) {
            state.insurances[i].new_max_payout = coverage_limits.second;
        }

        fee_limits = state.GetFeeLimits(state.insurances[i].new_max_payout);

        ImGui::InputInt("Стоимость (руб./мес.)", &state.insurances[i].new_monthly_fee);
        if (state.insurances[i].new_monthly_fee < fee_limits.first) {
            state.insurances[i].new_monthly_fee = fee_limits.first;
        }
        else if (state.insurances[i].new_monthly_fee > fee_limits.second) {
            state.insurances[i].new_monthly_fee = fee_limits.second;
        }

        ImGui::InputInt("Срок (мес.)", &state.insurances[i].contract_duration);
        if (state.insurances[i].contract_duration < 1) {
            state.insurances[i].contract_duration = 1;
        }
        else if (state.insurances[i].contract_duration > max_duration) {
            state.insurances[i].contract_duration = max_duration;
        }

        ImGui::InputInt("Франшиза (руб.)", &state.insurances[i].franchise);
        if (state.insurances[i].franchise < 0) {
            state.insurances[i].franchise = 0;
        }
        else if (state.insurances[i].franchise > max_franchise) {
            state.insurances[i].franchise = max_franchise;
        }

        ImGui::InputInt("Спрос (%)", &state.insurances[i].base_demand);
        state.insurances[i].base_demand = std::max(0, std::min(100, state.insurances[i].base_demand));

        if (i < 2) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::NewLine();

    if (total_demand != 100) {
        ImGui::BeginDisabled();
    }

    if (ImGui::Button("Сохранить", ImVec2(100, 30))) {
        state.ApplySettings();
        state.settings_window_show = false;
    }

    if (total_demand != 100) {
        ImGui::EndDisabled();
    }

    ImGui::SameLine();
    if (ImGui::Button("Отмена", ImVec2(100, 30))) {
        state.settings_window_show = false;
    }

    ImGui::End();
    ResetStyle();
}

inline void UI::DrawAllInsurancesWindow(GameState& state) {
    ApplyGreenStyle();
    ImGui::SetNextWindowPos(ImVec2(350, 150), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("Текущие страховки", &state.all_insurances_show);

    if (state.active_contracts.empty()) {
        ImGui::Text("Нет активных страховок.");
    }
    else {
        ImGui::BeginChild("ContractsTable", ImVec2(0, 400), true);

        ImGui::Columns(6);
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Тип"); ImGui::NextColumn();
        ImGui::Text("Стоимость (руб./мес.)"); ImGui::NextColumn();
        ImGui::Text("Покрытие (руб.)"); ImGui::NextColumn();
        ImGui::Text("Риск клиента (%)"); ImGui::NextColumn();
        ImGui::Text("Оставш. срок (мес.)"); ImGui::NextColumn();
        ImGui::Separator();

        for (const auto& contract : state.active_contracts) {
            ImGui::Text("%d", contract.id); ImGui::NextColumn();
            ImGui::Text("%s", state.insurances[contract.type].name.c_str()); ImGui::NextColumn();
            ImGui::Text("%d", contract.monthly_fee); ImGui::NextColumn();
            ImGui::Text("%d", contract.max_payout); ImGui::NextColumn();
            ImGui::Text("%.1f", contract.client_risk); ImGui::NextColumn();
            ImGui::Text("%d/%d", contract.remaining_months, contract.duration); ImGui::NextColumn();
        }

        ImGui::EndChild();

        int low_risk = 0, medium_risk = 0, high_risk = 0;
        for (const auto& contract : state.active_contracts) {
            if (contract.client_risk < 1.0f) {
                low_risk++;
            } else if (contract.client_risk < 5.0f) {
                medium_risk++;
            } else {
                high_risk++;
            }
        }

        ImGui::NewLine();
        ImGui::Text("Статистика по рискам:");
        ImGui::Text("  Низкий риск (0.1-1%%): %d клиентов", low_risk);
        ImGui::Text("  Средний риск (1-5%%): %d клиентов", medium_risk);
        ImGui::Text("  Высокий риск (5-10%%): %d клиентов", high_risk);
        ImGui::Text("Всего страховок: %d", (int)state.active_contracts.size());
    }

    ImGui::End();
    ResetStyle();
}

inline void UI::DrawGameOverWindow(GameState& state) {
    ImGui::SetNextWindowPos(ImVec2(400, 300));
    ImGui::SetNextWindowSize(ImVec2(400, 200));
    ImGui::Begin("Игра окончена", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (state.bankrupt) {
        ImGui::TextWrapped("Ваша компания обанкротилась и не может продолжать работать.");
    }
    else if (state.win) {
        ImGui::TextWrapped("Ваша компания успешно проработала 24 месяца! Поздравляем!");
        ImGui::NewLine();
        ImGui::Text("Итоговый баланс: %d руб.", state.balance);
    }

    ImGui::NewLine();
    ImGui::SetCursorPosX(150);
    if (ImGui::Button("Выйти из игры", ImVec2(100, 30))) {
        state.exit_program = true;
    }

    ImGui::End();
}