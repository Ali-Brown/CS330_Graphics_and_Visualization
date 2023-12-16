#include <core/application.h>    
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <core/camera.h>
#include <stb_image.h>
#include <game_objects/pointLight.h>
#include <game_objects/tableTop.h>
#include <game_objects/computer.h>
#include <game_objects/peanutJar.h>
#include <game_objects/tableLight.h>
#include <game_objects/charger.h>
#include <game_objects/calculator.h>
#include <core/shapes.h> // temp, see if needed

Application::Application(std::string WindowTitle, int width, int height) 
    : _applicationName{/*std::move( WindowTitle )*/WindowTitle}, _width{ width }, _height{ height },
    //camera's width, height, initial-position, and perspective true or false
    _camera{ width, height, {0.f, 0.f, 3.f}, true},
    _cameraLookSpeed {0.15f, 0.15f}
{}

void Application::Run() {
	//Open window
    if (!openWindow()) {
        return;
    }

    //Set up inputs
    setupInputs();

    _running = true;

    //Set up scene
    setUpScene();

	// Run application
	while (_running) {
        //calculate delta time based on computer speed - 
        //how fast computer returns current time
        float currentTime = glfwGetTime();

        if (_lastFrameTime == -1.f) {
            _lastFrameTime = currentTime;
        }

        auto deltaTime = currentTime - _lastFrameTime;
        _lastFrameTime = currentTime;

        if (glfwWindowShouldClose(_window)) {
            _running = false;
            continue;
        }

        //Update application with delta time
        update(deltaTime);

        // Draw
        draw();
	}

    glfwTerminate();
}

bool Application::openWindow() {
    // GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW: window creation
    _window = glfwCreateWindow(800, 600, "CS330_OpenGL_Project", nullptr, nullptr);

    if (!_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, (void*)this);

    // GFLW: whenever the window size changes
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);

        auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

        app->_width = width;
        app->_height = height;

        app->_camera.SetSize(width, height);
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to create GLAD window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Cull back faces
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glFrontFace(GL_CULL_FACE);

    return true;
}

void Application::setupInputs() {
    //app gets pressed key
    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                if (action == GLFW_PRESS) {
                    app->_running = false;
                }
                break;
            }
            case GLFW_KEY_P: {
                if (action == GLFW_PRESS) {
                    app->_camera.SetIsPerspective(!app->_camera.isPerspective());
                }
                break;
            }
            default: {}
        }
    });

    //app gets mouse position
    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
        auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    });

    //app gets mouse scroll offsets to contol camera's field of view amount 
    //for camera zoom in and out
    glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->_camera.IncrementZoom(yOffset * 2);
        //std::cout << "Mouse wheel (" << xOffset << "," << yOffset << ")" << std::endl;
    });

    //mouse scroll offsets
    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        //auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        //app->mousePositionCallback(xpos, ypos);
        //std::cout << "Button (" << button << "," << action << ")" << std::endl;
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT: {
                if (action == GLFW_PRESS) {
                    std::cout << "mouse left button pressed" << std::endl;
                }
                else {

                }
                break;
            }
            case GLFW_MOUSE_BUTTON_MIDDLE: {
                if (action == GLFW_PRESS) {

                }
                else {

                }
                break;
            }
            case GLFW_MOUSE_BUTTON_RIGHT: {
                if (action == GLFW_PRESS) {

                }
                else {

                }
                break;
            }

            default: 
                std::cout << "unhandle mouse button event" << std::endl;
        }

    });

}


