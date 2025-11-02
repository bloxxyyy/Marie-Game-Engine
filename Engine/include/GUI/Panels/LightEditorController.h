#pragma once
#include "GUI/Panels/IPanelController.h"
#include "GUI/Data/LightData.h"
#include <memory>

class GuiManager;
class Light;
class LightEditorPanel; // Forward-declare the concrete view

class LightEditorController : public IPanelController {
public:
    LightEditorController(Light& targetLight);

    void UpdatePush() override;
    void UpdatePull() override;
    IPanelView& GetView() override;

private:
    Light& m_TargetLight;                     // Reference to the Model
    LightData m_ViewData;                     // The ViewModel
    std::unique_ptr<LightEditorPanel> m_View; // the View
};