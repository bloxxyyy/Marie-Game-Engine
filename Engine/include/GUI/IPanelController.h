#pragma once

class IPanelView;

class IPanelController {
public:
    virtual ~IPanelController() = default;

    virtual void UpdatePush() = 0;
    virtual void UpdatePull() = 0;
    virtual IPanelView& GetView() = 0;
};