void Application::setUpScene()
{
    //LIGHT 1: 
    auto &light = _objects.emplace_back(std::make_unique<PointLight>());
    light->Transform = glm::translate(light->Transform, glm::vec3(-2.f, 1.f, 1.f));

    auto* castLight = reinterpret_cast<PointLight*>(light.get());
    castLight->AmbientColor = { 0.f, 0.f, 0.f };
    castLight->DiffuseColor = { 1.f, 1.f, 1.f };
    castLight->SpecularColor = { 1.f, 1.f, 1.f };
    
    castLight->Constant = 1.f;
    castLight->Linear = 0.35f;
    castLight->Quadratic = 0.44f;
    
    //LIGHT 2: 
    auto& light2 = _objects.emplace_back(std::make_unique<PointLight>());
    light2->Transform = glm::translate(light2->Transform, glm::vec3(2.f, 0.f, 1.f));

    auto* castLight2 = reinterpret_cast<PointLight*>(light2.get());
    castLight2->AmbientColor = { 0.0f, 0.0f, 0.0f };
    castLight2->DiffuseColor = { 1.f, 0.f, 0.f };
    castLight2->SpecularColor = { 1.f, 0.f, 0.f };

    castLight2->Constant = 1.f;
    castLight2->Linear = 0.007f;
    castLight2->Quadratic = 0.0002f;

    //TABLE TOP 
    auto& tableTop = _objects.emplace_back(std::make_unique<TableTop>());

    //COMPUTER
    auto& computer = _objects.emplace_back(std::make_unique<Computer>());
    computer->Transform = glm::translate(computer->Transform, glm::vec3(0.f, 0.f, 0.25f));

    //PEANUT JAR
    auto& peanutJar = _objects.emplace_back(std::make_unique<PeanutJar>());
    peanutJar->Transform = glm::translate(peanutJar->Transform, glm::vec3(1.5f, -0.46f, 0.f));
    peanutJar->Transform = glm::rotate(peanutJar->Transform, glm::radians(90.f), glm::vec3(1, 0, 0));

    //TABLE LIGHT
    auto& tableLight = _objects.emplace_back(std::make_unique<TableLight>());
    tableLight->Transform = glm::translate(tableLight->Transform, glm::vec3(0.f, -0.844f, 0.f));

    //CHARGER
    auto& charger = _objects.emplace_back(std::make_unique<Charger>());
    charger->Transform = glm::translate(charger->Transform, glm::vec3(-1.8f, -0.975f, 0.f));
    charger->Transform = glm::rotate(charger->Transform, glm::radians(45.f), glm::vec3(0, 1, 0));

    //CALCULATOR
    auto& calculator = _objects.emplace_back(std::make_unique<Calculator>());
    calculator->Transform = glm::translate(calculator->Transform, glm::vec3(-1.8f, -0.975f, 1.f));
}

bool Application::update(float deltaTime)
{
    //poll IO events (keys pressed/released, mouse moved etc.)
    glfwPollEvents();

    handleInput(deltaTime);

    //objects update per frame
    for (auto& object : _objects) {
        object->Update(deltaTime);
    }

    return false;
}

bool Application::draw()
{
    //BG COLOR 
    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.2, 0.196f, 0.184f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get Camera View Matrix
    glm::mat4 view = _camera.GetViewMatrix();
    // Get Camera Projection Matrix
    glm::mat4 projection = _camera.GetProjectionMatrix(); 

    SceneParameters sceneParams{
        .ProjectionMatrix = projection,
        .ViewMatrix = view,
        .CameraPosition = _camera.GetPosition(),
        .DirLight = {
            .Direction = glm::normalize(glm::vec3{-0.2f, -0.5f, 1.f}),
            .AmbientColor = {0.1f, 0.2f, 0.05f},
            .DiffuseColor = {1.f, 0.9f, 0.9f},
            .SpecularColor = {1.f, 0.9f, 0.9f}
        }
    };
    
    //Process lighting for all models
    for (auto& model : _objects) {
        model->ProcessLighting(sceneParams);
    }

    //Draw all game_object models
    for (auto& model : _objects) {
        model->Draw(sceneParams);
    }

    // glfw: swap buffers
    glfwSwapBuffers(_window);

    return false;
}

void Application::handleInput(float deltaTime) {
    //contols frame redraw rate affected computer processing power

    auto moveAmount = _moveSpeed * deltaTime;

    if (glfwGetKey(_window, GLFW_KEY_W)) {
        _camera.MoveCamera(Camera::MoveDirection::Forward, moveAmount);
    }
    if (glfwGetKey(_window, GLFW_KEY_S)) {
        _camera.MoveCamera(Camera::MoveDirection::Backward, moveAmount);
    }
    if (glfwGetKey(_window, GLFW_KEY_A)) {
        _camera.MoveCamera(Camera::MoveDirection::Left, moveAmount);
    }
    if (glfwGetKey(_window, GLFW_KEY_D)) {
        _camera.MoveCamera(Camera::MoveDirection::Right, moveAmount);
    }
    if (glfwGetKey(_window, GLFW_KEY_Q)) {
        _camera.MoveCamera(Camera::MoveDirection::Up, moveAmount);
    }
    if (glfwGetKey(_window, GLFW_KEY_E)) {
        _camera.MoveCamera(Camera::MoveDirection::Down, moveAmount);
    }

    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);

    mousePositionCallback(xpos, ypos);
}

void Application::mousePositionCallback(double xpos, double ypos) {
    if (!_firstMouse) {
        _lastMousePosition.x = static_cast<float>(xpos);
        _lastMousePosition.y = static_cast<float>(ypos);

        _firstMouse = true;
    };

    glm::vec2 moveAmount{
        xpos - _lastMousePosition.x,
        _lastMousePosition.y - ypos
    };

    _lastMousePosition.x = static_cast<float>(xpos);
    _lastMousePosition.y = static_cast<float>(ypos);

    _camera.RotateBy(moveAmount.x * _cameraLookSpeed.x, moveAmount.y * _cameraLookSpeed.y);
}




