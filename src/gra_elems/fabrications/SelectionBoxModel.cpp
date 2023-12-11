
#include "SelectionBoxModel.h"
#include "src/gra_elems/Model.h"

namespace SelectionBox {

    struct SelectionBoxModel : public Model {
        Model model{};

    };

    Model m_selectionBoxModel{};


    void init() {
        m_selectionBoxModel.init(
                {
                        .shaderName = selectionBox
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
    }

    void destroy() {
        m_selectionBoxModel.destroy();
    }
}