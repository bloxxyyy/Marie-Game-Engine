#pragma once

class IPanelView {
public:
    virtual ~IPanelView() = default;
    virtual void OnImGuiRender() = 0;
    bool IsOpen = true;
};