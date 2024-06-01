#include "Shaders.h"
#include "camera.h"
#include "vk/presentation/gra_swap_chain.h"
#include "models/SelectionBoxModel.h"

//void grassUpdateRenderUbo(Gra_Uniform::UBOMem &uboMem,
//                                   const std::shared_ptr<Entity> &entity) {
//    delete static_cast<Gra::UniformBufferObject *>(uboMem.uboStruct);
//    uboMem.uboStruct = new Gra::UniformBufferObject{
//            .pos = entity->pos - Camera::m_cam.pos,
//            .aspect = Gra::m_swapChainAspectRatio,
//    };
//}

//Model Shaders::m_grassModel(
//        "grass",
//        grassUpdateRenderUbo,
//        {
//                vert_ubo,
//                frag_image,
//        },
//        sizeof(Gra::UniformBufferObject),
//        0, 0,
//        {
//                "grass.png"
//        }
//);
//
//Model Shaders::m_villModel{
//        "grass",
//        grassUpdateRenderUbo,
//        {
//                vert_ubo,
//                frag_image,
//        },
//        sizeof(Gra::UniformBufferObject),
//        0, 0,
//        {
//                "unit.png"
//        }
//};
//
//Model Shaders::m_selectionBoxModel{
//        "selectionbox",
//        [](auto uboMem, auto entity) {
//            SelectionBox::m_ubo.aspect = Gra::m_swapChainAspectRatio;
//            SelectionBox::m_ubo.resolution.x = Window::WIDTH; // kanskje monitor size istedet?
//            SelectionBox::m_ubo.resolution.y = Window::HEIGHT;
//            SelectionBox::m_ubo.posCam.x = Camera::m_cam.pos.x;
//            SelectionBox::m_ubo.posCam.y = Camera::m_cam.pos.y;
//            uboMem.uboStruct = &SelectionBox::m_ubo;
//        }, {
//                vert_ubo,
//        },
//        sizeof(SelectionBox::SelectionBoxUBO),
//        128, 128,
//        {}
//};

void Shaders::init() {
    for (auto m : m_renderModels) {
        m->init();
    }
}
