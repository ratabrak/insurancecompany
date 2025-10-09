#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>
#include "imgui.h"

class UI {
public:
    void Update(GameState& state) {
        DrawMainUI(state);
        if (state.event_window_show) { 
            DrawEventsWindow(state); 
        }
        if (state.clients_window_show) { 
            DrawClientsWindow(state); 
        }
    }

private:
    void DrawMainUI(GameState& state) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(1200, 800));
        ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::BeginChild("Stats", ImVec2(300, 100), true);
        ImGui::Text("Balance: $%.2f", state.balance);
        ImGui::Text("Insured: %d", state.total_insured);
        ImGui::Text("Covered: $%.2f", state.total_covered);
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(880, 20));
        ImGui::BeginChild("Month", ImVec2(300, 100), true);
        ImGui::Text("Current Month: %d/24", state.curr_month);
        ImGui::EndChild();

        ImGui::End();
    }

    void DrawEventsWindow(GameState& state) {
        ImGui::SetNextWindowPos(ImVec2(200, 150));
        ImGui::SetNextWindowSize(ImVec2(800, 500));
        ImGui::Begin("Events", nullptr, ImGuiWindowFlags_NoResize);

        ImGui::Text("Events for month %d:", state.curr_month);
        ImGui::Separator();

        for (const auto& event : state.monthly_events) {
            ImGui::Text("%s", event.c_str());
        }

        if (ImGui::Button("Continue", ImVec2(100, 30))) {
            state.event_window_show = false;
            state.clients_window_show = true;
            GenerateNewClients(state);
        }
        ImGui::End();
    }

    void DrawClientsWindow(GameState& state) {
        ImGui::SetNextWindowPos(ImVec2(200, 150));
        ImGui::SetNextWindowSize(ImVec2(800, 500));
        ImGui::Begin("New Clients", nullptr, ImGuiWindowFlags_NoResize);

        ImGui::Text("New clients waiting for insurance decisions:");
        ImGui::Separator();

        ImGui::Text("1 Home Insurance");
        ImGui::Text("2 Car Insurance");
        ImGui::Text("3 Health Insurance");

        if (ImGui::Button("Process", ImVec2(100, 30))) {
            state.clients_window_show = false;
            state.curr_month++;
            PrepareNextMonth(state);
        }
        ImGui::End();
    }

    void GenerateNewClients(GameState& state) {
        // empty
    }

    void PrepareNextMonth(GameState& state) {
        state.monthly_events.clear();
        state.monthly_events.push_back("1 Paid taxes: k");
        state.monthly_events.push_back("2 Sold m new insurances");
        state.monthly_events.push_back("3 Processed n claims");

        state.UpdateStats();

        if (state.curr_month <= 24) {
            state.event_window_show = true;
        }
    }
};