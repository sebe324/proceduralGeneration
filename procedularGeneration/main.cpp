#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

constexpr uint32_t starColors[10]={0xf1c40fff, 0xf39c12ff, 0xe67e22ff, 0xd35400ff, 0xecf0f1ff,
0x1abc9cff, 0x2ecc71ff,0x3498dbff,0x9b59b6ff,0xc0392b};

constexpr uint32_t planetColors[10]={0xc23616ff,0xc23616ff,0x0097e6ff,0xe1b12cff,0x44bd32ff,
0x718093ff,0x192a56ff, 0xbe2eddff,0x6ab04cff,0x95afc0ff};

const std::string starNames[10]={"Aurora", "Alfa", "Omega", "Magnum", "Giga", "Nocturne", "Stella", "Fat", "Physica", "Astral"};
const std::string planetNames[10]={"Casa","Nox","Gea","Caelum","Oasa","Aera","Portus","Rex","Verisa","Xe"};
struct sPlanet{
double distance=0.0;
double radius=0.0;
double foliage=0.0;
double minerals=0.0;
double water=0.0;
double gases=0.0;
double temperature=0.0;
double population=0.0;
bool ring=false;
std::string name="";
sf::Color color;
};
class cStarSystem{
    public:
    cStarSystem(uint32_t x, uint32_t y, bool generateFullSystem){
        nLehmer=(x& 0xFFFF) << 16 | (y & 0xFFFF);

        starExists=(rndInt(0,20)==1);
        if(!starExists) return;
        color=sf::Color(starColors[rndInt(0,10)]);
        starRadius=rndDouble(10.0,40.0);
        if(!generateFullSystem) return;
        name=starNames[rndInt(0,10)]+"-"+std::to_string(rndInt(1000,9999));
        double distanceFromStar=rndDouble(30.f+starRadius,100.f+starRadius);
        int nPlanets=rndInt(0,10);
        for(int i=0; i<nPlanets; i++){
            sPlanet p;
            p.distance=distanceFromStar;
            p.radius=rndDouble(4.0,20.0);
            distanceFromStar+=rndDouble(40.0+p.radius,60.0+p.radius);
            p.temperature = rndDouble(-300.0,200.0)-p.distance;
            p.foliage=rndDouble(0.0,1.0);
            p.minerals=rndDouble(0.0,1.0);
            p.gases=rndDouble(0.0,1.0);
            p.water=rndDouble(0.0,1.0);
            p.color=sf::Color(planetColors[rndInt(0,10)]);
            if(p.water>0.5){
                p.population=std::max(rndDouble(-3000,10000),0.0);
            }
            p.name=planetNames[rndInt(0,10)]+"-"+std::to_string(rndInt(1000,9999));
            p.ring=rndDouble(0,10)==1;

            planets.push_back(p);
        }

    }
    bool starExists=false;
    double starRadius = 0.0f;
    std::string name;
    std::vector<sPlanet> planets;
    sf::Color color;
    private:
    uint32_t nLehmer=0;
    uint32_t Lehmer32(){
        nLehmer+=0xe120fc15;
        uint64_t tmp;
        tmp=(uint64_t)nLehmer * 0x4a39b70d;
        uint32_t m1=(tmp>>32) ^ tmp;
        tmp=(uint64_t)m1*0x12fad5c9;
        uint32_t m2=(tmp>>32) ^ tmp;
        return m2;
    }

    int rndInt(int min, int max){
        return (Lehmer32() % (max-min))+min;
    }
    double rndDouble(double min, double max){
    return ((double)Lehmer32() / (double)(0x7FFFFFFF))*(max-min)+min;
    }
};

//utilities
sf::RectangleShape createRect(sf::Vector2f pos, sf::Vector2f size, sf::Color color){
sf::RectangleShape rect(size);
rect.setPosition(pos);
rect.setFillColor(color);
return rect;
}
sf::CircleShape createCircle(sf::Vector2f pos, float radius, sf::Color color, sf::Color outlineColor=sf::Color::Black, float outlineThickness=0.f){
sf::CircleShape circle(radius);
circle.setPosition(pos);
circle.setFillColor(color);
circle.setOutlineColor(outlineColor);
circle.setOutlineThickness(outlineThickness);
return circle;
}
sf::Text createText(sf::Vector2f pos, sf::Font& font, std::string txt, int charSize){
sf::Text text;
text.setFont(font);
text.setString(txt);
text.setPosition(pos);
text.setCharacterSize(charSize);
return text;
}

