#include "nutz_pch.h"
#include "Swapchain.h"


#include "Core/Application.h"


namespace Nutz
{


	Ref<Swapchain> Swapchain::Get() { return Application::Get().GetWindow()->GetSwapchain(); }



}
