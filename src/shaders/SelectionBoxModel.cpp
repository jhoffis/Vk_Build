
#include <iostream>
#include "SelectionBoxModel.h"
#include "rendering/Model.h"
#include "Villager.h"

namespace SelectionBox {

    Model m_selectionBoxModel{};
    SelectionBoxUBO m_ubo{};
    std::vector<int> m_selected{};

    void init() {
        m_selectionBoxModel.init(
                {
                        .fallbackWidth = 128.f,
                        .fallbackHeight = 128.f,
                        .shaderName = selectionBox,
                }
        );
        auto entity = std::make_shared<Entity>(Entity{});
        entity->size.x = 1.5;
        entity->size.y = 1.5;
        entity->pos.x = -.10;
        entity->pos.y = -.10;
        entity->visible = true;
        m_selectionBoxModel.addEntity(entity, false);
        m_selectionBoxModel.recreateUboBuffer();

        m_renderModels.emplace_back(&m_selectionBoxModel); // TODO this whole init is very strange..? No?
        hide();
    }

    void visible(float x, float y) {
        m_selectionBoxModel.visible = true;
        m_ubo.posOriginal.x = x;
        m_ubo.posOriginal.y = y;
        m_ubo.posNew.x = x;
        m_ubo.posNew.y = y;
    }

    void hide() {
        m_selectionBoxModel.visible = false;
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

        const auto& vills =
                Villager::villsWithinBounds(x0, y0, x1, y1);
        for (auto &vill : vills) {
            std::cout << "vill: " << vill->age << std::endl;
        }
    }

    bool isVisible() {
        return m_selectionBoxModel.visible;
    }

    void destroy() {
        m_selectionBoxModel.destroy();
    }
}