int main(){
    sf::RenderWindow window(sf::VideoMode(1000,1000),"Universe Generator");
    sf::Font font;
    if(!font.loadFromFile("font.ttf")){std::cout<<"ERROR!";}
    sf::Vector2f galaxyOffSet(0,0);
    sf::Clock clock;
    sf::Time dt;
    bool bStarSelected;
    sf::Vector2i vStarSelected;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){

            if(event.type==sf::Event::Closed) window.close();

            /*if(event.type==sf::Event::KeyPressed){



                    if(event.key.code==sf::Keyboard::Space){
                            window.clear();
                           // std::random_device rd;
                           // std::mt19937 mt(rd());
                            //std::uniform_int_distribution<int> dist(0,256);
                     auto tp1 = std::chrono::system_clock::now();
                     sf::VertexArray vertices;
                        for(int x=0; x<window.getSize().x; x++)
                    for(int y=0; y<window.getSize().y; y++){
                            //srand(y<<16 | x);
                        bool bIsStar=false;
                        int nSeed=y<<16 | x;
                        //bIsStar=rand()%256<32;
                        //mt.seed(nSeed);
                        //bIsStar=dist(mt)<32;

                        nLehmer=nSeed;
                        bIsStar=Lehmer32()%256<32;
                        vertices.append(sf::Vertex(sf::Vector2f(x,y),bIsStar ? sf::Color::White : sf::Color::Black));
                    }
                     auto tp2 = std::chrono::system_clock::now();
                    std::chrono::duration<float> elapsedTime=tp2-tp1;
                    sf::Text text; text.setFont(font); text.setString(std::to_string(elapsedTime.count()));
                    text.setPosition(0.f,0.f);
                    text.setFillColor(sf::Color::Red);
                    window.draw(vertices);
                    window.draw(text);
                    window.display();

                    }
        }*/

        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) galaxyOffSet.y-=50.f*dt.asSeconds();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) galaxyOffSet.y+=50.f*dt.asSeconds();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) galaxyOffSet.x-=50.f*dt.asSeconds();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) galaxyOffSet.x+=50.f*dt.asSeconds();
        window.clear();

            sf::Vector2i mousePos=sf::Mouse::getPosition(window);
            mousePos/=16;
            sf::Vector2i mousePosGalaxy(mousePos.x+(int)galaxyOffSet.x, mousePos.y+(int)galaxyOffSet.y);
            int nSectorsX=window.getSize().x/16;
            int nSectorsY=window.getSize().y/16;
            sf::Vector2u screen_sector={0,0};
            for(screen_sector.x=0; screen_sector.x<nSectorsX; screen_sector.x++)
            for(screen_sector.y=0; screen_sector.y<nSectorsY; screen_sector.y++){
                cStarSystem star(screen_sector.x+galaxyOffSet.x,screen_sector.y+galaxyOffSet.y, false);
                if(mousePos.x==screen_sector.x && mousePos.y==screen_sector.y){
                        window.draw(createCircle({screen_sector.x*16-star.starRadius/8,screen_sector.y*16-star.starRadius/8},star.starRadius/4,sf::Color::Black, sf::Color::Yellow,2.f));
                    }
                if(star.starExists){
                    window.draw(createCircle({screen_sector.x*16, screen_sector.y*16}, star.starRadius/8,star.color));
                }
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                cStarSystem star(mousePosGalaxy.x,mousePosGalaxy.y,false);
                if(star.starExists){
                    bStarSelected=true;
                    vStarSelected=mousePosGalaxy;
                }
                else bStarSelected=false;

            }
            if(bStarSelected){
                cStarSystem star(vStarSelected.x,vStarSelected.y,true);
                float starY=window.getSize().y*0.5+(window.getSize().y*0.25-star.starRadius);
                window.draw(createRect({0.f,window.getSize().y*0.5},{window.getSize().x,window.getSize().y*0.5},sf::Color(90,90,90)));
                window.draw(createCircle({30.f,starY},star.starRadius,star.color));
                for(sPlanet planet : star.planets){
                    float planetY=window.getSize().y*0.5+(window.getSize().y*0.25-planet.radius);
                    float planetX=planet.distance+30.f+star.starRadius;
                    window.draw(createCircle({planetX, planetY},planet.radius,planet.color));
                            if(abs(planetX+planet.radius*0.5f-mousePos.x*16)<planet.radius*1.1&&abs(planetY+planet.radius*0.5f-mousePos.y*16)<planet.radius*1.1)
                                window.draw(createText({planetX,planetY-50.f},font,planet.name,25));


                }
                window.draw(createText({30.f,window.getSize().y*0.5},font,star.name,36));

            }
            window.display();
            dt=clock.restart();
    }
return 0;
}
