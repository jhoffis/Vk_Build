
#include "SelectionBoxModel.h"
#include "src/gra_elems/Model.h"

namespace SelectionBox {

    Model m_selectionBoxModel{};
    SelectionBoxUBO m_ubo{};

    void init() {
        m_selectionBoxModel.init(
                {
                        .fallbackWidth = 128.f,
                        .fallbackHeight = 128.f,
                        .shaderName = selectionBox,
                }
        );
        auto entity = new Entity();
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
//        m_selectionBoxModel.visible = false;
    }

    bool isVisible() {
        return m_selectionBoxModel.visible;
    }

    void destroy() {
        m_selectionBoxModel.destroy();
    }
}