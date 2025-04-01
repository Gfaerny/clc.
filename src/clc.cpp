    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <chrono>
    #include <thread>
    #include <fstream>
    #include <filesystem>

    using namespace std;
    using namespace chrono;
    namespace fs = std::filesystem;

    auto start = high_resolution_clock::now();
    long long savedTime = 0;
    bool running = false;
    fs::path homeDir = getenv("HOME");
    fs::path filePath = homeDir / ".clc" / "lt";


    void saveTime(long long total_ms)
{
    if (!fs::exists(filePath.parent_path()))
    {
        fs::create_directory(filePath.parent_path());
    }
    ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << total_ms;
        outFile.close();
    } else {
        cerr << "EXIT ERROR: CAN'T SAVE TIME" << endl;
    }
}

    long long loadTime()
{
    ifstream inFile(filePath);
    long long previousTime = 0;
    if (inFile.is_open())
    {
        inFile >> previousTime;
        inFile.close();
    }
    return previousTime;
}


void onExit() {
    auto now = high_resolution_clock::now();
    if (running) {
        savedTime += duration_cast<milliseconds>(now - start).count();
    }
    saveTime(savedTime);
}

    int main()
{

    atexit(onExit);

    savedTime = loadTime();
    start = high_resolution_clock::now();

    sf::RenderWindow window(sf::VideoMode(500, 300), "clc.", sf::Style::None);
    sf::Font font;
    if (!font.loadFromFile("/usr/share/clc/arial.ttf")) {
        cerr << "ERROR: CAN'T LOAD FONT" << endl;
        return -1;
    }

    sf::Texture closeTexture;
    if (!closeTexture.loadFromFile("/usr/share/clc/closb.png"))
    {
        cerr << "ERROR : CANT'T LOAD -> clos.png!" << endl;
        return -1;
    }

    sf::Sprite closeButton;
    closeButton.setTexture(closeTexture);
    closeButton.setPosition(470, 10);
    float scaleFactor = 0.15f;
    closeButton.setScale(scaleFactor, scaleFactor);

    sf::Text clcText;
    clcText.setFont(font);
    clcText.setPosition(0,0 );
    clcText.setCharacterSize(75);
    clcText.setFillColor(sf::Color::White);

    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(100, 115);

    bool dragging = false;
    sf::Vector2i dragOffset;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (closeButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    window.close();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                dragging = true;
                dragOffset = window.getPosition() - sf::Mouse::getPosition();
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                dragging = false;
            }

            if (event.type == sf::Event::MouseMoved && dragging) {
                window.setPosition(sf::Mouse::getPosition() + dragOffset);
            }

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (running) {
                    auto now = high_resolution_clock::now();
                    savedTime += duration_cast<milliseconds>(now - start).count();
                } else {
                    start = high_resolution_clock::now();
                }
                running = !running;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R &&
            (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)))
            {
            savedTime = 0;
            start = high_resolution_clock::now();
            saveTime(0);
            }

        }

        long long currentTime = savedTime;
        if (running) {
            auto now = high_resolution_clock::now();
            currentTime += duration_cast<milliseconds>(now - start).count();
        }

        long long hours = currentTime / (1000 * 60 * 60);
        currentTime %= (1000 * 60 * 60);
        long long minutes = currentTime / (1000 * 60);
        currentTime %= (1000 * 60);
        long long seconds = currentTime / 1000;
        long long milliseconds = currentTime % 1000;

        timerText.setString(to_string(hours) + " : " + to_string(minutes) + " : " + to_string(seconds) + " : " + to_string(milliseconds));

        clcText.setString("Clc.");
        window.clear();
        window.draw(clcText);
        window.draw(timerText);
        window.draw(closeButton);
        window.display();
    }

    return 0;
}
