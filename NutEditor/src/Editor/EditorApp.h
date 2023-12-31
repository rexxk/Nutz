#pragma once

#include "Nutz.h"


class EditorLayer : public Nutz::Layer
{
public:
    EditorLayer();

    virtual void OnAttach() override;

    virtual void OnUpdate(Nutz::Timestep ts) override;

private:
    Ref<Nutz::Window> m_Window = nullptr;
    Ref<Nutz::Scene> m_Scene = nullptr;
};
