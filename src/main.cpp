#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <string>

const int WIDTH = 1200, HEIGHT = 800;

struct Particle {
    float x, y;
    float vx, vy;
    float life, maxLife;
    float size;
    bool isEmber;
};

std::vector<Particle> particles;

float randF(float lo, float hi) {
    return lo + (hi - lo) * (rand() / (float)RAND_MAX);
}

void spawnFire(float cx, float baseY, int count, float windX) {
    for(int i = 0; i < count; i++) {
        Particle p;
        float spread = 120.f;
        p.x = cx + randF(-spread, spread) * (1.f - std::abs(randF(-1,1)));
        p.y = baseY + randF(-10.f, 10.f);
        p.vx = randF(-30.f, 30.f) + windX * 0.3f;
        p.vy = randF(-180.f, -320.f);
        p.maxLife = randF(0.6f, 1.8f);
        p.life = p.maxLife;
        p.size = randF(6.f, 22.f);
        p.isEmber = false;
        particles.push_back(p);
    }
}

void spawnEmber(float cx, float baseY, float windX) {
    Particle p;
    p.x = cx + randF(-80.f, 80.f);
    p.y = baseY + randF(-20.f, 0.f);
    p.vx = randF(-60.f, 60.f) + windX * 0.5f;
    p.vy = randF(-250.f, -400.f);
    p.maxLife = randF(1.f, 2.5f);
    p.life = p.maxLife;
    p.size = randF(2.f, 5.f);
    p.isEmber = true;
    particles.push_back(p);
}

sf::Color fireColor(float t) {
    // t: 0=old(top) -> 1=new(bottom)
    // black -> deep red -> red -> orange -> yellow -> white
    if(t < 0.15f) {
        float s = t / 0.15f;
        return sf::Color((uint8_t)(s*80), 0, 0, (uint8_t)(s*120));
    } else if(t < 0.35f) {
        float s = (t-0.15f)/0.2f;
        return sf::Color((uint8_t)(80+s*175), 0, 0, (uint8_t)(120+s*80));
    } else if(t < 0.55f) {
        float s = (t-0.35f)/0.2f;
        return sf::Color(255, (uint8_t)(s*140), 0, (uint8_t)(200+s*30));
    } else if(t < 0.75f) {
        float s = (t-0.55f)/0.2f;
        return sf::Color(255, (uint8_t)(140+s*100), 0, 230);
    } else if(t < 0.9f) {
        float s = (t-0.75f)/0.15f;
        return sf::Color(255, (uint8_t)(240+s*15), (uint8_t)(s*200), 230);
    } else {
        float s = (t-0.9f)/0.1f;
        return sf::Color(255, 255, (uint8_t)(200+s*55), (uint8_t)(230-s*80));
    }
}

sf::Color emberColor(float t) {
    if(t > 0.7f) return sf::Color(255, 220, 80, 255);
    if(t > 0.4f) return sf::Color(255, 120, 20, (uint8_t)(200*t));
    return sf::Color(180, 40, 0, (uint8_t)(150*t));
}

