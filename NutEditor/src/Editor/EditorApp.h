#pragma once

#include "Nutz.h"


class EditorLayer : public Nutz::Layer
{
public:
    EditorLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Nutz::Timestep ts) override;

private:
    Ref<Nutz::Scene> m_Scene = nullptr;

    Ref<Nutz::SceneRenderer> m_SceneRenderer = nullptr;
};
