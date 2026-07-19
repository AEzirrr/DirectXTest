#include "MaterialInspectorScreen.h"
#include "ImGui/imgui.h"
#include "AGameObject.h"
#include "Material.h"             
#include "UIManager.h"            
#include "InspectorScreen.h"     
#include "Vector4D.h"
#include "Texture.h" 
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

MaterialInspectorScreen::MaterialInspectorScreen(String name) : AUIScreen(name)
{
    refreshAssetCache();
}

MaterialInspectorScreen::~MaterialInspectorScreen()
{
}

void MaterialInspectorScreen::refreshAssetCache()
{
    m_availableTextures.clear();
    std::string assetRoot = "Assets";

    if (!fs::exists(assetRoot)) {
        fs::create_directory(assetRoot);
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(assetRoot)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                m_availableTextures.push_back(entry.path().string());
            }
        }
    }
}

void DrawTextureSelectionPopup(const char* popupId, Material* mat, int mapType, const std::vector<std::string>& availableTextures, char* searchBuffer, void(*refreshCallback)(void))
{
    if (ImGui::BeginPopup(popupId)) {
        ImGui::Text("Select Texture");
        ImGui::Separator();

        if (ImGui::Button("Refresh Assets", ImVec2(-1, 0))) {
            refreshCallback();
        }

        ImGui::Text("Search:");
        ImGui::SameLine();
        ImGui::InputText("##PopSearch", searchBuffer, 128);
        ImGui::Separator();

        if (ImGui::BeginChild("PopTexList", ImVec2(250, 200), true, ImGuiWindowFlags_HorizontalScrollbar)) {

            // "None" option is always displayed at the top
            if (ImGui::Selectable("<None>")) {
                if (mapType == 0) mat->setBaseTexture(nullptr);
                else if (mapType == 1) mat->setNormalTexture(nullptr);
                else if (mapType == 2) mat->setMetallicSmoothnessTexture(nullptr);
                ImGui::CloseCurrentPopup();
            }

            std::string searchStr(searchBuffer);
            std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);

            for (const auto& texPath : availableTextures) {
                fs::path p(texPath);
                std::string filename = p.filename().string();
                std::string lowerFilename = filename;
                std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);

                if (searchStr.empty() || lowerFilename.find(searchStr) != std::string::npos) {
                    ImGui::BeginGroup();
                    if (ImGui::Selectable(filename.c_str(), false, 0, ImVec2(ImGui::CalcTextSize(filename.c_str()).x, 0))) {
                        std::wstring widePathStr(texPath.begin(), texPath.end());
                        Texture* newTex = new Texture(widePathStr.c_str());

                        if (mapType == 0) mat->setBaseTexture(newTex);
                        else if (mapType == 1) mat->setNormalTexture(newTex);
                        else if (mapType == 2) mat->setMetallicSmoothnessTexture(newTex);

                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndGroup();
                }
            }
        }
        ImGui::EndChild();
        ImGui::EndPopup();
    }
}

