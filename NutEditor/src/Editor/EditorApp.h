#pragma once

#include "Nutz.h"


class EditorLayer : public Nutz::Layer
{
public:
    EditorLayer();

    virtual void OnUpdate(Nutz::Timestep ts) override;

private:
    Ref<Nutz::Window> m_Window = nullptr;
};
