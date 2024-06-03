
#include <iostream>
#include "SelectionBoxModel.h"
#include "rendering/Model.h"
#include "Villager.h"
#include "shaders/Shaders.h"

namespace SelectionBox {

    SelectionBoxUBO m_ubo{};
    std::vector<int> m_selected{};

    void init() {
        auto entity = std::make_shared<Entity>(Entity{
            .pos = {-.1, -.1},
            .size = {1.5, 1.5},
            .visible = true
        });
        Shaders::m_selectionBoxModel.addEntity(entity, false);
        Shaders::m_selectionBoxModel.recreateUboBuffer();

        hide();
    }

    void visible(float x, float y) {
        Shaders::m_selectionBoxModel.visible = true;
        m_ubo.posOriginal.x = x;
        m_ubo.posOriginal.y = y;
        m_ubo.posNew.x = x;
        m_ubo.posNew.y = y;
    }

    void hide() {
        Shaders::m_selectionBoxModel.visible = false;
        m_selected.clear();

        float x0, y0, x1, y1;

        if (m_ubo.posOriginal.x < m_ubo.posNew.x) {
            x0 = m_ubo.posOriginal.x;
            x1 = m_ubo.posNew.x;
        } else {
            x1 = m_ubo.posOriginal.x;
            x0 = m_ubo.posNew.x;
        }

        if (m_ubo.posOriginal.y < m_ubo.posNew.y) {
            y0 = m_ubo.posOriginal.y;
            y1 = m_ubo.posNew.y;
        } else {
            y1 = m_ubo.posOriginal.y;
            y0 = m_ubo.posNew.y;
        }

        Villager::unselectAll();
        const auto& vills = Villager::villsWithinBounds(x0, y0, x1, y1, true);
        for (auto &vill : vills) {
            vill->entity->selected = true;
            std::cout << "vill: " << vill->age << std::endl;
        }
    }

    bool isVisible() {
        return Shaders::m_selectionBoxModel.visible;
    }

    void destroy() {
        Shaders::m_selectionBoxModel.destroy();
    }
}