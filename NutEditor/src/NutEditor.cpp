#include "Editor/EditorApp.h"

#include "Core/Entrypoint.h"


class NutEditor : public Nutz::Application
{
public:
    NutEditor()
    {
        Ref<Nutz::Layer> editorLayer = CreateRef<EditorLayer>(); 
        AttachLayer(editorLayer);
    }
};


Ref<Nutz::Application> CreateApp()
{
    return CreateRef<NutEditor>();
}
