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
    if (ImGui::Button("Настройки", ImVec2(100, 30))) {
        state.settings_window_show = true;
    }

    ImGui::SetCursorPos(ImVec2(20, 550));
    if (ImGui::Button("Все текущие страховки", ImVec2(180, 30))) {
        state.all_insurances_show = true;
    }

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(20, 60));
    ImGui::BeginChild("Stats", ImVec2(300, 150), true);
    ImGui::TextColored(BLUE_TITLE, "Статистика компании");
    ImGui::Separator();

    if (state.balance >= 0) {
        ImGui::TextColored(GREEN_COLOR, "Баланс: %d", state.balance);
    }
    else {
        ImGui::TextColored(RED_COLOR, "Баланс: %d", state.balance);
    }

    if (state.monthly_income > 0) {
        ImGui::TextColored(GREEN_COLOR, "Месячный доход: %d", state.monthly_income);
    }
    else {
        ImGui::Text("Месячный доход: %d", state.monthly_income);
    }

    if (state.monthly_payouts > 0) {
        ImGui::TextColored(RED_COLOR, "Выплаты за месяц: %d", state.monthly_payouts);
    }
    else {
        ImGui::Text("Выплаты за месяц: %d", state.monthly_payouts);
    }

    int net_result = state.monthly_income - state.monthly_payouts;
    if (net_result >= 0) {
        ImGui::TextColored(GREEN_COLOR, "Доход в этом месяце: %d", net_result);
    }
    else {
        ImGui::TextColored(RED_COLOR, "Убыток в этом месяце: %d", net_result);
    }

    ImGui::Text("Всего застрахованы: %d", state.total_insured);
    ImGui::Text("Общее покрытие: %d", state.total_covered);
    ImGui::EndChild();
    ResetStyle();

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(20, 230));
    DrawInsuranceStats(state);
    ResetStyle();

    ApplyBlueStyle();
    ImGui::SetCursorPos(ImVec2(880, 20));
    ImGui::BeginChild("Month", ImVec2(300, 60), true);
    ImGui::TextColored(BLUE_TITLE, "Месяц: %d/24", state.curr_month);
    ImGui::EndChild();
    ResetStyle();

    ImGui::End();
}

inline void UI::DrawInsuranceStats(GameState& state) {
    ImGui::BeginChild("InsuranceStats", ImVec2(300, 300), true);
    ImGui::TextColored(BLUE_TITLE, "Тип страховки");
    ImGui::Separator();

    for (int i = 0; i < 3; i++) {
        ImGui::PushID(i);
        ImGui::Text("%s:", state.insurances[i].name.c_str());
        ImGui::Text("  Застрахованы: %d", state.insurances[i].insured_count);
        ImGui::Text("  Покрытие: %d", state.insurances[i].max_payout);
        ImGui::Text("  Вознаграждение: %d", state.insurances[i].monthly_fee);
        ImGui::Text("  Базовый спрос: %d%%", state.insurances[i].base_demand);
        if (i < 2) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::EndChild();
}

inline void UI::DrawEventsWindow(GameState& state) {
    ApplyYellowStyle();
    ImGui::SetNextWindowPos(ImVec2(200, 150), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
    ImGui::Begin("Месячный отчет", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored(YELLOW_TITLE, "Месяц %d Отчет:", state.curr_month);
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
    ImGui::SetNextWindowPos(ImVec2(200, 150), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
    ImGui::Begin("Управление клиентами", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored(YELLOW_TITLE, "Новые клиенты ожидают решения по страховкам:");
    ImGui::Separator();

    for (size_t i = 0; i < state.current_clients.size(); ++i) {
        auto& client = state.current_clients[i];
        ImGui::PushID(static_cast<int>(i));

        ImGui::Text("Клиент %d", (int)i + 1);
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
        ImGui::Text("Риск: %.1f%%", client.risk_factor);
        ImGui::PopStyleColor();

        ImGui::SameLine(250);
        ImGui::Text("Страховка: %s", state.insurances[client.insurance_type].name.c_str());

        ImGui::SameLine(450);
        ImGui::Text("Вознаграждение: %d",
            InsuranceSystem::CalculateMonthlyFee(state.insurances[client.insurance_type], client.risk_factor));

        ImGui::SameLine(600);
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
    ImGui::Begin("Настройки страхования", &state.settings_window_show);

    ImGui::TextColored(GRAY_TITLE, "Настройки условий страхования");
    ImGui::Separator();

    int total_demand = 0;
    for (int i = 0; i < 3; i++) {
        total_demand += state.insurances[i].base_demand;
    }

    if (total_demand != 100) {
        ImGui::TextColored(RED_COLOR, "Итоговый базовый спрос: %d%% (должен быть 100%%)", total_demand);
    }
    else {
        ImGui::Text("Итоговый базовый спрос: 100%%");
    }
    ImGui::Separator();

    for (int i = 0; i < 3; i++) {
        ImGui::PushID(i);
        ImGui::Text("%s", state.insurances[i].name.c_str());

        ImGui::InputInt("Вознаграждение", &state.insurances[i].new_monthly_fee);
        ImGui::InputInt("Max Payout", &state.insurances[i].new_max_payout);
        ImGui::InputInt("Длительность (в месяцах)", &state.insurances[i].contract_duration);
        ImGui::InputInt("Франшиза", &state.insurances[i].franchise);
        ImGui::InputInt("Базовый спрос %", &state.insurances[i].base_demand);

        state.insurances[i].base_demand = std::max(0, std::min(100, state.insurances[i].base_demand));

        if (i < 2) ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::NewLine();
    if (ImGui::Button("Сохранить", ImVec2(100, 30))) {
        state.ApplySettings();
        state.settings_window_show = false;
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
    ImGui::Begin("Все текущие страховки", &state.all_insurances_show);

    ImGui::TextColored(GREEN_TITLE, "Все текущие страховки");
    ImGui::Separator();

    if (state.active_contracts.empty()) {
        ImGui::Text("Нет активных страховок.");
    }
    else {
        ImGui::BeginChild("ContractsTable", ImVec2(0, 400), true);

        ImGui::Columns(5);
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Тип"); ImGui::NextColumn();
        ImGui::Text("Вознаграждение"); ImGui::NextColumn();
        ImGui::Text("Max Payout"); ImGui::NextColumn();
        ImGui::Text("Оставш. срок"); ImGui::NextColumn();
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
        ImGui::Text("Всего текущих страховок: %d", (int)state.active_contracts.size());
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
        ImGui::TextColored(RED_COLOR, "Bankruptcy!");
        ImGui::Separator();
        ImGui::TextWrapped("Ваша компания обанкротилась и не может продолжать работать.");
        //ImGui::NewLine();
        //ImGui::Text("Final balance: %d", state.balance);
    }
    else {
        ImGui::TextColored(GREEN_COLOR, "Win!");
        ImGui::Separator();
        ImGui::TextWrapped("Ваша компания успешно проработала 24 месяца! Поздравляем!");
        ImGui::NewLine();
        ImGui::Text("Финальный баланс: %d", state.balance);
    }

    ImGui::NewLine();
    ImGui::SetCursorPosX(150);
    if (ImGui::Button("Выйти из игры", ImVec2(100, 30))) {
        state.exit_program = true;
    }

    ImGui::End();
}