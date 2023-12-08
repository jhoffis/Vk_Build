
#include "SelectionBoxModel.h"
#include "src/gra_elems/Model.h"

namespace SelectionBox {

    struct SelectionBoxModel : public Model {
        Model model{};

    };

    Model m_selectionBoxModel{};


    void init() {
//        m_selectionBoxModel.init(
//                {
//                        .shaderName = selectionBox
//                }
//        );
//        m_selectionBoxModel.recreateUboBuffer();
//        m_renderModels.emplace_back(&m_selectionBoxModel); // TODO this whole init is very strange..? No?
    }
}