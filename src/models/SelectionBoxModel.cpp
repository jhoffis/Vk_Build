
#include <iostream>
#include "SelectionBoxModel.h"
#include "camera.h"
#include "rendering/Model.h"
#include "Villager.h"
#include "shaders/Shaders.h"
#include "vk/presentation/gra_swap_chain.h"

namespace SelectionBox {

    SelectionBoxUBO m_ubo{};
    std::vector<int> m_selected{};

    void init() {
        Shaders::m_selectionBoxModel.box = 
                Gra_desc::createDescriptorBox(1, {
                    {
                    .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    .bindingNum = 0,
                    .count = 1,
                    .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                    .sizeofUBO = sizeof(SelectionBoxUBO), 
                    },
                    });
        Shaders::m_selectionBoxModel.init(
                1);
        Shaders::m_selectionBoxModel.updateRenderUbo = 
            [](auto uboMem, auto entity, auto index) {
                SelectionBox::m_ubo.aspect = Gra::m_swapChainAspectRatio;
                SelectionBox::m_ubo.resolution.x =
                    Window::WIDTH; // kanskje monitor size istedet?
                SelectionBox::m_ubo.resolution.y = Window::HEIGHT;
                SelectionBox::m_ubo.posCam.x = Camera::m_cam.pos.x;
                SelectionBox::m_ubo.posCam.y = Camera::m_cam.pos.y;
                uboMem->uboStruct = &SelectionBox::m_ubo;
            };

        Shaders::m_selectionBoxModel.initRenderUbo = [](auto uboMem) {
        };

        // auto entity = std::make_shared<Entity>(Entity{
        //         .pos = {-.1, -.1},
        //         .size = {1.5, 1.5},
        //         .visible = true
        //         });
        Shaders::m_selectionBoxModel.spawn({-.1, -.1}, "");
        // Shaders::m_selectionBoxModel.spawn({-.1, -.1}, ""); //FIXME
        // Shaders::m_selectionBoxModel.recreateUboBuffer();

        hide(false);
    }

    void visible(float x, float y) {
        Shaders::m_selectionBoxModel.visible = true;
        m_ubo.posOriginal.x = x;
        m_ubo.posOriginal.y = y;
        m_ubo.posNew.x = x;
        m_ubo.posNew.y = y;
    }

    void hide(bool select) {
        Shaders::m_selectionBoxModel.visible = false;
        m_selected.clear();

        if (!select) return;

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
        Villager::villsWithinBounds(x0, y0, x1, y1, true);
//        for (auto &vill : vills) {
//            vill->entity->selected = true;
//            std::cout << "vill: " << vill->age << std::endl;
//        }
    }

    bool isVisible() {
        return Shaders::m_selectionBoxModel.visible;
    }

    void destroy() {
        Shaders::m_selectionBoxModel.destroy();
    }
}