int main() {
    sf::RenderWindow window(sf::VideoMode({(unsigned)WIDTH, (unsigned)HEIGHT}),
        "Fire Simulation | LMB: Move Fire | W/S: Wind | +/-: Intensity | R: Reset");
    window.setFramerateLimit(60);

    srand(42);

    float fireX = WIDTH / 2.f;
    float fireY = HEIGHT - 60.f;
    float windX = 0.f, windTarget = 0.f;
    int intensity = 18;
    bool lmbHeld = false;

    sf::Font font;
    bool hasFont = font.openFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text hud(font);
    hud.setCharacterSize(15);
    hud.setFillColor(sf::Color::White);
    hud.setOutlineColor(sf::Color::Black);
    hud.setOutlineThickness(1.5f);
    hud.setPosition({10.f, 10.f});

    // Background gradient texture
    sf::RenderTexture bgTex({(unsigned)WIDTH, (unsigned)HEIGHT});
    bgTex.clear(sf::Color::Black);
    sf::VertexArray bgGrad(sf::PrimitiveType::TriangleStrip, 4);
    bgGrad[0] = {sf::Vector2f(0,0),           sf::Color(5,5,15)};
    bgGrad[1] = {sf::Vector2f((float)WIDTH,0), sf::Color(5,5,15)};
    bgGrad[2] = {sf::Vector2f(0,(float)HEIGHT),sf::Color(15,8,5)};
    bgGrad[3] = {sf::Vector2f((float)WIDTH,(float)HEIGHT), sf::Color(20,10,5)};
    bgTex.draw(bgGrad);
    bgTex.display();
    sf::Sprite bgSprite(bgTex.getTexture());

    // Glow circle
    sf::CircleShape glow(200.f);
    glow.setOrigin({200.f, 200.f});

    sf::Clock clock;
    sf::Clock totalClock;
    int frameCount = 0;

    while(window.isOpen()) {
        float dt = std::min(clock.restart().asSeconds(), 0.033f);
        float elapsed = totalClock.getElapsedTime().asSeconds();
        auto mpos = sf::Mouse::getPosition(window);

        while(auto ev = window.pollEvent()) {
            if(ev->is<sf::Event::Closed>()) window.close();
            if(auto* k = ev->getIf<sf::Event::KeyPressed>()) {
                if(k->code == sf::Keyboard::Key::Escape) window.close();
                if(k->code == sf::Keyboard::Key::W) windTarget -= 40.f;
                if(k->code == sf::Keyboard::Key::S) windTarget += 40.f;
                if(k->code == sf::Keyboard::Key::R) { windTarget=0; windX=0; intensity=18; particles.clear(); }
                if(k->code == sf::Keyboard::Key::Equal) intensity = std::min(40, intensity+3);
                if(k->code == sf::Keyboard::Key::Hyphen) intensity = std::max(3, intensity-3);
            }
            if(auto* mb = ev->getIf<sf::Event::MouseButtonPressed>())
                if(mb->button == sf::Mouse::Button::Left) lmbHeld = true;
            if(auto* mb = ev->getIf<sf::Event::MouseButtonReleased>())
                if(mb->button == sf::Mouse::Button::Left) lmbHeld = false;
        }

        if(lmbHeld) {
            fireX = (float)mpos.x;
            fireY = (float)mpos.y;
        }

        // Wind smoothing + natural sway
        windX += (windTarget - windX) * 2.f * dt;
        windX += std::sin(elapsed * 0.7f) * 3.f * dt;

        // Spawn particles
        frameCount++;
        spawnFire(fireX, fireY, intensity, windX);
        if(frameCount % 3 == 0) spawnEmber(fireX, fireY, windX);

        // Update particles
        for(auto& p : particles) {
            p.life -= dt;
            float t = p.life / p.maxLife;
            p.vx += windX * dt * (p.isEmber ? 1.5f : 0.8f);
            p.vy += (p.isEmber ? 20.f : -30.f) * dt; // embers slow, fire rises
            // Turbulence
            p.vx += randF(-20.f, 20.f) * dt;
            p.x += p.vx * dt;
            p.y += p.vy * dt;
            // Embers flutter
            if(p.isEmber) p.vx += std::sin(elapsed*3.f + p.x*0.1f) * 15.f * dt;
        }

        // Remove dead
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [](const Particle& p){ return p.life <= 0; }),
            particles.end()
        );

        // Draw
        window.clear(sf::Color(8, 5, 12));
        window.draw(bgSprite);

        // Ground glow
        glow.setPosition({fireX, fireY + 30.f});
        float glowAlpha = 60.f + std::sin(elapsed*3.f)*15.f;
        glow.setFillColor(sf::Color(255, 80, 10, (uint8_t)glowAlpha));
        window.draw(glow);

        // Draw fire particles (sorted back to front by life)
        for(auto& p : particles) {
            if(p.isEmber) continue;
            float t = p.life / p.maxLife;
            sf::Color col = fireColor(t);
            float s = p.size * t * 1.4f;
            sf::CircleShape c(s, 12);
            c.setOrigin({s, s});
            c.setPosition({p.x, p.y});
            c.setFillColor(col);
            window.draw(c, sf::BlendAdd);
        }

        // Draw embers
        sf::VertexArray emberVA(sf::PrimitiveType::Triangles);
        for(auto& p : particles) {
            if(!p.isEmber) continue;
            float t = p.life / p.maxLife;
            sf::Color col = emberColor(t);
            float s = p.size;
            emberVA.append({{p.x-s, p.y-s}, col});
            emberVA.append({{p.x+s, p.y-s}, col});
            emberVA.append({{p.x,   p.y+s}, col});
        }
        window.draw(emberVA, sf::BlendAdd);

        if(hasFont) {
            hud.setString(
                "LMB: Move Fire  |  W/S: Wind Left/Right  |  +/-: Intensity(" +
                std::to_string(intensity) + ")  |  R: Reset  |  Esc: Quit   Particles: " +
                std::to_string(particles.size())
            );
            window.draw(hud);
        }

        window.display();
    }
    return 0;
}


