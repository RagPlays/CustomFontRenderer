#ifndef ENGINE_H
#define ENGINE_H

// For use by Engine applications

// --Application Basics--------------------------
#include "Engine/Core/Structs.h"
#include "Engine/Core/Timer.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/Base.h"
// ----------------------------------------------

// --Input / Events -----------------------------
#include "Engine/Core/Keycodes.h"
#include "Engine/Core/MouseCodes.h"
#include "Engine/Core/Input.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
// ----------------------------------------------

// --Renderer------------------------------------
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Model.h"

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/PerspectiveCamera.h"
// ----------------------------------------------
#endif // !ENGINE_H