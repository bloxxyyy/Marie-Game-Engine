#pragma once

class IPanel {
public:
    virtual ~IPanel() = default;
    virtual void OnImGuiRender() = 0;
    bool IsOpen = true;
};