void MaterialInspectorScreen::drawUI()
{
    ImGui::Begin(getName().c_str());
    ImGui::Separator();

    AUIScreen* inspectorScreen = UIManager::getInstance()->getScreen("Inspector");
    if (inspectorScreen == nullptr)
    {
        ImGui::Text("No active inspector context detected.");
        ImGui::End();
        return;
    }

    InspectorScreen* customInspector = static_cast<InspectorScreen*>(inspectorScreen);
    AGameObject* selectedObject = customInspector->getSelectedObject();
    if (selectedObject == nullptr)
    {
        ImGui::Text("Select an object from the Hierarchy to view its material.");
        ImGui::End();
        return;
    }

    Material* mat = selectedObject->getAssignedMaterial();
    if (mat == nullptr)
    {
        ImGui::Text("This primitive entity does not have an assigned material.");
        ImGui::End();
        return;
    }

    ImGui::Text("Material Asset: %s", mat->getName().c_str());
    ImGui::Separator();

    // Static callbacks for lambda wrapper
    auto refreshCallback = []() {
        MaterialInspectorScreen* screen = static_cast<MaterialInspectorScreen*>(UIManager::getInstance()->getScreen("Material Inspector"));
        if (screen) screen->refreshAssetCache();
        };

    // ALBEDO 
    if (ImGui::Button("o##AlbedoBox", ImVec2(18, 18))) {
        ImGui::OpenPopup("AlbedoMapPopup");
    }
    ImGui::SameLine();
    ImGui::Text("Albedo");
    ImGui::SameLine(180);

    // Color wheel button
    Vector4D currentColor = mat->getAlbedoColor();
    float colorArray[4] = { currentColor.x, currentColor.y, currentColor.z, currentColor.w };
    if (ImGui::ColorButton("AlbedoColorButton", ImVec4(colorArray[0], colorArray[1], colorArray[2], colorArray[3]), 0, ImVec2(40, 18))) {
        ImGui::OpenPopup("UnityColorPickerPopup");
    }

    // Albedo Popups
    DrawTextureSelectionPopup("AlbedoMapPopup", mat, 0, m_availableTextures, m_searchBuffer, refreshCallback);

    if (ImGui::BeginPopup("UnityColorPickerPopup")) {
        ImGui::Text("Color Picker");
        ImGui::Separator();
        if (ImGui::ColorPicker4("##picker", colorArray, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar)) {
            mat->setAlbedoColor(Vector4D(colorArray[0], colorArray[1], colorArray[2], colorArray[3]));
        }
        ImGui::EndPopup();
    }

    // METALLIC
    if (ImGui::Button("o##MetallicBox", ImVec2(18, 18))) {
        ImGui::OpenPopup("MetallicMapPopup");
    }
    ImGui::SameLine();
    ImGui::Text("Metallic");

    DrawTextureSelectionPopup("MetallicMapPopup", mat, 2, m_availableTextures, m_searchBuffer, refreshCallback);

    // Metallic slider
    float metallicVal = mat->getMetallic();
    ImGui::Indent(10.0f);
    ImGui::SetNextItemWidth(180);
    if (ImGui::SliderFloat("##MetallicSlider", &metallicVal, 0.0f, 1.0f)) {
        mat->setMetallic(metallicVal);
    }
    ImGui::Unindent(10.0f);

    // NORMAL 
    if (ImGui::Button("o##NormalBox", ImVec2(18, 18))) {
        ImGui::OpenPopup("NormalMapPopup");
    }
    ImGui::SameLine();
    ImGui::Text("Normal Map");

    DrawTextureSelectionPopup("NormalMapPopup", mat, 1, m_availableTextures, m_searchBuffer, refreshCallback);

    // SMOOTHNESS 
    ImGui::Text("Smoothness");
    float smoothnessVal = mat->getSmoothness();
    ImGui::Indent(10.0f);
    ImGui::SetNextItemWidth(180);
    if (ImGui::SliderFloat("##SmoothnessSlider", &smoothnessVal, 0.0f, 1.0f)) {
        mat->setSmoothness(smoothnessVal);
    }
    ImGui::Unindent(10.0f);

    ImGui::Separator();

    // TILING & OFFSET
    Vector2D tiling = mat->getTiling();
    float tileArr[2] = { tiling.x, tiling.y };
    ImGui::SetNextItemWidth(200);
    if (ImGui::DragFloat2("Tiling", tileArr, 0.05f)) {
        mat->setTiling(Vector2D(tileArr[0], tileArr[1]));
    }

    Vector2D offset = mat->getOffset();
    float offsetArr[2] = { offset.x, offset.y };
    ImGui::SetNextItemWidth(200);
    if (ImGui::DragFloat2("Offset", offsetArr, 0.05f)) {
        mat->setOffset(Vector2D(offsetArr[0], offsetArr[1]));
    }

    ImGui::End();
}